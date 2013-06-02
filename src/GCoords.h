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

// namespace nom
class GCoords
{
  public:
    GCoords ( void );

    GCoords ( signed int x_, signed int y_,
              signed int width_ = 0, signed height_ = 0);

    ~GCoords ( void );

    void setCoords (  signed int x_, signed int y_,
                      signed int width_ = 0, signed int height_ = 0 );

    SDL_Rect getSDL_Rect ( void ) const;
    // TODO: return GCoords?
    std::pair<signed int, signed int> getXY ( void );

    signed int getX ( void );
    signed int getY ( void );
    void setX ( signed int x_ );
    void setY ( signed int y_ );
    void setXY ( signed int x_, signed int y_ );

    signed int getWidth ( void );
    signed int getHeight ( void );

    void setWidth ( signed int width_ );
    void setHeight ( signed int height_ );
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

#endif // GAMELIB_COORDS_HEADERS defined
