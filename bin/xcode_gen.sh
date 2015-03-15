#!/bin/bash

# NOTE: This script is intended to be ran from the project's current build
# directory.

# xcode_gen.sh <build_type> <install_dir>
#     ...where...
# <build_type> is Debug or Release
# (defaults to Debug)
#
# <install_dir> is an absolute path for installation of libraries
# (defaults to ~/Library/Frameworks)

# Enable case-insensitive string matches
shopt -s nocasematch

MKDIR_BIN=$(which mkdir) # GNU coreutils v8.23
CMAKE_BIN=$(which cmake)

BUILD_TYPE_ARG=$1
BUILD_INSTALL_DIR_ARG=$2

# Defaults for generating project files for a Debug build
BUILD_TYPE="Debug"
BUILD_FLAGS="-DDEBUG=on -DDEBUG_ASSERT=on"

# Default installation path
BUILD_INSTALL_DIR="${HOME}/Library/Frameworks"

if [[ $BUILD_TYPE_ARG == "Release" ]]; then
  BUILD_TYPE="Release"
  BUILD_FLAGS=""
fi

if [[ !( -z "$BUILD_INSTALL_DIR_ARG" ) ]]; then
  # Custom installation dir; let CMake worry about validation...
  BUILD_INSTALL_DIR=$BUILD_INSTALL_DIR_ARG
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
