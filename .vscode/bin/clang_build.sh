#!/bin/sh

# NOTE: This script is intended to be ran from the project's current build
# directory.

BUILD_BIN=$(which make)

# Default build configuration type
BUILD_TYPE="Debug"

if [[ !( -z "${BUILD_TYPE_ARG}") ]]; then
  BUILD_TYPE=$1
fi

if [[ !( -z "${NUM_THREADS}") ]]; then
  NUM_THREADS_ARG="-j ${NUM_THREADS}"
fi
# echo "NUM_THREADS_ARG: ${NUM_THREADS_ARG}"

../bin/check_json.sh

if [[ $? -eq 0 ]]; then
  echo "Building ${BUILD_TYPE} project... [target: build]"

  ${BUILD_BIN} ${NUM_THREADS_ARG}
else
  echo "${0} ERROR: JSON validation failure has prevented building the project."
  exit 1
fi
