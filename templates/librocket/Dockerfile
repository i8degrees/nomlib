# librocket.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/librocket -t librocket:0.13.1
# docker run --rm -it -v $(pwd)/.build:/librocket/build \
#   -v $(pwd)/third-party:/vendor \
# librocket:0.13.1
#

#FROM nomlib:latest
FROM sdl2:0.13.1
LABEL maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"

USER root
WORKDIR /
RUN apt-get update; \
    apt-get install -y --no-install-recommends \
    autoconf \
    autogen \
    automake \
    build-essential \
    bzip2 \
    cmake \
    curl \
    file \
    git \
    git-lfs \
    libc6 \
    libc6-dev \
    libfreetype-dev \
    libfreetype6 \
    liblua5.1-0 \
    liblua5.1-0-dev \
    libtool \
    make \
    pandoc \
    patch \
    pkg-config \
    python \
    xdg-utils \
    zip \
    zstd
RUN git clone \
  "https://github.com/i8degrees/libRocket.git" /librocket

WORKDIR /librocket
RUN git checkout dev
RUN mkdir build

WORKDIR /librocket/build
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;
RUN cmake \
  -DCMAKE_INSTALL_PREFIX=/tmp/vendor/linux/libRocket \
  -DBUILD_SAMPLES=off \
  -DBUILD_LUA_BINDINGS=on \
  -DCMAKE_BUILD_TYPE=Debug \
  -DROCKET_DEBUG=on \
  -DCMAKE_VERBOSE_MAKEFILE=1 ../Build
RUN make
RUN make -j1 install

#ENTRYPOINT [ "cmake --version" ]

