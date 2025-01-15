#!/bin/bash
#
#

if [ -n "$INIT_BUILD" ]; then
  pushd /home/jeff/tmp/nomlib/
  git clone "https://github.com/libsdl-org/SDL_ttf.git" sdl2_ttf || exit 2
  pushd sdl2_ttf
  git checkout release-2.0.12
  popd
fi

SDL_LIBS="/mnt/fs1/Projects/nomlib.git.bck/third-party/linux/sdl2/lib/libSDL2.a"
CPPFLAGS="/mnt/fs1/Projects/nomlib.git.bck/third-party/linux/sdl2/include/SDL2"
SDL2_TTF_DEST=/mnt/fs1/Projects/nomlib.git.bck/third-party/linux/sdl2_ttf
export SDL2_TTF_DEST

if [ -n "$DEBUG" ]; then
  export SDL_LIBS
  export CPPFLAGS
  export SDL2_TTF_DEST
fi

#cd /home/jeff/tmp/nomlib/sdl2_ttf.git
#make clean
./autogen.sh
./configure --prefix=$SDL2_TTF_DEST
make install

