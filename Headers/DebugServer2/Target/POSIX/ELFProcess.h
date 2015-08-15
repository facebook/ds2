//
// Copyright (c) 2014, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#ifndef __DebugServer2_Target_POSIX_ELFProcess_h
#define __DebugServer2_Target_POSIX_ELFProcess_h

#include "DebugServer2/Target/POSIX/Process.h"
#include "DebugServer2/Support/POSIX/ELFSupport.h"

namespace ds2 {
namespace Target {
namespace POSIX {

class ELFProcess : public Process {
protected:
  std::string _auxiliaryVector;
  Address _sharedLibraryInfoAddress;

public:
  virtual ErrorCode getAuxiliaryVector(std::string &auxv);
  virtual uint64_t getAuxiliaryVectorValue(uint64_t type);

public:
  virtual ErrorCode getSharedLibraryInfoAddress(Address &address);
  virtual ErrorCode enumerateSharedLibraries(
      std::function<void(SharedLibraryInfo const &)> const &cb);

public:
  virtual ErrorCode enumerateAuxiliaryVector(std::function<
      void(Support::ELFSupport::AuxiliaryVectorEntry const &)> const &cb);

public:
  virtual bool isELFProcess() const;

protected:
  virtual ErrorCode updateInfo();
  virtual ErrorCode updateAuxiliaryVector();
};
}
}
}

#endif // !__DebugServer2_Target_POSIX_Process_h
