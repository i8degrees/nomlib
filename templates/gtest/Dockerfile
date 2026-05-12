# gtest.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/gtest -t gtest:0.13.1
# docker run --rm -it \
#   -v $(pwd)/.build:/gtest/build \
#   -v $(pwd)/vendor:/vendor \
# gtest:0.13.1
#

FROM nomlib:latest
LABEL maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/libsndfile:13

USER root
WORKDIR /gtest.git
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
    g++ \
    gcc \
    git \
    git-lfs \
    libc6 \
    libc6-dev \
    libtool \
    make \
    patch \
    pkg-config \
    python \
    xdg-utils \
    zip \
    zstd
RUN git clone \
  "https://github.com/google/googletest.git" \
/gtest.git

WORKDIR /gtest.git
#RUN git switch -c v1.8.x ; mkdir googletest/build
RUN git checkout v1.8.x ; mkdir googletest/build

WORKDIR /gtest.git/googletest/build
RUN cmake --version >> cmake.version; \
  gcc --version >> gcc.version; \
  g++ --version >> g++.version;

#-DCMAKE_C_COMPILER=/usr/bin/gcc \
#-DCMAKE_CXX_COMPILER=/usr/bin/gcc++ \
RUN cmake \
  -DCMAKE_INSTALL_PREFIX=/tmp/vendor/linux/gtest ..

#ENTRYPOINT [ "cmake --version" ]

