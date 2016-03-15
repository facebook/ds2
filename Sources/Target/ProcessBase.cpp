//
// Copyright (c) 2014-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#define __DS2_LOG_CLASS_NAME__ "Target::ProcessBase"

#include "DebugServer2/Target/ProcessBase.h"
#include "DebugServer2/Architecture/CPUState.h"
#include "DebugServer2/SoftwareBreakpointManager.h"
#include "DebugServer2/Target/Thread.h"
#include "DebugServer2/Utils/Log.h"

namespace ds2 {
namespace Target {

ProcessBase::ProcessBase()
    : _flags(0), _pid(kAnyProcessId), _loadBase(), _entryPoint(),
      _currentThread(nullptr) {}

ProcessBase::~ProcessBase() {
  for (auto thread : _threads) {
    delete thread.second;
  }
  _threads.clear();
  _currentThread = nullptr;
}

ErrorCode ProcessBase::getInfo(ProcessInfo &info) {
  ErrorCode error = updateInfo();
  if (error == kSuccess || error == kErrorAlreadyExist) {
    info = _info;
    error = kSuccess;
  }
  return error;
}

//
// This is a utility function for detach.
//
void ProcessBase::cleanup() {
  std::set<Thread *> threads;
  for (auto thread : _threads) {
    threads.insert(thread.second);
  }
  for (auto thread : threads) {
    removeThread(thread->tid());
  }
  _threads.clear();
  _currentThread = nullptr;
}

ErrorCode ProcessBase::initialize(ProcessId pid, uint32_t flags) {
  if (_pid != kAnyProcessId)
    return kErrorAlreadyExist;

  _pid = pid;
  _flags = flags;

  //
  // Update process information immediatly.
  //
  ErrorCode error = updateInfo();
  if (error != kSuccess) {
    _pid = kAnyProcessId;
    _flags = 0;
  }

  return error;
}

// ELF only
ErrorCode ProcessBase::getAuxiliaryVector(std::string &auxv) {
  return kErrorUnsupported;
}

uint64_t ProcessBase::getAuxiliaryVectorValue(uint64_t type) { return 0; }

ErrorCode
ProcessBase::enumerateThreads(std::function<void(Thread *)> const &cb) const {
  if (_pid == kAnyProcessId)
    return kErrorProcessNotFound;

  for (auto const &it : _threads) {
    static_cast<ThreadBase *>(it.second)->updateState();
    cb(it.second);
  }

  return kSuccess;
}

void ProcessBase::getThreadIds(std::vector<ThreadId> &tids) {
  tids.clear();
  for (auto const &it : _threads) {
    tids.push_back(it.first);
  }
}

ds2::Target::Thread *ProcessBase::thread(ThreadId tid) const {
  auto it = _threads.find(tid);
  return (it == _threads.end()) ? nullptr : it->second;
}

ErrorCode ProcessBase::readMemoryBuffer(Address const &address, size_t length,
                                        std::string &buffer) {
  if (_pid == kAnyProcessId)
    return kErrorProcessNotFound;
  else if (!address.valid())
    return kErrorInvalidArgument;

  buffer.resize(length);

  size_t nread;
  ErrorCode error = readMemory(address, &buffer[0], length, &nread);
  if (error != kSuccess) {
    buffer.clear();
    return error;
  }

  buffer.resize(nread);
  return kSuccess;
}

ErrorCode ProcessBase::writeMemoryBuffer(Address const &address,
                                         std::string const &buffer,
                                         size_t *nwritten) {
  if (_pid == kAnyProcessId)
    return kErrorProcessNotFound;
  else if (!address.valid())
    return kErrorInvalidArgument;

  return writeMemory(address, &buffer[0], buffer.length(), nwritten);
}

ErrorCode ProcessBase::writeMemoryBuffer(Address const &address,
                                         std::string const &buffer,
                                         size_t length, size_t *nwritten) {
  if (_pid == kAnyProcessId)
    return kErrorProcessNotFound;
  else if (!address.valid())
    return kErrorInvalidArgument;

  if (length > buffer.length()) {
    length = buffer.length();
  }

  return writeMemory(address, &buffer[0], length, nwritten);
}

void ProcessBase::insert(ThreadBase *thread) {
  if (!_threads
           .insert(std::make_pair(thread->tid(), static_cast<Thread *>(thread)))
           .second)
    return;

  DS2LOG(Debug, "[New Thread %p (LWP %" PRIu64 ")]", thread,
         (uint64_t)thread->tid());
}

void ProcessBase::removeThread(ThreadId tid) {
  auto it = _threads.find(tid);
  if (it == _threads.end())
    return;

  Thread *thread = it->second;
  _threads.erase(it);

  DS2LOG(Debug, "[Thread %p (LWP %" PRIu64 ") exited]", thread,
         (uint64_t)thread->tid());

  delete thread;
}

void ProcessBase::remove(ThreadBase *thread) { removeThread(thread->tid()); }

bool ProcessBase::isSingleStepSupported() const { return true; }

ErrorCode ProcessBase::beforeResume() {
  if (!isAlive())
    return kErrorProcessNotFound;

  SoftwareBreakpointManager *bpm = softwareBreakpointManager();

  //
  // Enable breakpoints.
  //
  if (bpm != nullptr) {
    bpm->enable();
  }

  return kSuccess;
}

ErrorCode ProcessBase::afterResume() {
  if (!isAlive())
    return kSuccess;

  DS2LOG(Debug, "process still alive, _pid=%" PRIu64, (uint64_t)_pid);

  SoftwareBreakpointManager *bpm = softwareBreakpointManager();

  //
  // Disable breakpoints and try to hit the breakpoint.
  //
  if (bpm != nullptr) {
    bpm->disable();

    for (auto it : _threads) {
      if (bpm->hit(it.second)) {
        DS2LOG(Debug, "hit breakpoint for tid %" PRIu64,
               (uint64_t)it.second->tid());
      }
    }
  }

  return kSuccess;
}

void ProcessBase::prepareForDetach() {
  SoftwareBreakpointManager *bpm = softwareBreakpointManager();
  if (bpm != nullptr) {
    bpm->clear();
  }
}
}
}
