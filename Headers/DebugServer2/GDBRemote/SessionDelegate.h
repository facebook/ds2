//
// Copyright (c) 2014, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#ifndef __DebugServer2_GDBRemote_SessionDelegate_h
#define __DebugServer2_GDBRemote_SessionDelegate_h

#include "DebugServer2/GDBRemote/Types.h"

namespace ds2 {
namespace GDBRemote {

class Session;

class SessionDelegate {
protected:
  friend class Session;

public:
  virtual ~SessionDelegate();

protected: // General Information
  virtual size_t getGPRSize() const = 0;

protected: // Common
  virtual ErrorCode onEnableExtendedMode(Session &session) = 0;
  virtual ErrorCode onSetBaudRate(Session &session, uint32_t speed) = 0;
  virtual ErrorCode onToggleDebugFlag(Session &session) = 0;

  virtual ErrorCode onSetMaxPacketSize(Session &session, size_t size) = 0;
  virtual ErrorCode onSetMaxPayloadSize(Session &session, size_t size) = 0;

  virtual void onSetLogging(Session &session, std::string const &mode,
                            std::string const &filename,
                            StringCollection const &flags) = 0;

  virtual ErrorCode
  onAllowOperations(Session &session,
                    std::map<std::string, bool> const &operations) = 0;
  virtual ErrorCode onQuerySupported(Session &session,
                                     Feature::Collection const &remoteFeatures,
                                     Feature::Collection &localFeatures) = 0;

  virtual ErrorCode onExecuteCommand(Session &session,
                                     std::string const &command) = 0;

  virtual ErrorCode onQueryServerVersion(Session &session,
                                         ServerVersion &version) = 0;
  virtual ErrorCode onQueryHostInfo(Session &session, HostInfo &info) = 0;

protected: // Debugging Session
  virtual ErrorCode onEnableControlAgent(Session &session, bool enable) = 0;
  virtual ErrorCode onNonStopMode(Session &session, bool enable) = 0;
  virtual ErrorCode onEnableBTSTracing(Session &session, bool enable) = 0;

  virtual ErrorCode onPassSignals(Session &session,
                                  std::vector<int> const &signals) = 0;
  virtual ErrorCode onProgramSignals(Session &session,
                                     std::vector<int> const &signals) = 0;

  virtual ErrorCode onQuerySymbol(Session &session, std::string const &name,
                                  std::string const &value,
                                  std::string &next) = 0;

  virtual ErrorCode onQueryRegisterInfo(Session &session, uint32_t regno,
                                        RegisterInfo &info) = 0;

  virtual ErrorCode onAttach(Session &session, ProcessId pid, AttachMode mode,
                             StopCode &) = 0;
  virtual ErrorCode onAttach(Session &session, std::string const &name,
                             AttachMode mode, StopCode &) = 0;
  virtual ErrorCode onRunAttach(Session &session, std::string const &filename,
                                StringCollection const &arguments,
                                StopCode &stop) = 0;
  virtual ErrorCode onDetach(Session &session, ProcessId pid, bool stopped) = 0;
  virtual ErrorCode onQueryAttached(Session &session, ProcessId pid,
                                    bool &attachedProcess) = 0;
  virtual ErrorCode onQueryProcessInfo(Session &session, ProcessInfo &info) = 0;
  virtual ErrorCode onQueryThreadStopInfo(Session &session,
                                          ProcessThreadId const &ptid,
                                          bool list, StopCode &stop) = 0;

  virtual ErrorCode onQueryHardwareWatchpointCount(Session &session,
                                                   size_t &count) = 0;

  virtual ErrorCode onQuerySectionOffsets(Session &session, Address &text,
                                          Address &data, bool &isSegment) = 0;
  virtual ErrorCode onQuerySharedLibrariesInfoAddress(Session &session,
                                                      Address &address) = 0;
  virtual ErrorCode onQuerySharedLibraryInfo(Session &session,
                                             std::string const &path,
                                             std::string const &triple,
                                             SharedLibraryInfo &info) = 0;

  virtual ErrorCode onRestart(Session &session, ProcessId pid) = 0;
  virtual ErrorCode onInterrupt(Session &session) = 0;
  virtual ErrorCode onTerminate(Session &session, ProcessThreadId const &ptid,
                                StopCode &stop) = 0;
  virtual ErrorCode onTerminate(Session &session, ProcessId pid) = 0;

