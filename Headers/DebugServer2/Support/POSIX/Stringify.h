//
// Copyright (c) 2014-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#ifndef __DebugServer2_Support_POSIX_Stringify_h
#define __DebugServer2_Support_POSIX_Stringify_h

#ifndef __DebugServer2_Support_Stringify_h
#error "You shall not include this file directly."
#endif

namespace ds2 {
namespace Support {
namespace POSIX {

class Stringify {
public:
  static char const *Signal(int signal);
  static char const *SignalCode(int signal, int code);
  static char const *Errno(int error);
  static char const *Ptrace(int code);
};
}
}
}

#endif // !__DebugServer2_Support_POSIX_Stringify_h
