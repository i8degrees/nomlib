# sdl2_image.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/sdl2_image -t sdl2_image:0.13.1
# docker run --rm -it -v $(pwd)/.build:/sdl2_image/build \
#   -v $(pwd)/third-party:/vendor \
# sdl2_image:0.13.1
#

#FROM nomlib:latest
FROM sdl2:0.13.1
LABEL maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/libsndfile:13

USER root
WORKDIR /
RUN mkdir -p /sdl2_image
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
  "https://github.com/libsdl-org/SDL_image.git" /sdl2_image

WORKDIR /sdl2_image
RUN git checkout release-2.0.0

WORKDIR /sdl2_image
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;

RUN ./autogen.sh; \
  ./configure \
  --with-sdl-prefix=/tmp/vendor/linux/sdl2 \
  --prefix=/tmp/vendor/linux/sdl2_image
RUN make
RUN make -j1 install

#ENTRYPOINT [ "cmake --version" ]
