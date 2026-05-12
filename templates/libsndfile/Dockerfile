# libsndfile.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/libsndfile -t libsndfile:0.13.1
# docker run --rm -it -v $(pwd)/third-party:/nomlib/third-party -v $(pwd)/.build:/libsndfile/build libsndfile:0.13.1
#

#FROM local:0.13.1
FROM nomlib:latest
LABEL maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/libsndfile:13

USER root
WORKDIR /libsndfile.git
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
  "https://github.com/libsndfile/libsndfile.git" \
/libsndfile.git

WORKDIR /libsndfile.git
RUN git switch -c 1.0.25 && mkdir build

WORKDIR /libsndfile.git/build
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;
RUN cmake \
  -DBUILD_SHARED_LIBS=1 \
  -DCMAKE_INSTALL_PREFIX=/.build/deps/linux/libsndfile \
  -DCMAKE_BUILD_TYPE=DEBUG \
  -DBUILD_EXAMPLES=off \
.. && make

#ENTRYPOINT [ "cmake --version" ]

