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

      const int32_t getX ( void ) const;
      const int32_t getY ( void ) const;
      const nom::Coords& getXY ( void ) const;
      void setX ( int32_t x_ );
      void setY ( int32_t y_ );
      void setXY ( int32_t x_, int32_t y_ );
      const nom::Color& getColor ( void ) const;
      void setColor ( const nom::Color& color );

    protected:
      nom::Coords coords;
      nom::Color color;
  };
}

#endif // NOMLIB_TRANSFORMABLE_HEADERS defined
