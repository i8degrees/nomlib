#!/bin/sh

# NOTE: This script is intended to be ran from the project's current build
# directory.

# Enable case-insensitive string matches for passing arguments around
shopt -s nocasematch

function usage_info()
{
  SCRIPT_NAME = $(basename $0)
  echo "Usage: ${SCRIPT_NAME} <build_type> <install_dir>\n"
  echo "...where <build_type> is one of Debug or Release"
  echo "    (Defaults: ${BUILD_TYPE})\n"
  echo "...where <install_dir> is the engine's destination install prefix"
  echo "    (Defaults: ${BUILD_INSTALL_DIR})"
  exit 0
}

MKDIR_BIN=$(which mkdir) # GNU coreutils
RM_BIN=$(which rm) # GNU coreutils
CMAKE_BIN=$(which cmake)

# Default build configuration type
BUILD_TYPE_ARG=$1
GEN_PROJECT_TYPE_ARG=$2
BUILD_TYPE="Debug"
GEN_PROJECT_TYPE="" # i.e. CMake project generator: Xcode

if [[ !( -z "${BUILD_TYPE_ARG}") ]]; then
  BUILD_TYPE=${BUILD_TYPE_ARG}
fi

if [[ !( -z "${GEN_PROJECT_TYPE_ARG}") ]]; then
  GEN_PROJECT_TYPE="-G${GEN_PROJECT_TYPE_ARG}"
fi

# The absolute path to CMake's local cache of project build variables
CMAKE_CACHE_FILE="$(pwd)/CMakeCache.txt"

# Sane engine defaults for building apps against
#
# IMPORTANT: A space character **must** be inserted at the beginning of the
# BUILD_FLAGS string variable, or else the preprocessor flags will not be
# parsed correctly by cmake
BUILD_FLAGS+=" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.7"
if [ -z "${NOMLIB_DEPS_ROOT}" ]; then
  BUILD_FLAGS+=" -DNOMLIB_DEPS_PREFIX=${HOME}/Projects/nomlib.git/third-party/osx"
else
  BUILD_FLAGS+=" -DNOMLIB_DEPS_PREFIX=${NOMLIB_DEPS_ROOT}"
fi

# Default installation path
BUILD_INSTALL_DIR="$(pwd)"
BUILD_INSTALL_DIR_ARG=$3

# Check command arguments
if [[ $1 == "-h" || $1 == "--help" ]]; then
  usage_info
else
  # Set build configuration type
  #
  # IMPORTANT: A space character **must** be inserted at the beginning of the
  # BUILD_FLAGS string variable, or else the preprocessor flags will not be
  # parsed correctly by cmake
  if [[ ${BUILD_TYPE_ARG} == "Debug" ]]; then
    BUILD_FLAGS+=" -DDEBUG=on -DDEBUG_ASSERT=on"
  elif [[ ${BUILD_TYPE_ARG} == "Release" ]]; then
    BUILD_TYPE="Release"
    BUILD_FLAGS+=" -DDEBUG=off -DDEBUG_ASSERT=off"
  fi

  echo "BUILD_FLAGS=${BUILD_FLAGS}"

  if [[ !( -z "${BUILD_INSTALL_DIR_ARG}" ) ]]; then
    # Override installation prefix path; it is best to let CMake deal with
    # file path validation
    BUILD_INSTALL_DIR=${3}
  fi

  echo "\nClearing CMake cache..."

  if [[ -f "CMakeCache.txt" ]]; then
    ${RM_BIN} -rf CMakeCache.txt
  fi

  if [[ -d "CMakeFiles" ]]; then
    ${RM_BIN} -rf CMakeFiles
  fi

  echo "Generating ${BUILD_TYPE} project files..."
  echo "BUILD_INSTALL_DIR: ${BUILD_INSTALL_DIR}"

  ${CMAKE_BIN} "$GEN_PROJECT_TYPE" ${BUILD_FLAGS} \
  -DCMAKE_INSTALL_PREFIX=${BUILD_INSTALL_DIR} \
  ..
fi
