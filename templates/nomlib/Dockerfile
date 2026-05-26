# nomlib.Dockerfile:jeff
#
# Base build environment
#
# cd ~/Projects/nomlib.git
# docker build templates/nomlib -t nomlib:0.13.1
# docker run --rm -it -v $(pwd)/third-party:/nomlib/third-party -v $(pwd)/build:/nomlib/build nomlib:0.13.1
#

#FROM silkeh/clang:20-bullseye
FROM i8degrees/nomlib-builder:0.13.1-amd64

LABEL version="0.13.1" \
      tag="i8degrees/nomlib:0.13.1" \
      description="nomlib dev builder" \
      maintainer="Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>"
# i8degrees/nomlib:latest

USER root
WORKDIR /nomlib.git
RUN mkdir -p build

FROM sdl2:0.13.1
WORKDIR /nomlib

# NOTE(JEFF): Any conveniences we wish to have within the container's shell
# should be handled here if we wish for it to be permanent.
RUN git clone \
  https://github.com/i8degrees/nomlib.git \
/nomlib.git && git checkout dev
#WORKDIR /app/third-party
#RUN tar xzvf third-party/

#COPY bin Resources cmake examples extra include src tests third-party vendor version.cmake /app

WORKDIR /nomlib.git/build
RUN \
  clang++ --version >> clang.version; \
  cmake --version >> cmake.version;

CMD ["bash"]

