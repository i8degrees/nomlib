/******************************************************************************

    SDL backwards-compatibility wrappers for nomlib

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_UTILS_HEADERS
#define NOMLIB_SDL_UTILS_HEADERS

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "config.hpp"
#include "audio/AL/OpenAL.hpp"
#include "math/Color.hpp"
#include "math/Coords.hpp"

/// The custom deleters exist solely as debugging tools right now, but are also
/// implemented in case needs arise in the future that require more
/// sophisticated means of freeing resources.
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

  /// Custom deleter for SDL_Surface* smart pointers
  void Canvas_FreeSurface ( SDL_Surface* );

  /// Custom deleter for the display context smart pointer -- this is managed by
  /// SDL and thus we do not own it (let SDL worry about freeing it)
  void Display_FreeSurface ( SDL_Surface* );

  /// Custom deleter for TTF_Font* smart pointers
  void TTF_FreeSurface ( TTF_Font* );

  /// Custom deleter for freeing an OpenAL audio device
  void AL_FreeAudioDevice ( ALCdevice* );

  /// Custom deleter for freeing an OpenAL audio context
  void AL_FreeAudioContext ( ALCcontext* );

} // namespace nom

#endif // NOMLIB_SDL_UTILS_HEADERS defined
