/******************************************************************************
    GCoords.h

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_COORDS_HEADERS
#define GAMELIB_COORDS_HEADERS

#include <iostream>
#include <string>
#include <map>

#include "SDL.h"

// TODO: GRect superclass AKA std::pair template <class _T1, class _T2>

class GCoords
{
  public:
    GCoords ( void );

    GCoords ( signed int x, signed int y,
              signed int width = -1, signed height = -1 );

    ~GCoords ( void );

    void setCoords (  signed int x = -1, signed int y = -1,
                      signed int width = -1, signed int height = -1 );

    signed int getX ( void );
    signed int getY ( void );
    std::pair<signed int, signed int> getXY ( void );
    void setXY ( signed int x, signed int y );

    signed int getWidth ( void );
    signed int getW ( void );
    signed int getHeight ( void );
    signed int getH ( void );

    std::pair<signed int, signed int> getDimensions ( void );
    void setDimensions ( signed int width, signed int height );

  private:
    std::pair<signed int, signed int> coords; // x, y coords
    std::pair<signed int, signed int> dims; // width, height coords
};

#endif // GAMELIB_COORDS_HEADERS defined
