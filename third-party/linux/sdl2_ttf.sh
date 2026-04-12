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

SDL_LIBS="/mnt/fs1/Projects/nomlib.git.fixed/third-party/linux/sdl2/lib/libSDL2.a"
CPPFLAGS="/mnt/fs1/Projects/nomlib.git.fixed/third-party/linux/sdl2/include/SDL2"
SDL2_TTF_DEST=/mnt/fs1/Projects/nomlib.git.fixed/third-party/linux/sdl2_ttf
FREETYPE_PREFIX=/mnt/fs1/Projects/nomlig.git.fixed/third-party/linux/freetype
export SDL2_TTF_DEST FREETYPE_PREFIX

#cd /home/jeff/tmp/nomlib/sdl2_ttf.git
#make clean
./autogen.sh
#./configure --prefix=$SDL2_TTF_DEST
./configure --prefix="$SDL2_TTF_DEST" --with-freetype-prefix="$FREETYPE_PREFIX"
make -j4
make -j1 install

