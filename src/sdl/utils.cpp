/******************************************************************************

    SDL backwards-compatibility wrappers for nomlib

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "utils.hpp"

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
