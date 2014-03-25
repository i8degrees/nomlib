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

EXCLUSION_MASKS="*.DS_Store*"

if [[ $1 == "osx" ]]; then
  DEPS_FILENAME="./nomlib_osx-dependencies.tar.gz"
  INCLUSION_MASKS="osx/"

  ${TAR_BIN} ${TAR_ARGS} ${DEPS_FILENAME} --exclude=${EXCLUSION_MASKS} ${INCLUSION_MASKS}
elif [[ $1 == "windows" ]]; then
  DEPS_FILENAME="./nomlib_windows-dependencies.zip"
  INCLUSION_MASKS="windows/"

  ${ZIP_BIN} ${ZIP_ARGS} ${DEPS_FILENAME} ${INCLUSION_MASKS} -x ${EXCLUSION_MASKS}
#elif [[ $1 == "linux" ]]; then
  # TODO: Handle Linux platform case
#elif [[ $1 == "ios" ]]; then
  # TODO: Handle iOS platform case
else
  echo "Usage: ./build-deps.sh [osx|windows]"
  #echo "Usage: ./build-deps.sh [osx|windows|linux|ios]"
fi
