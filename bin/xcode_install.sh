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

# Ensure that we do not waste time by trying to install an incomplete build
if [[ $? -eq 0 ]]; then
  echo "Installing ${BUILD_TYPE} project... [target: install]"
  ${XCODEBUILD_BIN} -configuration ${BUILD_TYPE} -target install
else
  exit -1
fi
