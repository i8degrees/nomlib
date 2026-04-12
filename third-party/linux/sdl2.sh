#!/bin/bash

SDL2_DEST=/mnt/fs1/Projects/nomlib.git/third-party/linux/sdl2
export SDL2_DEST

if [ -n "$INIT_BUILD" ]; then
  git clone "https://github.com/libsdl-org/SDL.git" sdl2.git
  cd sdl2.git
  #git checkout release-2.0.6
  git checkout release-2.24.0
  mkdir build && cd build && rm -rf *
fi

#-DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake \
  -DCMAKE_INSTALL_PREFIX=$SDL2_DEST
  -DVIDEO_KMSDRM=off -DCMAKE_WAYLAND=on \
  -DWAYLAND_SHARED=off \
..
make -j8
make -j1 install

