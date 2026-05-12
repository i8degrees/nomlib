# openal-soft.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/openal-soft -t openal-soft:0.13.1
# docker run --rm -it \
#   -v $(pwd)/third-party:/vendor \
#   -v $(pwd)/.build:/openal-soft/build \
# openal-soft:0.13.1
#

#FROM local:0.13.1
FROM libsndfile:0.13.1
LABEL maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/libsndfile:13

USER root
WORKDIR /
RUN apt-get update; \
    apt-get install -y --no-install-recommends \
    gcc g++ \
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
  "https://github.com/kcat/openal-soft" /openal-soft
WORKDIR /openal-soft
#RUN git switch -c 1.15.1 && mkdir build
#RUN git switch -c 1.17.1
RUN git checkout master
#RUN mkdir /openal-soft/build
WORKDIR /openal-soft/build
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;
RUN cmake \
  -DCMAKE_C_COMPILER=/usr/bin/gcc \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
  -DCMAKE_INSTALL_PREFIX=/tmp/vendor/linux/openal-soft \
..
RUN make
RUN make -j1 install

#ENTRYPOINT [ "cmake --version" ]

