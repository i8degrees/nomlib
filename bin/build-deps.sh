#!/usr/bin/env bash

# Third-Party dependency packer
#
# This is a replacement for nomdev's archive feature (said feature is incomplete
# and should be considered broken).
#
# NOTE:
#
# This script should always be ran from nomlib's root path, i.e.:
#
#   $ ~/Projects/nomlib.git/bin/build-deps.sh
#
# Prerequisites:
#
# tar, zip, bash
#
# See also: nomdev.git/src/ExternalDeps.rb,
#           https://sf.net/p/nomlib,
#           https://sf.net/p/ttcards
#

TAR_BIN="$(command -v tar)"
TAR_ARGS="-czvf"

ZIP_BIN="$(command -v zip)"
ZIP_ARGS="-r"

# Exclusion masks for zip & tar
EXCLUSION_MASKS="*.DS_Store*"

PROJECT_NAME="nomlib"
DEPS_DIR="third-party"

# Date binary for prefixing the created timestamp onto resulting archive
DATE_BIN="$(command -v date)"

TIMESTAMP="$($DATE_BIN +%Y-%m-%d)" # BSD date(1)

# Optional git rev number
GIT_BIN="$(command -v git)"
# GIT_VER=$( ${GIT_BIN} rev-parse HEAD) # Full SHA
GIT_VER="$( ${GIT_BIN} rev-parse --short HEAD)"

function usage_info()
{
  echo "Usage: ./$0 [osx|ios|linux|windows|all]"
}

function osx_deps()
{
  if [[ ${GIT_BIN} ]]; then
    # Put the resulting output file at project's root dir
    DEPS_FILENAME="../${TIMESTAMP}_${PROJECT_NAME}-${GIT_VER}_osx-dependencies.tar.gz"
  else
    # Put the resulting output file at project's root dir
    DEPS_FILENAME="../${TIMESTAMP}_${PROJECT_NAME}_osx-dependencies.tar.gz"
  fi

  INCLUSION_MASKS="osx/ common/ README.md"

  "${TAR_BIN} ${TAR_ARGS} ${DEPS_FILENAME} --exclude=${EXCLUSION_MASKS} ${INCLUSION_MASKS}"
}

function ios_deps()
{
  if [[ ${GIT_BIN} ]]; then
    # Put the resulting output file at project's root dir
    DEPS_FILENAME="../${TIMESTAMP}_${PROJECT_NAME}-${GIT_VER}_ios-dependencies.tar.gz"
  else
    # Put the resulting output file at project's root dir
    DEPS_FILENAME="../${TIMESTAMP}_${PROJECT_NAME}_ios-dependencies.tar.gz"
  fi

  INCLUSION_MASKS="ios/ common/ README.md"

  "${TAR_BIN} ${TAR_ARGS} ${DEPS_FILENAME} --exclude=${EXCLUSION_MASKS} ${INCLUSION_MASKS}"
}

function windows_deps()
{
  if [[ ${GIT_BIN} ]]; then
    # Put the resulting output file at project's root dir
    DEPS_FILENAME="../${TIMESTAMP}_${PROJECT_NAME}-${GIT_VER}_windows-dependencies.zip"
  else
    # Put the resulting output file at project's root dir
    DEPS_FILENAME="../${TIMESTAMP}_${PROJECT_NAME}_windows-dependencies.zip"
  fi

  INCLUSION_MASKS="windows/ common/ README.md"

  "${ZIP_BIN} ${ZIP_ARGS} ${DEPS_FILENAME} ${INCLUSION_MASKS} -x ${EXCLUSION_MASKS}"
}

function linux_deps()
{
  INCLUSION_MASKS="common linux README.md"
  ARCHIVE_FILENAME="${TIMESTAMP}_${PROJECT_NAME}_linux-dependencies.tar.gz"

  # Write the archive file at the root of the project (nomlib.git) with the
  # latest git commit hash as the version of the distributed dependencies.
  if [[ ${GIT_BIN} ]]; then
    ARCHIVE_FILENAME="${TIMESTAMP}_${PROJECT_NAME}-${GIT_VER}_linux-dependencies.tar.gz"
  fi

  "${TAR_BIN} ${TAR_ARGS} ${ARCHIVE_FILENAME} ${INCLUSION_MASKS}"
  # "${TAR_BIN} ${TAR_ARGS} --exclude=${EXCLUSION_MASKS} ${ARCHIVE_FILENAME} ${INCLUSION_MASKS}"
}

function all_deps()
{
  osx_deps
  ios_deps
  linux_deps
  windows_deps
}

echo "DEPS_DIR: $DEPS_DIR"
pushd "${DEPS_DIR}" || exit 255

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
else
  usage_info
  exit 0
fi

popd
exit 0
