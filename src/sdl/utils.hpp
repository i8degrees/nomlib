/******************************************************************************

    SDL backwards-compatibility wrappers for nomlib

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_UTILS_HEADERS
#define NOMLIB_SDL_UTILS_HEADERS

#include <SDL/SDL.h>

#include "math/Color.hpp"
#include "math/Coords.hpp"
#include "nomlib_config.hpp"

namespace nom
{
  /// Returns a SDL_Rect structure of a nom::Coords object
  SDL_Rect getSDL_Rect ( const Coords& );

  /// Returns a SDL color structure of a nom::Color object
  SDL_Color getSDL_Color ( const Color& );

  /// Convenience helper for obtaining a color as an integer, respective to
  /// the video surface pixel format (color bit per pixel)
  uint32 getColorAsInt ( void*, const Color& );

  /// Convenience helper for obtaining a color as an integer, respective to
  /// the video surface pixel format (color bit per pixel)
  uint32 getAlphaColorAsInt ( void*, const Color& );

} // namespace nom

#endif // NOMLIB_SDL_UTILS_HEADERS defined