  virtual ErrorCode onSynchronizeThreadState(Session &session,
                                             ProcessId pid) = 0;

  //
  // If lastTid is kAllThreadId, it's the first request; if it's kAnyThreadId
  // the next of the previous request, in any other case the thread next to
  // the one specified.
  //
  virtual ErrorCode onQueryThreadList(Session &session, ProcessId pid,
                                      ThreadId lastTid, ThreadId &tid) = 0;

  virtual ErrorCode onQueryCurrentThread(Session &session,
                                         ProcessThreadId &ptid) = 0;
  virtual ErrorCode onThreadIsAlive(Session &session,
                                    ProcessThreadId const &ptid) = 0;
  virtual ErrorCode onQueryThreadInfo(Session &session,
                                      ProcessThreadId const &ptid,
                                      uint32_t mode, void *info) = 0;

  virtual ErrorCode onQueryTLSAddress(Session &session,
                                      ProcessThreadId const &ptid,
                                      Address const &offset,
                                      Address const &linkMap,
                                      Address &address) = 0;
  virtual ErrorCode onQueryTIBAddress(Session &session,
                                      ProcessThreadId const &ptid,
                                      Address &address) = 0;

  virtual ErrorCode onEnableAsynchronousProfiling(Session &session,
                                                  ProcessThreadId const &ptid,
                                                  bool enabled,
                                                  uint32_t interval,
                                                  uint32_t scanType) = 0;
  virtual ErrorCode onQueryProfileData(Session &session,
                                       ProcessThreadId const &ptid,
                                       uint32_t scanType, void *data) = 0;

  virtual ErrorCode onResume(Session &session,
                             ThreadResumeAction::Collection const &actions,
                             StopCode &stop) = 0;

  virtual ErrorCode
  onReadGeneralRegisters(Session &session, ProcessThreadId const &ptid,
                         Architecture::GPRegisterValueVector &regs) = 0;
  virtual ErrorCode
  onWriteGeneralRegisters(Session &session, ProcessThreadId const &ptid,
                          std::vector<uint64_t> const &regs) = 0;

  virtual ErrorCode onSaveRegisters(Session &session,
                                    ProcessThreadId const &ptid,
                                    uint64_t &id) = 0;
  virtual ErrorCode onRestoreRegisters(Session &session,
                                       ProcessThreadId const &ptid,
                                       uint64_t id) = 0;

  virtual ErrorCode onReadRegisterValue(Session &session,
                                        ProcessThreadId const &ptid,
                                        uint32_t regno, std::string &value) = 0;
  virtual ErrorCode onWriteRegisterValue(Session &session,
                                         ProcessThreadId const &ptid,
                                         uint32_t regno,
                                         std::string const &value) = 0;

  virtual ErrorCode onReadMemory(Session &session, Address const &address,
                                 size_t length, std::string &data) = 0;
  virtual ErrorCode onWriteMemory(Session &session, Address const &address,
                                  std::string const &data,
                                  size_t &nwritten) = 0;

  virtual ErrorCode onAllocateMemory(Session &session, size_t size,
                                     uint32_t permissions,
                                     Address &address) = 0;
  virtual ErrorCode onDeallocateMemory(Session &session,
                                       Address const &address) = 0;
  virtual ErrorCode onQueryMemoryRegionInfo(Session &session,
                                            Address const &address,
                                            MemoryRegionInfo &info) = 0;

  virtual ErrorCode onComputeCRC(Session &session, Address const &address,
                                 size_t length, uint32_t &crc) = 0;

  virtual ErrorCode onSearch(Session &session, Address const &address,
                             std::string const &pattern, Address &location) = 0;
  virtual ErrorCode onSearchBackward(Session &session, Address const &address,
                                     uint32_t pattern, uint32_t mask,
                                     Address &location) = 0;

  virtual ErrorCode onInsertBreakpoint(Session &session, BreakpointType type,
                                       Address const &address, uint32_t kind,
                                       StringCollection const &conditions,
                                       StringCollection const &commands,
                                       bool persistentCommands) = 0;
  virtual ErrorCode onRemoveBreakpoint(Session &session, BreakpointType type,
                                       Address const &address,
                                       uint32_t kind) = 0;

  virtual ErrorCode onXferRead(Session &session, std::string const &object,
                               std::string const &annex, uint64_t offset,
                               uint64_t length, std::string &buffer,
                               bool &last) = 0;
  virtual ErrorCode onXferWrite(Session &session, std::string const &object,
                                std::string const &annex, uint64_t offset,
                                std::string const &buffer,
                                size_t &nwritten) = 0;

protected: // Platform Session
  virtual ErrorCode onDisableASLR(Session &session, bool disable) = 0;

