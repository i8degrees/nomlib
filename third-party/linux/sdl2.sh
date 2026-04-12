#!/bin/bash

SDL2_DEST=/mnt/fs1/Projects/nomlib.git.bck/third-party/linux/sdl2
export SDL2_DEST

cd /home/jeff/tmp/nomlib/

if [ -n "$INIT_BUILD" ]; then
  git clone "https://github.com/libsdl-org/SDL.git" sdl2.git
  cd sdl2.git
  git checkout release-2.0.6
  mkdir build && cd build && rm -rf *
fi

cmake \
  -DCMAKE_INSTALL_PREFIX=$SDL2_DEST
  -DVIDEO_KMSDRM=off -DVIDEO_WAYLAND=off \
..

make install

