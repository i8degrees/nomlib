#!/bin/bash
#
#

[ -n "$DEBUG_TRACE" ] && set -o xtrace

SDL_LIBS="/mnt/fs1/Projects/nomlib.git/third-party/linux/sdl2/lib/libSDL2.a"
CPPFLAGS="/mnt/fs1/Projects/nomlib.git/third-party/linux/sdl2/include/SDL2"
SDL2_TTF_DEST=/mnt/fs1/Projects/nomlib.git/third-party/linux/sdl2_ttf
export SDL2_TTF_DEST
# freetype2/bin/freetype-config
FREETYPE_PREFIX=/mnt/fs1/Projects/nomlib.git/third-party/linux/freetype2
export FREETYPE_PREFIX

if [ -n "$DEBUG" ]; then
  export SDL_LIBS
  export CPPFLAGS
  export SDL2_TTF_DEST
  export FREETYPE_PREFIX
fi

SRC_PREFIX_DIR=/mnt/fs1/Projects/nomlib.git/vendor
if [ -e "$SRC_PREFIX_DIR" ]; then
  git clone "https://github.com/libsdl-org/SDL_ttf.git" sdl2_ttf.git || exit 2
  pushd "sdl2_ttf.git"
  git checkout release-2.0.12
  ./autogen.sh
  autoupdate
  # !! We must use our freetype-config workaround script here as the original
  # script no longer exists; it has been replaced with a pkg-config equivalent.
  ./configure --prefix=$SDL2_TTF_DEST --with-freetype-prefix=$FREETYPE_PREFIX
  make
  make -j1 install
  popd
else
  echo "CRITICAL: The path at $SRC_PREFIX_DIR does not exist!"
  echo
  exit 255
fi

