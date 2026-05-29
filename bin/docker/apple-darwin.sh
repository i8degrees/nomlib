#!/bin/bash
#
#
#

workspace=$1
#workspace="$(pwd)"

# (linux|macos|windows)
if [ -z "$PLATFORMBUILD" ]; then
  PLATFORMBUILD=amd64-macos
fi

# docker env
REPOSITORY=i8degrees/nomlib-builder
VERSION=0.13.1
ARCH=${PLATFORMBUILD}

if [ -z "$workspace" ]; then
  echo "Failed to run -- missing workspace path given to script!"
  echo
  exit 1
fi

# inside container paths
VENDOR_PREFIX=/app/vendor
WORKDIR=/app

#-v $(pwd)/vendor:/app/vendor \
# --user app \
docker run --rm -it \
		-v nomlib-libs:${VENDOR_PREFIX}:rw \
    -v "${workspace}":/app:rw \
		-w "${WORKDIR}" \
		"${REPOSITORY}:${VERSION}-${ARCH}" \
/bin/bash

