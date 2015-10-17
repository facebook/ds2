//
// Copyright (c) 2014, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#ifndef __DebugServer2_Constants_h
#define __DebugServer2_Constants_h

namespace ds2 {

//
// Error Codes as defined by GDB remoting documentation,
// plus some others.
//

enum ErrorCode {
  kSuccess,
  kErrorNoPermission = 1,
  kErrorNotFound = 2,
  kErrorProcessNotFound = 3,
  kErrorInterrupted = 4,
  kErrorInvalidHandle = 9,
  kErrorNoMemory = 12,
  kErrorAccessDenied = 13,
  kErrorInvalidAddress = 14,
  kErrorBusy = 16,
  kErrorAlreadyExist = 17,
  kErrorNoDevice = 19,
  kErrorNotDirectory = 20,
  kErrorIsDirectory = 21,
  kErrorInvalidArgument = 22,
  kErrorTooManySystemFiles = 23,
  kErrorTooManyFiles = 24,
  kErrorFileTooBig = 27,
  kErrorNoSpace = 28,
  kErrorInvalidSeek = 29,
  kErrorNotWriteable = 30,
  kErrorNameTooLong = 91,
  kErrorUnknown = 9999,
  kErrorUnsupported = 10000
};

//
// Endian
//

enum Endian {
  kEndianUnknown = 0,
  kEndianBig = 1,
  kEndianLittle = 2,
  kEndianPDP = 3,
#if defined(ENDIAN_BIG)
  kEndianNative = kEndianBig,
#elif defined(ENDIAN_LITTLE)
  kEndianNative = kEndianLittle,
#elif defined(ENDIAN_MIDDLE)
  kEndianNative = kEndianPDP,
#else
  kEndianNative = kEndianUnknown,
#endif
};

//
// Memory Protection
//

enum /*Protection*/ {
  kProtectionNone = 0,
  kProtectionExecute = (1 << 0),
  kProtectionWrite = (1 << 1),
  kProtectionRead = (1 << 2)
};

char const *GetErrorCodeString(ErrorCode err);
}

#endif // !__DebugServer2_Constants_h
