//
// Copyright (c) 2014, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#include "DebugServer2/Support/Stringify.h"
#include "DebugServer2/Utils/Log.h"

namespace ds2 {
namespace Support {
namespace Windows {

#define DO_STRINGIFY(VALUE)                                                    \
  case VALUE:                                                                  \
    return #VALUE;

#define DO_DEFAULT(MESSAGE, VALUE)                                             \
  default:                                                                     \
    if (dieOnFail)                                                             \
      DS2BUG(MESSAGE ": %#x", VALUE);                                          \
    else                                                                       \
      return nullptr;

char const *Stringify::ExceptionCode(DWORD code, bool dieOnFail) {
  switch (code) {
    DO_STRINGIFY(EXCEPTION_ACCESS_VIOLATION)
    DO_STRINGIFY(EXCEPTION_ARRAY_BOUNDS_EXCEEDED)
    DO_STRINGIFY(EXCEPTION_BREAKPOINT)
    DO_STRINGIFY(EXCEPTION_DATATYPE_MISALIGNMENT)
    DO_STRINGIFY(EXCEPTION_FLT_DENORMAL_OPERAND)
    DO_STRINGIFY(EXCEPTION_FLT_DIVIDE_BY_ZERO)
    DO_STRINGIFY(EXCEPTION_FLT_INEXACT_RESULT)
    DO_STRINGIFY(EXCEPTION_FLT_INVALID_OPERATION)
    DO_STRINGIFY(EXCEPTION_FLT_OVERFLOW)
    DO_STRINGIFY(EXCEPTION_FLT_STACK_CHECK)
    DO_STRINGIFY(EXCEPTION_FLT_UNDERFLOW)
    DO_STRINGIFY(EXCEPTION_ILLEGAL_INSTRUCTION)
    DO_STRINGIFY(EXCEPTION_IN_PAGE_ERROR)
    DO_STRINGIFY(EXCEPTION_INT_DIVIDE_BY_ZERO)
    DO_STRINGIFY(EXCEPTION_INT_OVERFLOW)
    DO_STRINGIFY(EXCEPTION_INVALID_DISPOSITION)
    DO_STRINGIFY(EXCEPTION_NONCONTINUABLE_EXCEPTION)
    DO_STRINGIFY(EXCEPTION_PRIV_INSTRUCTION)
    DO_STRINGIFY(EXCEPTION_SINGLE_STEP)
    DO_STRINGIFY(EXCEPTION_STACK_OVERFLOW)
    DO_DEFAULT("unknown exception code", code);
  }
}
}
}
}
