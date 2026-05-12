# sdl2.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/sdl2 -t sdl2:0.13.1
# docker run --rm -it -v $(pwd)/.build:/sdl2/build \
#   -v $(pwd)/third-party:/vendor \
# sdl2:0.13.1
#

FROM nomlib:latest
LABEL maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/libsndfile:13

USER root
WORKDIR /
RUN mkdir /sdl2
WORKDIR /sdl2
RUN apt-get update; \
    apt-get install -y --no-install-recommends \
    autoconf \
    autogen \
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
    libc6-dev \
    libdrm2 \
    libegl-dev \
    libfreetype-dev \
    libfreetype6 \
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
    libxext-dev \
    libxext6 \
    libxi6 \
    libxinerama1 \
    libxkbcommon0 \
    libxrandr2 \
    libxss1 \
    libxxf86vm1 \
    make \
    mesa-common-dev \
    patch \
    pkg-config \
    python \
    xdg-utils \
    xdg-utils \
    zip \
    zstd
RUN git clone \
  "https://github.com/libsdl-org/SDL.git" /sdl2
RUN git checkout release-2.24.x
RUN mkdir build

WORKDIR /sdl2/build
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;

RUN cmake \
  -DCMAKE_INSTALL_PREFIX=/tmp/vendor/linux/sdl2 \
  -DVIDEO_KMSDRM=off -DCMAKE_WAYLAND=on \
  -DWAYLAND_SHARED=off ..
RUN make
RUN make -j1 install

#ENTRYPOINT [ "cmake --version" ]

