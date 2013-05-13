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

    GCoords ( unsigned int x, unsigned int y,
              unsigned int width = 0, unsigned height = 0 );

    ~GCoords ( void );

    void setCoords (  unsigned int x, unsigned int y,
                      unsigned int width, unsigned int height );

    unsigned int getX ( void );
    unsigned int getY ( void );
    std::pair<unsigned int, unsigned int> getXY ( void );
    void setXY ( unsigned int x, unsigned int y );

    unsigned int getWidth ( void );
    unsigned int getW ( void );
    unsigned int getHeight ( void );
    unsigned int getH ( void );

    std::pair<unsigned int, unsigned int> getDimensions ( void );
    void setDimensions ( unsigned int width, unsigned int height );

  private:
    std::pair<unsigned int, unsigned int> coords; // x, y coords
    std::pair<unsigned int, unsigned int> dims; // width, height coords
};

#endif // GAMELIB_COORDS_HEADERS defined
