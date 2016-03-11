//
// Copyright (c) 2014-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#define __DS2_LOG_CLASS_NAME__ "Target::Thread"

#include "DebugServer2/Host/Platform.h"
#include "DebugServer2/Host/Windows/ExtraWrappers.h"
#include "DebugServer2/Support/Stringify.h"
#include "DebugServer2/Target/Process.h"
#include "DebugServer2/Target/Windows/Thread.h"
#include "DebugServer2/Utils/Log.h"

#include <cinttypes>

#define super ds2::Target::ThreadBase

using ds2::Support::Stringify;

namespace ds2 {
namespace Target {
namespace Windows {

Thread::Thread(Process *process, ThreadId tid, HANDLE handle)
    : super(process, tid), _handle(handle) {
  //
  // Initially the thread is stopped.
  //
  _state = kStopped;
  _stopInfo.event = StopInfo::kEventStop;
  _stopInfo.reason = StopInfo::kReasonNone;
}

Thread::~Thread() { CloseHandle(_handle); }

ErrorCode Thread::resume(int signal, Address const &address) {
  // TODO(sas): Not sure how to translate the signal concept to Windows yet.
  // We'll probably have to get rid of these at some point.
  DS2ASSERT(signal == 0);
  // TODO(sas): Continuing a thread from a given address is not implemented yet.
  DS2ASSERT(!address.valid());

  ErrorCode error = kSuccess;

  if (_state == kStopped || _state == kStepped) {
    ProcessInfo info;

    error = process()->getInfo(info);
    if (error != kSuccess)
      return error;

    BOOL result = ContinueDebugEvent(_process->pid(), _tid, DBG_CONTINUE);
    if (!result)
      return Host::Platform::TranslateError();

    _state = kRunning;
  } else if (_state == kTerminated) {
    error = kErrorProcessNotFound;
  }

  return error;
}

void Thread::updateState() {
  // This function does nothing, because there's no way of querying the state
  // of a thread on Windows without being in the context of a debug event.
  // Instead, what we do is we wait for a debug event, and then call the other
  // overload of this function, `Thread::updateState(DEBUG_EVENT const &)`
  // which will be able to get accurate information about the process.
}

void Thread::updateState(DEBUG_EVENT const &de) {
  DS2ASSERT(de.dwThreadId == _tid);

  switch (de.dwDebugEventCode) {
  case EXCEPTION_DEBUG_EVENT:
    _state = kStopped;
    // We always specify kReasonBreakpoint here, but we should instead analyze
    // what the ExceptionCode is, and set the stop reason accordingly.
    _stopInfo.event = StopInfo::kEventStop;
    _stopInfo.reason = StopInfo::kReasonBreakpoint;
    DS2LOG(
        Debug, "exception from inferior, code=%s, address=%p",
        Stringify::ExceptionCode(de.u.Exception.ExceptionRecord.ExceptionCode),
        de.u.Exception.ExceptionRecord.ExceptionAddress);
    break;

  case LOAD_DLL_DEBUG_EVENT: {
    ErrorCode error;
    std::wstring name = L"<NONAME>";

    ProcessInfo pi;
    error = process()->getInfo(pi);
    if (error != kSuccess)
      goto skip_name;

    if (de.u.LoadDll.lpImageName == nullptr)
      goto skip_name;

    uint64_t ptr;
    // ptr needs to be set to 0 or `if (ptr != 0)` might be true even if the
    // pointer is null, on 32-bit targets.
    ptr = 0;
    error = process()->readMemory(
        reinterpret_cast<uint64_t>(de.u.LoadDll.lpImageName), &ptr,
        pi.pointerSize);
    if (error != kSuccess)
      goto skip_name;

    if (ptr == 0)
      goto skip_name;

    // It seems like all strings passed by the kernel here are guaranteed to be
    // unicode.
    DS2ASSERT(de.u.LoadDll.fUnicode);

    name.clear();
    wchar_t c;
    do {
      error = process()->readMemory(ptr, &c, sizeof(c));
      if (error != kSuccess)
        break;
      name.append(1, c);

      ptr += sizeof(c);
    } while (c != '\0');

  skip_name:
    DS2LOG(Debug, "new DLL loaded: %s, base=%p",
           Host::Platform::WideToNarrowString(name).c_str(),
           de.u.LoadDll.lpBaseOfDll);

    if (de.u.LoadDll.hFile != NULL)
      CloseHandle(de.u.LoadDll.hFile);

    _state = kStopped;
    _stopInfo.event = StopInfo::kEventStop;
    _stopInfo.reason = StopInfo::kReasonLibraryLoad;
  } break;

  case UNLOAD_DLL_DEBUG_EVENT:
    DS2LOG(Debug, "DLL unloaded, base=%p", de.u.UnloadDll.lpBaseOfDll);
    _state = kStopped;
    _stopInfo.event = StopInfo::kEventStop;
    _stopInfo.reason = StopInfo::kReasonLibraryUnload;
    break;

  case OUTPUT_DEBUG_STRING_EVENT:
    _state = kStopped;
    _stopInfo.event = StopInfo::kEventStop;
    _stopInfo.reason = StopInfo::kReasonNone;
    break;

  default:
    DS2BUG("unknown debug event code: %lu", de.dwDebugEventCode);
  }
}
}
}
}
