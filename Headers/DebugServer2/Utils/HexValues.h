//
// Copyright (c) 2014-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#ifndef __DebugServer2_Utils_HexValues_h
#define __DebugServer2_Utils_HexValues_h

#include "DebugServer2/Utils/CompilerSupport.h"
#include "DebugServer2/Utils/Log.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace ds2 {

static inline char NibbleToHex(uint8_t byte) {
  return "0123456789abcdef"[byte & 0xf];
}

static inline uint8_t HexToNibble(char ch) {
  if (ch >= '0' && ch <= '9')
    return static_cast<uint8_t>(ch - '0');
  else if (ch >= 'a' && ch <= 'f')
    return static_cast<uint8_t>(ch - 'a' + 10);
  else if (ch >= 'A' && ch <= 'F')
    return static_cast<uint8_t>(ch - 'A' + 10);
  DS2_UNREACHABLE();
}

static inline uint8_t HexToByte(char const *chars) {
  return static_cast<uint8_t>((HexToNibble(chars[0]) << 4) | HexToNibble(chars[1]));
}

static inline std::string StringToHex(std::string const &str) {
  std::string result;
  for (char n : str) {
    result += NibbleToHex(static_cast<uint8_t>(n >> 4));
    result += NibbleToHex(static_cast<uint8_t>(n & 15));
  }
  return result;
}

static inline std::string HexToString(std::string const &str) {
  std::string result;
  DS2ASSERT(str.size() % 2 == 0);
  for (size_t n = 0; n < str.size(); n += 2) {
    result +=
        static_cast<char>((HexToNibble(str[n]) << 4) | HexToNibble(str[n + 1]));
  }
  return result;
}
}

#endif // !__DebugServer2_Utils_HexValues_h
