#!/bin/sh

# NOTE: This script is intended to be ran from the project's current build
# directory.

XCODEBUILD_BIN=$(which xcodebuild)
BUILD_TYPE_ARG=$1

# Default
BUILD_TYPE="Debug"

if [[ !( -z "${BUILD_TYPE_ARG}") ]]; then
  BUILD_TYPE=$1
fi

if [[ !( -z "${NUM_THREADS}") ]]; then
  NUM_THREADS_ARG="-jobs ${NUM_THREADS}"
fi
echo "NUM_THREADS_ARG: ${NUM_THREADS_ARG}"

# Try to not waste time by trying to install an incomplete build
if [[ $? -eq 0 ]]; then
  echo "Installing ${BUILD_TYPE} project... [target: install]"
  ${XCODEBUILD_BIN} ${NUM_THREADS_ARG} -configuration ${BUILD_TYPE} \
  -target install
else
  echo "Previous build command failed; **NOT** installing ${BUILD_TYPE} project!"
  echo "EXIT_CODE: $?"
  exit $?
fi
