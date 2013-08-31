/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_SDL_UTILS_HEADERS
#define NOMLIB_SDL_UTILS_HEADERS

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "nomlib/config.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"

/// \brief SDL backwards-compatibility wrappers for nomlib

namespace nom {

  /// Returns a SDL_Rect structure of a nom::Coords object
  SDL_Rect getSDL_Rect ( const Coords& );

  /// Returns a SDL color structure of a nom::Color object
  SDL_Color getSDL_Color ( const Color& );

  /// Return a nom::Color from a SDL_Color struct
  const Color mapSDL_Color ( SDL_Color color );

  /// Convenience helper for obtaining a color as an integer, respective to
  /// the video surface pixel format (color bit per pixel)
  uint32 getColorAsInt ( void*, const Color& );

  /// Convenience helper for obtaining a color as an integer, respective to
  /// the video surface pixel format (color bit per pixel)
  uint32 getAlphaColorAsInt ( void*, const Color& );

} // namespace nom

#endif // NOMLIB_SDL_UTILS_HEADERS defined
