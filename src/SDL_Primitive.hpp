/******************************************************************************
    SDL_Primitive.hpp

    SDL Primitives (pixels, lines, rectangles and other basic geometry shapes)

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_PRIMITIVES_HEADERS
#define NOMLIB_SDL_PRIMITIVES_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Coords.h"
#include "Color.h"
#include "SDL_Drawable.hpp"
#include "gamelib.h"

namespace nom
{
  // This is an inheritance-only class
  class Primitive: public nom::SDL_Drawable // "is a" relationship
  {
    public:
      Primitive ( void )  : coords ( 0, 0, 0, 0 ), color ( 0, 0, 0, SDL_ALPHA_OPAQUE )
      {
        // ...
      }

      virtual ~Primitive ( void )
      {
        // ...
      }

      void setPosition ( const Coords& coords, const Color& color )
      {
        this->coords = coords;
        this->color = color;
      }

    protected:
      nom::Coords coords;
      nom::Color color;
  };
}

#endif // NOMLIB_SDL_PRIMITIVES_HEADERS defined
