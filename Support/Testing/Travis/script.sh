#!/usr/bin/env bash
##
## Copyright (c) 2014, Facebook, Inc.
## All rights reserved.
##
## This source code is licensed under the University of Illinois/NCSA Open
## Source License found in the LICENSE file in the root directory of this
## source tree. An additional grant of patent rights can be found in the
## PATENTS file in the same directory.
##

set -eu

cd "$(git rev-parse --show-toplevel)"

if [[ "$TARGET" = "Style" ]]; then
  CLANG_FORMAT=clang-format-3.6 "./Support/Scripts/check-style.sh" {Sources,Headers,Main}
  exit
fi

mkdir build && cd build

if [[ "${CLANG-}" = "1" ]]; then
  cmake_options=(-DCMAKE_TOOLCHAIN_FILE="../Support/CMake/Toolchain-${TARGET}-Clang.cmake")
else
  cmake_options=(-DCMAKE_TOOLCHAIN_FILE="../Support/CMake/Toolchain-${TARGET}.cmake")
fi

if [[ "${REGSGEN-}" = "1" ]]; then
  cmake_options+=(-DDS2_ENABLE_REGSGEN2="1")
fi

if [[ "${RELEASE-}" = "1" ]]; then
  cmake_options+=(-DCMAKE_BUILD_TYPE="Release")
fi

cmake "${cmake_options[@]}" ..
make

if [[ -n "${LLGS_TESTS-}" ]]; then
  ../Support/Scripts/run-llgs-tests.sh
fi
