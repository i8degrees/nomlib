# local.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/local -t local:0.13.1
# docker run --rm -it -v $(pwd)/third-party:/local/third-party -v $(pwd)/.build:/local/build local:0.13.1
#

FROM silkeh/clang:20-bullseye
LABEL maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/nomlib:latest

USER root
WORKDIR /nomlib
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
    libasound2 \
    libc6 \
    libdrm2 \
    libgbm1 \
    libgl-dev \
    libglu1-mesa-dev \
    libegl-dev \
    libgles-dev \
    libpulse0 \
    libwayland-client0 \
    libwayland-cursor0 \
    libwayland-egl1 \
    libx11-6 \
    libxcursor1 \
    libxext6 \
    libxi6 \
    libxinerama1 \
    libxkbcommon0 \
    libxrandr2 \
    libxss1 \
    libxxf86vm1 \
    make \
    patch \
    libc6-dev \
    pandoc \
    mesa-common-dev \
    xdg-utils \
    zip \
    zstd
#RUN mkdir -p /nomlib/build && cd /nomlib/build

WORKDIR /nomlib/build
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;
RUN cmake \
  -DDEBUG=on -DDEBUG_ASSERT=on \
  -DEXAMPLES=off -DNOM_BUILD_TESTS=off \
..

#ENTRYPOINT [ "cmake --version" ]

