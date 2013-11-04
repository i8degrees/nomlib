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
#ifndef NOMLIB_SDL2_COMPAT_HPP
#define NOMLIB_SDL2_COMPAT_HPP

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Rect-inl.hpp"
#include "nomlib/math/Point2-inl.hpp"

namespace nom {

/// SDL2 data structure wrappers for nomlib
///
/// \return A SDL_bool from a boolean value
SDL_bool SDL_BOOL ( bool value );

/// SDL2 data structure wrappers for nomlib
///
/// \return A SDL_Rect structure composed from a nom::Coords object
SDL_Rect SDL_RECT ( const Coords& coords );

/// SDL2 data structure wrappers for nomlib
///
/// \return SDL_Rect structure composed from a nom::Rect<T> object
///
/// \todo Implement me
template <typename T>
SDL_Rect SDL_RECT ( const Rect<T>& )
{
  SDL_Rect null;
  return null;
}

/// SDL2 data structure wrappers for nomlib
///
/// \return SDL_Point structure composed from a nom::Point2<T> object
///
/// \todo Implement me
template <typename T>
SDL_Point SDL_POINT ( const Point2<T>& )
{
  SDL_Point null;
  return null;
}

/// SDL2 data structure wrappers for nomlib
///
/// \return A SDL_Color structure composed from a nom::Color object
SDL_Color SDL_COLOR ( const Color& color );

/// SDL2 helper functions for nomlib
///
/// \return RGB components of a pixel represented as a nom::Color object
const Color pixel ( uint32 pixel, const SDL_PixelFormat* fmt );

/// SDL2 helper functions for nomlib
///
/// \return RGBA components of a pixel represented as a nom::Color object
const Color alpha_pixel ( uint32 pixel, const SDL_PixelFormat* fmt );

/// SDL2 helper functions for nomlib
///
/// Obtain a color as an integer, respective to the video surface pixel format
/// (color bit per pixel).
///
/// \return RGB components as an unsigned integer.
uint32 RGB ( const Color& color, const SDL_PixelFormat* fmt );

/// SDL2 helper functions for nomlib
///
/// Obtain a color as an integer, respective to the video surface pixel format
/// (color bit per pixel).
///
/// \return RGBA components as an unsigned integer.
uint32 RGBA ( const Color& color, const SDL_PixelFormat* fmt );


} // namespace nom

#endif // include guard defined
