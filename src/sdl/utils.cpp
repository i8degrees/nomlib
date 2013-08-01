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
#include "nomlib/sdl/utils.hpp"

namespace nom {

SDL_Rect getSDL_Rect ( const Coords& coords_ )
{
  SDL_Rect coords;

  coords.x = coords_.x;
  coords.y = coords_.y;
  coords.w = coords_.width;
  coords.h = coords_.height;

  return coords;
}

SDL_Color getSDL_Color ( const Color& color_ )
{
  SDL_Color color;

  color.r = color_.red;
  color.g = color_.green;
  color.b = color_.blue;
  //color.a = color_.alpha;

  return color;
}

uint32 getColorAsInt ( void* pixel_format, const Color& color )
{
  return SDL_MapRGB ( static_cast<SDL_PixelFormat*> ( pixel_format ), color.red, color.green, color.blue );
}

uint32 getAlphaColorAsInt ( void* pixel_format, const Color& color )
{
  return SDL_MapRGBA ( static_cast<SDL_PixelFormat*> ( pixel_format ), color.red, color.green, color.blue, color.alpha );
}


} // namespace nom
