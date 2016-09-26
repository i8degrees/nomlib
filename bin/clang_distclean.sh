#!/bin/bash

# NOTE: This script is intended to be ran from the project's current build
# directory.

# Enable case-insensitive string matches for passing arguments around
shopt -s nocasematch

WORKING_DIR=$(pwd)

function usage_info()
{
  SCRIPT_NAME = $(basename $0)
  echo "Usage: ${SCRIPT_NAME} <build_type> <build_dir>\n"
  echo "...where <build_type> is one of Debug or Release"
  echo "    (Defaults: ${BUILD_TYPE})\n"
  echo
  echo "...where <build_dir> is the engine's source build tree"
  echo "    (Defaults: ${BUILD_DIR}) -- this flag is not yet implemented."
}

# Default build configuration type
BUILD_TYPE_ARG=$1
BUILD_TYPE="Debug"
BUILD_DIR=${WORKING_DIR}

if [[ -n "${BUILD_TYPE_ARG}" ]]; then
  BUILD_TYPE=${BUILD_TYPE_ARG}
fi

# The absolute path to CMake's local cache of project build variables
CMAKE_CACHE_FILE="$(pwd)/CMakeCache.txt"
CMAKE_INSTALL_RECEIPT="$(pwd)/install_manifest.txt"

# Default installation path
BUILD_INSTALL_DIR="${HOME}/Library/Frameworks"
BUILD_INSTALL_DIR_ARG=$3

# Check command arguments
if [[ $1 == "-h" || $1 == "--help" ]]; then
  usage_info
  exit 0
else
  # NOTE(jeff): If the end-user has given an alternative installation prefix,
  # go ahead and try honoring the request by updating the default install
  # prefix used by CMake.
  if [[ -n "${BUILD_INSTALL_DIR_ARG}" ]]; then
    BUILD_INSTALL_DIR=${3}
  fi

  echo "\n...Cleaning up development build environment..."
  echo "  BUILD_INSTALL_DIR: ${BUILD_INSTALL_DIR}"

  make uninstall
  make clean

  echo "\nClearing CMake cache..."

  if [[ -f "${CMAKE_CACHE_FILE}" ]]; then
    rm -rf CMakeCache.txt
  fi

  if [[ -d "CMakeFiles" ]]; then
    rm -rf CMakeFiles
  fi

  if [[ -d "${CMAKE_INSTALL_RECEIPT}" ]]; then
    rm -rf CMakeFiles
  fi

fi
