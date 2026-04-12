#!/bin/bash
#
#

[ -n "$DEBUG_TRACE" ] && set -o xtrace

# old code
if [ -n "$INIT_BUILD" ]; then
  cd /home/jeff/tmp/nomlib/
  git clone "https://github.com/google/googletest.git"
  cd googletest
  git checkout v1.8.x
  mkdir build && cd build
  cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ ..
  # copy header files and library files to dist
fi

# new code
SRC_DIR=/mnt/fs1/Projects/nomlib.git.bck/vendor/gtest.git
VENDOR_DEST_DIR=/mnt/fs1/Projects/nomlib.git.bck/third-party/linux/gtest
test -r "$VENDOR_DEST_DIR" || echo -e "CRITICAL: The path at $VENDOR_DEST_DIR does not exist!\n"; exit 1

#git clone "https://github.com/google/googletest.git"
#cd googletest
#git checkout v1.8.x
pushd "$SRC_DIR" || exit 2
mkdir build
if [ -e "./build" ]; then
  # TODO(JEFF): Fresh build upon X flag
else
  cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
    -DCMAKE_INSTALL_PREFIX=$VENDOR_DEST_DIR ..
    # clang is NOT supported
    #-DCMAKE_C_COMPILER=clang \
    #-DCMAKE_CXX_COMPILER=clang++
fi

# TODO(JEFF): Append cxx_base_flags from here via cmake build cmd above
# /mnt/fs1/Projects/nomlib.git.bck/third-party/linux/gtest/googletest/cmake/internal_utils.cmake:105

