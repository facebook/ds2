//
// Copyright (c) 2014-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#ifndef __DebugServer2_Target_Windows_Process_h
#define __DebugServer2_Target_Windows_Process_h

#include "DebugServer2/Host/ProcessSpawner.h"
#include "DebugServer2/Target/ProcessBase.h"

namespace ds2 {
namespace Target {
namespace Windows {

class Process : public Target::ProcessBase {
protected:
  HANDLE _handle;
  SoftwareBreakpointManager *_softwareBreakpointManager;
  bool _terminated;

protected:
  Process();

public:
  ~Process() override;

public:
  inline HANDLE handle() const { return _handle; }

protected:
  ErrorCode initialize(ProcessId pid, HANDLE handle, ThreadId tid,
                       HANDLE threadHandle, uint32_t flags);

public:
  ErrorCode detach() override;
  ErrorCode interrupt() override;
  ErrorCode terminate() override;
  bool isAlive() const override;

public:
  ErrorCode suspend() override { return kErrorUnsupported; }
  ErrorCode
  resume(int signal = 0,
         std::set<Thread *> const &excluded = std::set<Thread *>()) override;

public:
  ErrorCode readString(Address const &address, std::string &str, size_t length,
                       size_t *nread = nullptr) override;
  ErrorCode readMemory(Address const &address, void *data, size_t length,
                       size_t *nread = nullptr) override;
  ErrorCode writeMemory(Address const &address, void const *data, size_t length,
                        size_t *nwritten = nullptr) override;

public:
  ErrorCode getMemoryRegionInfo(Address const &address,
                                MemoryRegionInfo &info) override {
    return kErrorUnsupported;
  }

public:
  ErrorCode updateInfo() override;

public:
  SoftwareBreakpointManager *softwareBreakpointManager() const override;
  HardwareBreakpointManager *hardwareBreakpointManager() const override {
    return nullptr;
  }

public:
  bool isELFProcess() const override { return false; }

public:
  ErrorCode allocateMemory(size_t size, uint32_t protection,
                           uint64_t *address) override;
  ErrorCode deallocateMemory(uint64_t address, size_t size) override;

public:
  void resetSignalPass() {}
  void setSignalPass(int signo, bool set) {}

public:
  ErrorCode wait() override;

public:
  static Target::Process *Create(Host::ProcessSpawner &spawner);
  static Target::Process *Attach(ProcessId pid);

public:
  ErrorCode getSharedLibraryInfoAddress(Address &address) {
    return kErrorUnsupported;
  }
  ErrorCode enumerateSharedLibraries(
      std::function<void(SharedLibraryInfo const &)> const &cb);

public:
  Architecture::GDBDescriptor const *getGDBRegistersDescriptor() const override;
  Architecture::LLDBDescriptor const *
  getLLDBRegistersDescriptor() const override;

#if defined(ARCH_ARM)
public:
  int getMaxBreakpoints() const;
  int getMaxWatchpoints() const;
  int getMaxWatchpointSize() const;
#endif
};
}
}
}

#endif // !__DebugServer2_Target_Windows_Process_h
