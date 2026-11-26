#!/bin/sh
#
# Bootstrap build env for nomlib
#
# shellcheck shell=bash
#
# SEE ALSO
# 1. README.md
# 2. third-party/README.md
#

if [ ! -x "$(which git)" ]; then
  echo "CRITICAL: git must be installed before running this script."
  echo
  exit 1 # EPERM
fi

echo "INFO: Bootstrapping nomlib build environment..."
echo

# ensures that vendor/vcpkg is setup
git submodule update --init && git submodule sync

if vendor/vcpkg/bootstrap-vcpkg.sh; then
  if [ ! -x "vendor/vcpkg/vcpkg" ]; then
    echo "CRITICAL: Failed to find vcpkg binary."
    echo
    exit 3 # ESRCH
  fi
else
  echo "CRITICAL: vendor/vcpkg/bootstrap-vcpkg.sh failed to exit cleanly."
  echo
  exit 2 # ENOENT
fi

if [ ! "$(vendor/vcpkg/vcpkg version)" ]; then
  echo "ERROR: Failed to print vcpkg version."
  echo
  exit 255 # ???
fi

echo "INFO: The development build environment has been setup. Proceed with CMake configure, build and install."
echo

#cmake -S . --preset release -DCMAKE_INSTALL_PREFIX=$HOME/local
#cmake --build build/release --target all
#cmake --install build/release
exit 0

