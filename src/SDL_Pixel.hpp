/******************************************************************************
    SDL_Pixel.hpp

  SDL Point (Pixel) Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_PIXEL_HEADERS
#define NOMLIB_SDL_PIXEL_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Coords.hpp"
#include "Color.hpp"
#include "SDL_Drawable.hpp"
#include "Transformable.hpp"
#include "gamelib.h"

namespace nom
{
  class Pixel:  public nom::SDL_Drawable,   // "is a" relationship
                public nom::Transformable   // "has a" relationship
  {
    public:
      Pixel ( void );
      virtual ~Pixel ( void );
      Pixel ( const nom::Coords& coords, const nom::Color& color );
      Pixel ( int32_t x, int32_t y, const nom::Color& color );
      void Update ( void );

      void Draw ( void* video_buffer ) const;
    private:
      // ...
  };
}

#endif // NOMLIB_SDL_PIXEL_HEADERS defined
