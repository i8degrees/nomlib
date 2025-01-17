#!/bin/sh

# NOTE: This script is intended to be ran from the project's current build
# directory.

BUILD_BIN=$(which make)
BUILD_TYPE_ARG=$1

# Default
BUILD_TYPE="Debug"

if [[ !( -z "${BUILD_TYPE_ARG}") ]]; then
  BUILD_TYPE=$1
fi

if [[ !( -z "${NUM_THREADS}") ]]; then
  NUM_THREADS_ARG="-j ${NUM_THREADS}"
fi
# echo "NUM_THREADS_ARG: ${NUM_THREADS_ARG}"

echo "Installing ${BUILD_TYPE} project... [target: build]"
${BUILD_BIN} install
