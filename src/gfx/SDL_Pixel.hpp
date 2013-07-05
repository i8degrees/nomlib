/******************************************************************************

    SDL Point (Pixel) Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_PIXEL_HEADERS
#define NOMLIB_SDL_PIXEL_HEADERS

#include <iostream>
#include <string>

#include <SDL/SDL.h>

#include "math/Color.hpp"
#include "math/Coords.hpp"
#include "math/Transformable.hpp"
#include "gfx/SDL_Drawable.hpp"
#include "nomlib_config.hpp"

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
