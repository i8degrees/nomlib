# local.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/local -t local:0.13.1
# docker run --rm -it -v $(pwd)/third-party:/local/third-party -v $(pwd)/.build:/local/build local:0.13.1
#

FROM silkeh/clang:20-bullseye

LABEL version=0.13.1 \
      description="" \
      maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/local:latest

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
    less \
    libasound2 \
    libc6 \
    libc6-dev \
    libdrm2 \
    libegl-dev \
    libgbm1 \
    libgl-dev \
    libgles-dev \
    libglu1-mesa-dev \
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
    mesa-common-dev \
    pandoc \
    patch \
    rsync \
    xdg-utils \
    zip \
    zstd

RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;

#ENTRYPOINT [ "cmake --version" ]
