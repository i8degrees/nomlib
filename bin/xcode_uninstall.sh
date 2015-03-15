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

# Ensure that we do not accidentally lose anything by jumping the gun too
# early! ;-)
if [[ $? -eq 0 ]]; then
  echo "Uninstalling ${BUILD_TYPE} project... [target: uninstall]"
  ${XCODEBUILD_BIN} -configuration ${BUILD_TYPE} -target uninstall
else
  exit -1
fi
