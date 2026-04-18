#!/bin/bash

TMPDIR=$1
[ -z $TMPDIR ] && TMPDIR=/tmp/nom

mkdir -p $TMPDIR && cd $TMPDIR
LOG_DIR=$TMPDIR/logs
export LOG_DIR
SRC=/mnt/fs1/Projects/nomlib.git
mkdir -p $LOG_DIR
cmake -S $SRC -B $TMPDIR -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DDEBUG=on -DDEBUG_ASSERT=on -DEXAMPLES=on -DNOM_BUILD_TESTS=on -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DGTEST_ROOT=/mnt/fs1/Projects/nomlib.git/third-party/linux/gtest -DGTEST_INCLUDE_DIR=/mnt/fs1/Projects/nomlib.git/third-party/linux/gtest/include -DGTEST_LIBRARY=/mnt/fs1/Projects/nomlib.git/third-party/linux/gtest/lib/libgtest.a