  virtual ErrorCode onSetEnvironmentVariable(Session &session,
                                             std::string const &name,
                                             std::string const &value) = 0;
  virtual ErrorCode onSetWorkingDirectory(Session &session,
                                          std::string const &path) = 0;
  virtual ErrorCode onSetStdFile(Session &session, int fileno,
                                 std::string const &path) = 0;

  virtual ErrorCode onSetArchitecture(Session &session,
                                      std::string const &architecture) = 0;

  virtual ErrorCode onSetProgramArguments(Session &session,
                                          StringCollection const &args) = 0;

  virtual ErrorCode onExecuteProgram(Session &session,
                                     std::string const &command,
                                     uint32_t timeout,
                                     std::string const &workingDirectory,
                                     ProgramResult &result) = 0;

  virtual ErrorCode onFileCreateDirectory(Session &session,
                                          std::string const &path,
                                          uint32_t mode) = 0;

  virtual ErrorCode onFileOpen(Session &session, std::string const &path,
                               uint32_t flags, uint32_t mode, int &fd) = 0;
  virtual ErrorCode onFileClose(Session &session, int fd) = 0;
  virtual ErrorCode onFileRead(Session &session, int fd, size_t count,
                               uint64_t offset, std::string &buffer) = 0;
  virtual ErrorCode onFileWrite(Session &session, int fd, uint64_t offset,
                                std::string const &buffer,
                                size_t &nwritten) = 0;

  virtual ErrorCode onFileRemove(Session &session, std::string const &path) = 0;
  virtual ErrorCode onFileReadLink(Session &session, std::string const &path,
                                   std::string &resolved) = 0;

#if 0
    //
    // more F packets:
    // https://sourceware.org/gdb/onlinedocs/gdb/List-of-Supported-Calls.html#List-of-Supported-Calls
    //
    virtual ErrorCode onGetCurrentTime(Session &session, TimeValue &tv) = 0;

    virtual ErrorCode onFileIsATTY(Session &session, int fd) = 0;
    virtual ErrorCode onFileRename(Session &session,
            std::string const &oldPath, std::string const &newPath) = 0;

    virtual ErrorCode onFileGetStat(Session &session, std::string const &path,
            FileStat &stat) = 0;
    virtual ErrorCode onFileGetStat(Session &session, int fd,
            FileStat &stat) = 0;

    virtual ErrorCode onFileSeek(Session &session, int fd,
            int64_t offset, int whence, int64_t &newOffset) = 0;
#endif

  virtual ErrorCode onFileExists(Session &session, std::string const &path) = 0;
  virtual ErrorCode onFileComputeMD5(Session &session, std::string const &path,
                                     uint8_t digest[16]) = 0;
  virtual ErrorCode onFileGetSize(Session &session, std::string const &path,
                                  uint64_t &size) = 0;

  virtual ErrorCode onQueryProcessList(Session &session,
                                       ProcessInfoMatch const &match,
                                       bool first, ProcessInfo &info) = 0;
  virtual ErrorCode onQueryProcessInfo(Session &session, ProcessId pid,
                                       ProcessInfo &info) = 0;

  virtual ErrorCode onLaunchDebugServer(Session &session,
                                        std::string const &host, uint16_t &port,
                                        ProcessId &pid) = 0;

  virtual ErrorCode onQueryLaunchSuccess(Session &session, ProcessId pid) = 0;

  virtual ErrorCode onQueryUserName(Session &session, UserId const &uid,
                                    std::string &name) = 0;
  virtual ErrorCode onQueryGroupName(Session &session, GroupId const &gid,
                                     std::string &name) = 0;
  virtual ErrorCode onQueryWorkingDirectory(Session &session,
                                            std::string &workingDir) = 0;

protected: // System Session
  virtual void onReset(Session &session) = 0;
  virtual ErrorCode onFlashErase(Session &session, Address const &address,
                                 size_t length) = 0;
  virtual ErrorCode onFlashWrite(Session &session, Address const &address,
                                 std::string const &data) = 0;
  virtual ErrorCode onFlashDone(Session &session) = 0;
};
}
}

#endif // !__DebugServer2_GDBRemote_SessionDelegate_h
