//
// Copyright (c) 2014-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#pragma once

#include "DebugServer2/Core/BreakpointManager.h"

namespace ds2 {
class SoftwareBreakpointManager : public BreakpointManager {
private:
  std::map<uint64_t, ByteVector> _insns;

public:
  SoftwareBreakpointManager(Target::ProcessBase *process);
  ~SoftwareBreakpointManager() override;

public:
  virtual void clear() override;

public:
  virtual int hit(Target::Thread *thread, Site &site) override;
  void afterResume();

protected:
  virtual void getOpcode(uint32_t type, ByteVector &opcode) const;

protected:
  virtual ErrorCode enableLocation(Site const &site,
                                   Target::Thread *thread = nullptr) override;
  virtual ErrorCode disableLocation(Site const &site,
                                    Target::Thread *thread = nullptr) override;

protected:
  ErrorCode isValid(Address const &address, size_t size,
                    Mode mode) const override;

public:
  void insertStashedInsns(Address const &start, size_t length,
                          ByteVector &data);

#if defined(ARCH_ARM) || defined(ARCH_ARM64)
public:
  virtual void
  enumerate(std::function<void(Site const &)> const &cb) const override;

public:
  virtual ErrorCode add(Address const &address, Type type, size_t size,
                        Mode mode) override;
  virtual ErrorCode remove(Address const &address) override;

public:
  virtual bool has(Address const &address) const override;
#endif

public:
  virtual bool fillStopInfo(Target::Thread *thread,
                            StopInfo &stopInfo) override;
};
} // namespace ds2
