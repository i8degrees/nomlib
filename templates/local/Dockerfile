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
    libc6-dev \
    make \
    pandoc \
    patch \
    xdg-utils \
    zip \
    zstd

WORKDIR /
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;

