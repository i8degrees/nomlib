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
#ifndef NOMLIB_SDL2_HELPERS_HPP
#define NOMLIB_SDL2_HELPERS_HPP

#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"

#include "nomlib/config.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Rect-inl.hpp"
#include "nomlib/math/Point2-inl.hpp"

namespace nom {

/// \brief Convenience definitions for pointer types
namespace SDL_WINDOW
{
  typedef std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> UniquePtr;
  typedef SDL_Window* RawPtr;
}

/// \brief Convenience definitions for pointer types
namespace SDL_RENDERER
{
  typedef std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> UniquePtr;
  typedef SDL_Renderer* RawPtr;
}

/// \brief Convenience definitions for pointer types
namespace SDL_PIXELFORMAT
{
  typedef SDL_PixelFormat* RawPtr;
}

/// \brief Convenience definitions for pointer types
namespace SDL_SURFACE
{
  typedef std::unique_ptr<SDL_Surface, void(*) (SDL_Surface*)> UniquePtr;
  typedef std::shared_ptr<SDL_Surface> SharedPtr;
  typedef SDL_Surface* RawPtr;
}

/// \brief Convenience definitions for pointer types
namespace SDL_TEXTURE
{
  typedef std::shared_ptr<SDL_Texture> SharedPtr;
  typedef SDL_Texture* RawPtr;
}

/// SDL2 data structure wrappers for nomlib
///
/// \return A SDL_bool from a boolean value
SDL_bool SDL_BOOL ( bool value );

/// SDL data structure wrappers for nomlib
///
/// \return A SDL_Rect structure composed from a nom::Coords object
SDL_Rect SDL_RECT ( const Coords& rectangle );

/// SDL data structure wrappers for nomlib
///
/// \return SDL_Rect structure composed from a nom::IntRect object
///
/// \todo Test me out
SDL_Rect SDL_RECT ( const IntRect& rectangle );

/// SDL2 data structure wrappers for nomlib
///
/// \return SDL_Point structure composed from a nom::Point2i object
SDL_Point SDL_POINT ( const Point2i& );

/// SDL2 data structure wrappers for nomlib
///
/// \return A SDL_Color structure composed from a nom::Color object
SDL_Color SDL_COLOR ( const Color4u& color );

/// SDL helper functions for nomlib
///
/// \return RGB components of a pixel represented as a nom::Color object
const Color4u pixel ( uint32 pixel, const SDL_PixelFormat* fmt );

/// SDL helper functions for nomlib
///
/// \return RGBA components of a pixel represented as a nom::Color object
const Color4u alpha_pixel ( uint32 pixel, const SDL_PixelFormat* fmt );

/// SDL helper functions for nomlib
///
/// Obtain a color as an integer, respective to the video surface pixel format
/// (color bit per pixel).
///
/// \return RGB components as an unsigned integer.
uint32 RGB ( const Color4u& color, const SDL_PixelFormat* fmt );

/// SDL2 helper functions for nomlib
///
/// Obtain a color as an integer, respective to the video surface pixel format
/// enum, a new SDL2 structure
///
/// \return RGBA components as an unsigned integer.
uint32 RGB ( const Color4u& color, uint32 fmt );

/// SDL helper functions for nomlib
///
/// Obtain a color as an integer, respective to the video surface pixel format
/// (color bit per pixel).
///
/// \return RGBA components as an unsigned integer.
uint32 RGBA ( const Color4u& color, const SDL_PixelFormat* fmt );

/// SDL2 helper functions for nomlib
///
/// Obtain a color as an integer, respective to the video surface pixel format
/// enum, a new SDL2 structure
///
/// \return RGBA components as an unsigned integer.
uint32 RGBA ( const Color4u& color, uint32 fmt );

/// SDL helper function
///
/// Wrapper for SDL_GetTicks
uint32 ticks ( void );

/// SDL2 helper function
///
/// Wrapper for SDL_GetHint
///
/// \param name   Hint to query
///
/// \return Value of the queried name, or a null-terminated string if not set
const std::string hint ( const std::string& name );

/// SDL2 helper function
///
/// Wrapper for SDL_SetHint
///
/// \param name   Name of hint to set
/// \param value  New value to set
///
/// Note that it is best to set certain hints -- such as
/// SDL_RENDERER_PRESENTVSYNC -- before the first call to nom::Window::create.
bool set_hint ( const std::string& name, const std::string& value );

namespace priv {

/// Custom deleter for SDL_Window structures
void FreeWindow ( SDL_Window* );

/// Custom deleter for SDL_Renderer structures
void FreeRenderTarget ( SDL_Renderer* );

/// Custom deleter for SDL_Texture structures
void FreeTexture ( SDL_Texture* );

/// Custom deleter for SDL_Surface structures
void FreeSurface ( SDL_Surface* );

/// Custom deleter for TTF_Font* structures
void TTF_FreeFont ( TTF_Font* );

/// Custom deleter for SDL_Joystick* structures
void Free_Joystick ( SDL_Joystick* );

} // namespace priv
} // namespace nom

#endif // include guard defined
