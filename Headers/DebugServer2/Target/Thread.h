//
// Copyright (c) 2014, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#ifndef __DebugServer2_Target_Thread_h
#define __DebugServer2_Target_Thread_h

#include "DebugServer2/Base.h"

#if defined(OS_LINUX)
#include "DebugServer2/Target/Linux/Thread.h"
#elif defined(OS_WIN32)
#include "DebugServer2/Target/Windows/Thread.h"
#elif defined(__FreeBSD__)
#include "DebugServer2/Target/FreeBSD/Thread.h"
#else
#error "Target not supported."
#endif

namespace ds2 {
namespace Target {

#if defined(OS_LINUX)
using Linux::Thread;
#elif defined(OS_WIN32)
using Windows::Thread;
#elif defined(__FreeBSD__)
using FreeBSD::Thread;
#else
#error "Target not supported."
#endif
}
}

#endif // !__DebugServer2_Target_Thread_h
