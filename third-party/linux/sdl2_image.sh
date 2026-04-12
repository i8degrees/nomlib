#!/bin/bash
#
#

if [ -n "$INIT_BUILD" ]; then
  cd /home/jeff/tmp/nomlib/
  git clone "https://github.com/libsdl-org/SDL_image.git" sdl2_image.git
  cd sdl2_image.git
  git checkout release-2.0.0
fi


SDL_LIBS="/mnt/fs1/Projects/nomlib.git.fixed/third-party/linux/sdl2/lib/libSDL2.a"
CPPFLAGS="/mnt/fs1/Projects/nomlib.git.fixed/third-party/linux/sdl2/include/SDL2"

SDL2_IMAGE_DEST=/mnt/fs1/Projects/nomlib.git.fixed/third-party/linux/sdl2_image
export SDL2_IMAGE_DEST
SDL_PREFIX=/mnt/fs1/Projects/nomlib.git.fixed/third-party/linux/sdl2

if [ -n "$DEBUG" ]; then
  export SDL_LIBS
  export CPPFLAGS
  export SDL2_IMAGE_DEST
fi

# assume that we are in the right directory, i.e.: /home/jeff/tmp/nomlib/sdl2_image.git/
# build dir...
cd /home/jeff/tmp/nomlib/sdl2_image.git
make clean
./autogen.sh
./configure --prefix=$SDL2_IMAGE_DEST --with-sdl-prefix="$SDL_PREFIX"
make install

