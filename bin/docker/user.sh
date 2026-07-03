#!/bin/bash
#
#
#

workspace=$1
#workspace="$(pwd)"

# (linux|macos|windows)
if [ -z "$PLATFORMBUILD" ]; then
  PLATFORMBUILD=amd64
fi

# docker env
REPOSITORY=i8degrees/nomlib-builder
VERSION=0.13.1
ARCH=${PLATFORMBUILD}
HOST_WORKDIR="$(pwd)"

if [ -z "$workspace" ]; then
  HOST_VOLUMES+=("-v nomlib-project:/app:rw")
  #echo "Failed to run -- missing workspace path given to script!"
  #echo
  #exit 1
else
  HOST_VOLUMES+=("-v $HOST_WORKDIR:/app:rw");
fi

# inside container paths
VENDOR_PREFIX=/app/vendor/prebuild
WORKDIR="/home/$(id -un)"
STAMP=$(date +%S) # displays in seconds 0..59
#USER_IDS=(-e BUILDER_UID="$( id -u )" -e BUILDER_GID="$( id -g )" -e BUILDER_USER="$( id -un )" -e BUILDER_GROUP="$( id -gn )")

SSH_DIR=
if [ -z "${SSH_DIR}" ]; then
  SSH_DIR="${HOME}/.ssh"
fi

echo $SSH_DIR

#HOST_VOLUMES+=("-v $SSH_DIR:/home/$(id -un)/.ssh");
HOST_VOLUMES+=("-v nomlib-libs:${VENDOR_PREFIX}:rw")
HOST_VOLUMES+=("-v nomlib-bashhistory:/commandhistory:rw")

    #--entrypoint /entrypoint.sh \
#-v $(pwd)/vendor:/app/vendor \
docker run --rm -it \
    --name nomlib-build-${STAMP} \
    -w "${WORKDIR}" \
    --user jeff \
    ${HOST_VOLUMES[@]} \
    "${USER_IDS[@]}" \
    "${REPOSITORY}:${VERSION}-${ARCH}" \
/bin/bash
# The first arg should be given instead of /bin/bash
#$1
