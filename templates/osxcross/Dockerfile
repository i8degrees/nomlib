# osxcross.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/osxcross -t osxcross:0.13.1
# docker run --rm -it
#   -v $(pwd)/third-party:/vendor
#   -v $(pwd)/.build:/osxcross/build \
# osxcross:0.13.1
#
# syntax=docker/dockerfile:1
#

ARG VERSION=0.13.1

FROM joseluisq/docker-osxcross:1.0.0-beta.2
LABEL version="${VERSION}" \
      tag="i8degrees/osxcross:0.13.1" \
      description="Darwin toolchain to build nomlib for MacOS" \
      maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"

USER root
WORKDIR /
RUN apt-get update; \
    apt-get install -y --no-install-recommends \
    autoconf \
    automake \
    build-essential \
    bzip2 \
    cmake \
    curl \
    doxygen \
    file \
    git \
    git-lfs \
    graphviz \
    libc6-dev \
    libgl-dev \
    libgles-dev \
    libglu1-mesa-dev \
    make \
    pandoc \
    patch \
    xdg-utils \
    zip \
    zstd

#RUN mkdir -p /nomlib/build && cd /nomlib/build

#FROM nomlib:0.13.1 AS build-nom
#RUN cmake \
#-DCMAKE_TOOLCHAIN=  \
#..

