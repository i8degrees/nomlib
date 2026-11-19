#!/bin/bash
#
#
#

workspace=$1

# docker env
REPOSITORY=i8degrees/nomlib-builder
VERSION=0.13.1
ARCH=amd64

if [ -z "$workspace" ]; then
  echo "Failed to run -- missing workspace path given to script!"
  echo
  exit 1
fi

# inside container paths
INSTALL_PREFIX=/app/vendor
WORKDIR=/app

#rm -rf "$(pwd)/vendor" &&
mkdir -p "${workspace}/vendor"
docker run --rm -it \
  -v ${workspace}/vendor:/dist \
  "${REPOSITORY}:${VERSION}-${ARCH}-libs" \
bash -c 'if findmnt /dist; then rsync -avc ${INSTALL_PREFIX}/* /dist; fi'

