#!/bin/sh

# NOTE: This script is intended to be ran from the project's current build
# directory.

# Enable case-insensitive string matches for passing arguments around
shopt -s nocasematch

MKDIR_BIN=$(which mkdir) # GNU coreutils
RM_BIN=$(which rm) # GNU coreutils
CMAKE_BIN=$(which cmake)

BUILD_TYPE_ARG=$1
BUILD_INSTALL_DIR_ARG=$2

# The absolute path to CMake's local cache of project build variables
CMAKE_CACHE_FILE="$(pwd)/CMakeCache.txt"

# Defaults for generating project files for a Debug build
BUILD_TYPE="Debug"
BUILD_FLAGS="-DDEBUG=on -DDEBUG_ASSERT=on"

# Default installation path
BUILD_INSTALL_DIR="${HOME}/Library/Frameworks"

function usage_info()
{
  echo "Usage: $(basename $0) <build_type> <install_dir>\n"
  echo "...where <build_type> is either Debug or Release"
  echo "    (Defaults: ${BUILD_TYPE})\n"
  echo "...where <install_dir> is the installation prefix for the engine "
  echo "libraries and header files"
  echo "    (Defaults: ${BUILD_INSTALL_DIR})"
  exit 0
}

if [[ $1 == "-h" || $1 == "--help" ]]; then
  usage_info
else

  if [[ ${BUILD_TYPE_ARG} == "Release" ]]; then
    BUILD_TYPE="Release"
    BUILD_FLAGS=""
  fi

  if [[ !( -z "${BUILD_INSTALL_DIR_ARG}" ) ]]; then
    # Custom installation dir; let CMake worry about the file path validation!
    BUILD_INSTALL_DIR=${BUILD_INSTALL_DIR_ARG}
  fi

  if [[ -f "${CMAKE_CACHE_FILE}" ]]; then
    ${RM_BIN} ${CMAKE_CACHE_FILE}
  fi

  echo "Generating ${BUILD_TYPE} project files..."
  echo "BUILD_FLAGS: ${BUILD_FLAGS}"
  echo "BUILD_INSTALL_DIR: ${BUILD_INSTALL_DIR}"

  # I have no idea **why**, but my Sublime Text build script fails unless I
  # create this directory beforehand ...
  ${MKDIR_BIN} -p CMakeFiles/ALL_BUILD

  ${CMAKE_BIN} -GXcode -DCMAKE_INSTALL_PREFIX=${BUILD_INSTALL_DIR} \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=10.7 ${BUILD_FLAGS} \
  -DEXAMPLES=on -DNOM_BUILD_TESTS=on ..
fi
