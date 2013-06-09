/******************************************************************************
    Coords.h

  Coordinate values (x, y, z, width, height ) wrapper

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_COORDS_HEADERS
#define GAMELIB_COORDS_HEADERS

#include <iostream>
#include <string>
#include <map>

#include "SDL.h"

namespace nom
{
  class Coords
  {
    public:
      Coords ( void );

      Coords ( signed int x_, signed int y_,
                signed int width_ = 0, signed height_ = 0);

      ~Coords ( void );

      // phase out
      void setCoords (  signed int x_, signed int y_,
                        signed int width_ = 0, signed int height_ = 0 );

      SDL_Rect getSDL_Rect ( void ) const;

      signed int getX ( void ) const;
      signed int getY ( void ) const;
      void setX ( signed int x_ );
      void setY ( signed int y_ );
      void setXY ( signed int x_, signed int y_ );

      signed int getWidth ( void ) const;
      signed int getHeight ( void ) const;

      void setWidth ( signed int width_ );
      void setHeight ( signed int height_ );

      // phase out
      void setDimensions ( signed int width_, signed int height_ );

      void updateX ( signed int x_ );
      void updateY ( signed int y_ );
      void updateXY ( signed int x_ = 0, signed int y_ = 0 );

      void updateWidth ( signed int width_ );
      void updateHeight ( signed int height_ );

      void updateCoords ( signed int x_ = 0, signed int y_ = 0, signed int width_ = 0, signed int height_ = 0 );

    private:
      signed int x;
      signed int y;
      signed int z; // reserved
      signed int width;
      signed int height;
  };
}

#endif // GAMELIB_COORDS_HEADERS defined
