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

#include "Coords.h"
#include "Color.h"
#include "gamelib.h"

namespace nom
{
  class Transformable
  {
    public:
      Transformable ( void );
      virtual ~Transformable ( void );

      int32_t getX ( void ) const;
      int32_t getY ( void ) const;
      const nom::Coords& getXY ( void ) const;
      void setX ( int32_t x );
      void setY ( int32_t y );
      void setXY ( int32_t x, int32_t y );
      const nom::Coords& getPosition ( void ) const;
      void setPosition ( int32_t x, int32_t y );
      void setPosition ( int32_t x, int32_t y, int32_t width, int32_t height );
      void setPosition ( const nom::Coords& coords );

      int32_t getWidth ( void ) const;
      void setWidth ( int32_t width, int32_t height );

      int32_t getHeight ( void ) const;
      void setHeight ( int32_t height );

      // Rename to move
      void updateXY ( int32_t x = 0, int32_t y = 0);

      const nom::Color& getColor ( void ) const;
      void setColor ( const nom::Color& color );

    protected:
      nom::Coords coords;
      nom::Color color;
  };
}

#endif // NOMLIB_TRANSFORMABLE_HEADERS defined
