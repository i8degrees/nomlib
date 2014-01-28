/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/system/SDL_helpers.hpp"

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

SDL_Rect SDL_RECT ( const IntRect& rectangle )
{
  SDL_Rect r;

  r.x = rectangle.x;
  r.y = rectangle.y;
  r.w = rectangle.w;
  r.h = rectangle.h;

  return r;
}

SDL_Rect SDL_RECT ( const Point2i& pos, const Size2i& size )
{
  SDL_Rect r;

  r.x = pos.x;
  r.y = pos.y;
  r.w = size.w;
  r.h = size.h;

  return r;
}

SDL_Point SDL_POINT ( const Point2i& point )
{
  SDL_Point p;

  p.x = point.x;
  p.y = point.y;

  return p;
}

SDL_Color SDL_COLOR ( const Color4i& color )
{
  SDL_Color c;

  c.r = color.r;
  c.g = color.g;
  c.b = color.b;
  c.a = color.a;

  return c;
}

const Color4i pixel ( uint32 pixel, const SDL_PixelFormat* fmt )
{
  SDL_Color c;

  SDL_GetRGB ( pixel, fmt, &c.r, &c.g, &c.b );

  return Color4i ( c.r, c.g, c.b, Color4i::ALPHA_OPAQUE );
}

const Color4i alpha_pixel ( uint32 pixel, const SDL_PixelFormat* fmt )
{
  SDL_Color c;

  SDL_GetRGBA ( pixel, fmt, &c.r, &c.g, &c.b, &c.a );

  return Color4i ( c.r, c.g, c.b, c.a );
}

const Color4i pixel ( uint32 pixel, uint32 fmt )
{
  return nom::pixel ( pixel, SDL_AllocFormat(fmt) );
}

const Color4i alpha_pixel ( uint32 pixel, uint32 fmt )
{
  return nom::alpha_pixel ( pixel, SDL_AllocFormat(fmt) );
}

uint32 RGB ( const Color4i& color, const SDL_PixelFormat* fmt )
{
  return SDL_MapRGB ( fmt, color.r, color.g, color.b );
}

uint32 RGB ( const Color4i& color, uint32 fmt )
{
  return SDL_MapRGB ( SDL_AllocFormat(fmt), color.r, color.g, color.b );
}

uint32 RGBA ( const Color4i& color, const SDL_PixelFormat* fmt )
{
  return SDL_MapRGBA ( fmt, color.r, color.g, color.b, color.a );
}

uint32 RGBA ( const Color4i& color, uint32 fmt )
{
  return SDL_MapRGBA ( SDL_AllocFormat(fmt), color.r, color.g, color.b, color.a );
}

const std::string hint ( const std::string& name )
{
  std::string result;

  result = SDL_GetHint ( name.c_str() );
  return result;
}

bool set_hint ( const std::string& name, const std::string& value )
{
  if ( SDL_SetHint ( name.c_str(), value.c_str() ) == false ) return false;

  return true;
}

const std::string PIXEL_FORMAT_NAME ( uint32 format )
{
  return std::string (SDL_GetPixelFormatName ( format ) );
}

bool save_png ( SDL_SURFACE::RawPtr buffer, const std::string& filename )
{
  if ( buffer == nullptr )
  {
    NOM_LOG_ERR( NOM, "Video surface memory is not valid for saving output");
    return false;
  }

  // Undocumented method for writing out a PNG file with SDL2_image
  if ( IMG_SavePNG ( buffer, filename.c_str() ) != 0 )
  {
    NOM_LOG_ERR(NOM,SDL_GetError());
    return false;
  }

  return true;
}

namespace priv {

void FreeWindow ( SDL_Window* handle )
{
  if ( handle != nullptr )
  {
    SDL_DestroyWindow ( handle );
    handle = nullptr;
  }
}

void FreeRenderTarget ( SDL_Renderer* handle )
{
  if ( handle != nullptr )
  {
    SDL_DestroyRenderer ( handle );
    handle = nullptr;
  }
}

void FreeTexture ( SDL_Texture* video_buffer )
{
  if ( video_buffer != nullptr )
  {
    SDL_DestroyTexture ( video_buffer );
    video_buffer = nullptr;
  }
}

void FreeSurface ( SDL_Surface* video_buffer )
{
  if ( video_buffer != nullptr )
  {
    SDL_FreeSurface ( video_buffer );
    video_buffer = nullptr;
  }
}

void TTF_FreeFont ( TTF_Font* font )
{
  if ( font != nullptr )
  {
    TTF_CloseFont ( font );
    font = nullptr;
  }
}

void Free_Joystick ( SDL_Joystick* joy )
{
  if ( joy != nullptr )
  {
    if ( SDL_JoystickGetAttached ( joy ) )
    {
      SDL_JoystickClose ( joy );
      joy = nullptr;
    }
  }
}

} // namespace priv
} // namespace nom
