#!/usr/bin/env bash
##
## Copyright (c) 2014-present, Facebook, Inc.
## All rights reserved.
##
## This source code is licensed under the University of Illinois/NCSA Open
## Source License found in the LICENSE file in the root directory of this
## source tree. An additional grant of patent rights can be found in the
## PATENTS file in the same directory.
##

set -eu

if [[ "${TARGET-}" == Darwin-* ]]; then
  exit 0
fi

if [[ "${COVERAGE-}" = "1" ]]; then
  top="$(git rev-parse --show-toplevel)"
  PATH=$PATH:"~/.local/bin" coveralls --exclude lib --exclude include --exclude sys --exclude lldb --exclude deps -r "$top" -b "$top/build"
fi
