/******************************************************************************
    Transformable.hpp

  Inheritance class for adding Coords & Color methods

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_TRANSFORMABLE_HEADERS
#define NOMLIB_TRANSFORMABLE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "gamelib.h"

#include "Coords.h"
#include "Color.h"

namespace nom
{
  class Transformable
  {
    public:
      Transformable ( void );
      virtual ~Transformable ( void );

    private:
      nom::Coords coords;
      nom::Coords offsets;
      nom::Color color;
  };
}

#endif // NOMLIB_TRANSFORMABLE_HEADERS defined
