# sdl2_ttf.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/sdl2_ttf -t sdl2_ttf:0.13.1
# docker run --rm -it -v $(pwd)/.build:/sdl2_ttf/build \
#   -v $(pwd)/third-party:/vendor \
# sdl2_ttf:0.13.1
#

#FROM nomlib:latest
FROM sdl2:0.13.1
LABEL maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/libsndfile:13

USER root
WORKDIR /
RUN mkdir -p /sdl2_ttf
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
    libfreetype6 \
    libfreetype-dev \
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
  "https://github.com/libsdl-org/SDL_ttf.git" /sdl2_ttf

WORKDIR /sdl2_ttf
RUN git checkout release-2.0.12

WORKDIR /sdl2_ttf
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;

RUN make clean; ./autogen.sh && autoupdate; \
  ./configure \
  --with-sdl-prefix=/tmp/vendor/linux/sdl2 \
  --prefix=/tmp/vendor/linux/sdl2_ttf \
  --with-freetype-prefix=/vendor/linux/freetype
RUN make
RUN make -j1 install

#ENTRYPOINT [ "cmake --version" ]

