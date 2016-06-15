//
// Copyright (c) 2014-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#include "DebugServer2/Host/File.h"
#include "DebugServer2/Host/Platform.h"
#include "DebugServer2/Host/Windows/ExtraWrappers.h"

namespace ds2 {
namespace Host {

File::File(std::string const &path, uint32_t flags, uint32_t mode)
    : _fd(-1), _lastError(kErrorUnsupported) {}

File::~File() = default;
}
}
