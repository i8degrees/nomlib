# libsndfile.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/libsndfile -t libsndfile:0.13.1
# docker run --rm -it \
#   -v $(pwd)/third-party:/vendor \
#   -v $(pwd)/.build:/libsndfile/build \
# libsndfile:0.13.1
#

FROM local:0.13.1
#FROM nomlib:latest
LABEL maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/libsndfile:13

USER root
WORKDIR /libsndfile
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
    libasound2-dev \
    libpulse0 \
    libc6 \
    libc6-dev \
    libflac-dev \
    libmp3lame-dev \
    libmpg123-dev \
    libogg-dev \
    libopus-dev \
    libpipewire-0.3-0 \
    libpipewire-0.3-dev \
    libspa-0.2-dev \
    libtool \
    libvorbis-dev \
    make \
    patch \
    pkg-config \
    python \
    xdg-utils \
    zip \
    zlib1g-dev \
    libpulse-dev \
    zstd
RUN git clone \
  "https://github.com/libsndfile/libsndfile.git" \
/libsndfile

WORKDIR /libsndfile
RUN git switch -c 1.0.25 && mkdir build

WORKDIR /libsndfile/build
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;
RUN cmake \
  -DBUILD_SHARED_LIBS=1 \
  -DCMAKE_INSTALL_PREFIX=/tmp/vendor/linux/libsndfile \
  -DCMAKE_BUILD_TYPE=DEBUG \
  -DBUILD_EXAMPLES=off \
  -DBUILD_PROGRAMS=off \
  -DBUILD_TESTING=off \
..
RUN make
RUN make -j1 install

#ENTRYPOINT [ "cmake --version" ]

