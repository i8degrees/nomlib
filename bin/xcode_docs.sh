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

echo "Building ${BUILD_TYPE} project... [target: docs]"
${XCODEBUILD_BIN} -configuration ${BUILD_TYPE} -target docs

# Optionally setup local site access for the generated documentation set at
# http://nomlib-docs.dev -- requires an existing Pow installation [1].
#
# 1. http://pow.cx/
if [[ $(which pow) ]]; then
  ../bin/gen_docs.sh nomlib-docs

  # TODO: Finish integration of this script! i.e.: If we could try to remember
  # the last jump URL before reloading the page(s), it would be perfect!
  /usr/bin/osascript ${HOME}/Projects/dotfiles.git/AppleScript/chrome_reload.applescript
fi
