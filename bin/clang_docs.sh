#!/bin/sh

# NOTE: This script is intended to be ran from the project's current build
# directory.

# We rely on the outside shell environment to set the system PATH to the
# appropriate locations for the development tooling, i.e.: make
# PATH=/usr/bin:/usr/local/bin

BUILD_TYPE_ARG=$1

# Default
BUILD_TYPE="Debug"

if [[ !( -z "${BUILD_TYPE_ARG}") ]]; then
  BUILD_TYPE=$1
fi

echo "Building ${BUILD_TYPE} project... [target: docs]"
make docs

# Optionally setup local site access for the generated documentation set at
# http://nomlib-docs.dev -- requires an existing Pow installation [1].
#
# 1. http://pow.cx/
if [[ $(which pow) ]]; then
  ../bin/gen_docs.sh nomlib-docs

  # NOTE: See Auto-Reload plugin for auto-reload of API documentation page
fi
