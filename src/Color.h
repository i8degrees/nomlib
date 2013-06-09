/******************************************************************************
    Color.h

    Utility class container for RGBA colors

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_COLORS_HEADERS
#define NOMLIB_COLORS_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

namespace nom
{
  class Color
  {
  public:
    Color ( void );
    Color ( int32_t r, int32_t g, int32_t b, int32_t a = -1 );
    ~Color ( void );

    Color getColor ( void ) const;
    int32_t getRed ( void ) const;
    int32_t getGreen ( void ) const;
    int32_t getBlue ( void ) const;
    int32_t getAlpha ( void ) const;

    void setRed ( int32_t r );
    void setGreen ( int32_t g );
    void setBlue ( int32_t b );
    void setAlpha ( int32_t a );

    void setColor ( int32_t r, int32_t g, int32_t b, int32_t a = -1 );

    /// SDL compatibility wrappers
    SDL_Color getSDL_Color ( void ) const;
    uint32_t getColorAsInt ( void* pixel_format ) const;

  private:
    int32_t red;
    int32_t green;
    int32_t blue;
    int32_t alpha;
  };
}

#endif // NOMLIB_COLORS_HEADERS defined
