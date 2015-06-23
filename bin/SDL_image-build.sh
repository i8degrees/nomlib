#!/bin/sh

# Automated build for OS X Framework Bundle

# webp.framework
SRCROOT=${HOME}/Projects/third-party/SDL2_image.hg/Xcode

# installation prefix
TARGET_DIR=/private/tmp/SDL2_image

if [[ -d $SRCROOT ]]; then

  echo "SRCROOT: ${SRCROOT}"
  echo "TARGET_DIR: ${TARGET_DIR}"

  cd ${SRCROOT}

  if [[ -d $TARGET_DIR ]]; then
    rm -rf "${TARGET_DIR}/*"
  else
    mkdir -p "${TARGET_DIR}"
  fi

  xcodebuild clean
  rm -rf build/*
  # If the build fails, this file needs to be deleted before we can restart
  # the build again
  rm -rf Frameworks/Frameworks

  xcodebuild -configuration Debug -scheme Framework \
  SYMROOT="${TARGET_DIR}" \
  BUILD_DIR="${TARGET_DIR}" \
  USER_HEADER_SEARCH_PATHS="${HOME}/Projects/nomlib.git/third-party/osx/SDL2.framework/Headers" FRAMEWORK_SEARCH_PATHS="${HOME}/Projects/nomlib.git/third-party/osx ${SRCROOT}/Frameworks"

  xcodebuild clean
  rm -rf build/*
  rm -rf Frameworks/Frameworks

  xcodebuild -configuration Release -scheme Framework \
  SYMROOT="${TARGET_DIR}" \
  BUILD_DIR="${TARGET_DIR}" \
  USER_HEADER_SEARCH_PATHS="${HOME}/Projects/nomlib.git/third-party/osx/SDL2.framework/Headers" FRAMEWORK_SEARCH_PATHS="${HOME}/Projects/nomlib.git/third-party/osx ${SRCROOT}/Frameworks"
else
  echo "ERROR: Directory path does not exist: ${SRCROOT}"
fi
