#!/bin/sh

# Third-Party dependency packer
#
# This is a replacement for nomdev's archive feature (said feature is incomplete
# and should be considered broken).
#
# NOTE:
#
# This script should be ran from the same directory it was found in.
#
# Prerequisites:
#
# tar, zip, bash
#
# See also: nomdev.git/src/ExternalDeps.rb,
#           https://sf.net/p/nomlib,
#           https://sf.net/p/ttcards
#

TAR_BIN=$(which tar)
TAR_ARGS="-czvf"

ZIP_BIN=$(which zip)
ZIP_ARGS="-r"

# Exclusion masks for zip & tar
EXCLUSION_MASKS="*.DS_Store*"

PROJECT_NAME="nomlib"

# Date binary for prefixing the created timestamp onto resulting archive
DATE_BIN=$(which date)

TIMESTAMP="$($DATE_BIN +%Y-%m-%d)" # BSD date(1)

function usage_info()
{
  echo "Usage: ./$0 [osx|ios|linux|windows|all]"
}

function osx_deps()
{
  DEPS_FILENAME="./${TIMESTAMP}_${PROJECT_NAME}_osx-dependencies.tar.gz"
  INCLUSION_MASKS="osx/ common/"

  ${TAR_BIN} ${TAR_ARGS} ${DEPS_FILENAME} --exclude=${EXCLUSION_MASKS} ${INCLUSION_MASKS}
}

function ios_deps()
{
  DEPS_FILENAME="./${TIMESTAMP}_${PROJECT_NAME}_ios-dependencies.tar.gz"
  INCLUSION_MASKS="ios/ common/"

  ${TAR_BIN} ${TAR_ARGS} ${DEPS_FILENAME} --exclude=${EXCLUSION_MASKS} ${INCLUSION_MASKS}
}

function windows_deps()
{
  DEPS_FILENAME="./${TIMESTAMP}_${PROJECT_NAME}_windows-dependencies.zip"
  INCLUSION_MASKS="windows/ common/"

  ${ZIP_BIN} ${ZIP_ARGS} ${DEPS_FILENAME} ${INCLUSION_MASKS} -x ${EXCLUSION_MASKS}
}

function linux_deps()
{
  echo "TODO"
}

function all_deps()
{
  osx_deps
  ios_deps
  linux_deps
  windows_deps
}

if [[ "$1" == "osx" ]]; then
  osx_deps
elif [[ $1 == "ios" ]]; then
  ios_deps
elif [[ $1 == "linux" ]]; then
  linux_deps
elif [[ $1 == "windows" ]]; then
  windows_deps
elif [[ $1 == "ios" ]]; then
  ios_deps
elif [[ $1 == "all" ]]; then
  all_deps
elif [[ $1 == "?" ]]; then
  usage_info
else
  usage_info
fi

