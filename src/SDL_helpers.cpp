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
#include "nomlib/SDL_helpers.hpp"

namespace nom {

SDL_bool SDL_BOOL ( bool value )
{
  if ( value )
  {
    return SDL_TRUE;
  }
  else
  {
    return SDL_FALSE;
  }
}

SDL_Rect SDL_RECT ( const Coords& coords )
{
  SDL_Rect r;

  r.x = coords.x;
  r.y = coords.y;
  r.w = coords.width;
  r.h = coords.height;

  return r;
}

SDL_Color SDL_COLOR ( const Color& color )
{
  SDL_Color c;

  c.r = color.red;
  c.g = color.green;
  c.b = color.blue;
  c.a = color.alpha;

  return c;
}

const Color pixel ( uint32 pixel, const SDL_PixelFormat* fmt )
{
  Color c;

  SDL_GetRGB ( pixel, fmt, &c.red, &c.green, &c.blue );

  return c;
}

const Color alpha_pixel ( uint32 pixel, const SDL_PixelFormat* fmt )
{
  Color c;

  SDL_GetRGBA ( pixel, fmt, &c.red, &c.green, &c.blue, &c.alpha );

  return c;
}

uint32 RGB ( const Color& color, const SDL_PixelFormat* fmt )
{
  return SDL_MapRGB ( fmt, color.red, color.green, color.blue );
}

uint32 RGB ( const Color& color, uint32 fmt )
{
  return SDL_MapRGB ( SDL_AllocFormat(fmt), color.red, color.green, color.blue );
}

uint32 RGBA ( const Color& color, const SDL_PixelFormat* fmt )
{
  return SDL_MapRGBA ( fmt, color.red, color.green, color.blue, color.alpha );
}

uint32 RGBA ( const Color& color, uint32 fmt )
{
  return SDL_MapRGBA ( SDL_AllocFormat(fmt), color.red, color.green, color.blue, color.alpha );
}

namespace priv {

void FreeWindow ( SDL_Window* handle )
{
  SDL_DestroyWindow ( handle );
}

void FreeRenderTarget ( SDL_Renderer* handle )
{
  SDL_DestroyRenderer ( handle );
}

void FreeTexture ( SDL_Texture* video_buffer )
{
  SDL_DestroyTexture ( video_buffer );
}

void FreeSurface ( SDL_Surface* video_buffer )
{
  SDL_FreeSurface ( video_buffer );
}

void TTF_FreeFont ( TTF_Font* font )
{
  TTF_CloseFont ( font );
}

} // namespace priv
} // namespace nom
