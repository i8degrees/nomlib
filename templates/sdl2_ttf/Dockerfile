# sdl2_ttf.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/sdl2_ttf -t sdl2_ttf:0.13.1
# docker run --rm -it -v $(pwd)/.build:/sdl2_ttf/build sdl2_ttf:0.13.1
#

#FROM nomlib:latest
LABEL maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/libsndfile:13

USER root
WORKDIR /gtest.git
RUN apt-get update; \
    apt-get install -y --no-install-recommends \
    autoconf autogen automake build-essential libasound2-dev libflac-dev libogg-dev libtool libvorbis-dev libopus-dev libmp3lame-dev libmpg123-dev pkg-config python \
    bzip2 \
    cmake \
    curl \
    file \
    git \
    git-lfs \
    libc6 \
    make \
    patch \
    libc6-dev \
    pandoc \
    xdg-utils \
    zip \
    zstd
RUN git clone \
  "https://github.com/google/gtest.git" \
/sdl2_ttf.git

WORKDIR /sdl2_ttf.git
RUN git switch -c 1.8.x && mkdir build

WORKDIR /sdl2_ttf.git/build
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;
RUN cmake \
  -DBUILD_SHARED_LIBS=1 \
  -DCMAKE_INSTALL_PREFIX=/third-party/linux/sdl2_ttf \
  -DCMAKE_BUILD_TYPE=RELEASE \
.. && make

#ENTRYPOINT [ "cmake --version" ]

