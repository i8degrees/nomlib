# nomlib.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/nomlib -t nomlib:0.13.1
# docker run --rm -it -v $(pwd)/third-party:/nomlib/third-party -v $(pwd)/build:/nomlib/build nomlib:0.13.1
#

ARG VERSION=0.13.1
FROM local:0.13.1
#FROM silkeh/clang:20-bullseye
#LABEL version="${VERSION}" \
LABEL version="0.13.1" \
      tag="i8degrees/nomlib:0.13.1" \
      description="" \
      maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/nomlib:latest

USER root
WORKDIR /nomlib
RUN apt-get update; \
    apt-get install -y --no-install-recommends \
    bzip2 \
    cmake \
    curl \
    doxygen \
    file \
    git \
    git-lfs \
    graphviz \
    pandoc \
    xdg-utils \
    zip \
    zstd
#RUN mkdir -p /nomlib/build && cd /nomlib/build

FROM sdl2:0.13.1
WORKDIR /nomlib

# NOTE(JEFF): Any conveniences we wish to have within the container's shell
# should be handled here if we wish for it to be permanent.
RUN git clone \
  https://github.com/i8degrees/nomlib.git \
/nomlib && git checkout dev

#WORKDIR /nomlib/third-party
#RUN wget \
#  "https://nomlib.479831.xyz/files/common/2026-05-06_nomlib-a9bbad13_common-dependencies.tar.gz"
#RUN tar xzvf 2026-05-06_nomlib-a9bbad13_common-dependencies.tar.gz

#COPY bin Resources cmake examples extra include src tests third-party vendor version.cmake /nomlib

WORKDIR /nomlib/build
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;
#RUN cmake \
  #-DDEBUG=on -DDEBUG_ASSERT=on \
#-DEXAMPLES=off -DNOM_BUILD_TESTS=off \
#..

#ENTRYPOINT [ "cmake --version" ]

