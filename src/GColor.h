/******************************************************************************
    GColor.h

  Color container class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_COLOR_HEADERS
#define GAMELIB_COLOR_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

namespace nom
{
  class Color
  {
  public:
    Color ( void );
    Color ( unsigned int r, unsigned int g, unsigned int b, unsigned int a = -1 ); // Reconsider a = -1
    ~Color ( void );

    SDL_Color getSDL_Color ( void ) const;
    Color getColor ( void ) const;
    unsigned int getRed ( void ) const;
    unsigned int getGreen ( void ) const;
    unsigned int getBlue ( void ) const;
    unsigned int getAlpha ( void ) const;

    void setRed ( unsigned int r );
    void setGreen ( unsigned int g );
    void setBlue ( unsigned int b );
    void setAlpha ( unsigned int a = -1 ); // Reconsider

    void setColor ( unsigned int r, unsigned int g, unsigned int b, unsigned int a = -1 ); // Reconsider a = -1

    // SDL wrappers
    unsigned int getColorAsInt ( SDL_PixelFormat *pixel_buffer ) const;

    static unsigned int mapRGB (  SDL_PixelFormat *video_buffer, unsigned int r,
                                  unsigned int g, unsigned int b );

    static unsigned int mapRGBA ( SDL_PixelFormat *video_buffer, unsigned int r,
                                  unsigned int g, unsigned int b, unsigned int a );

    static unsigned int mapRGB (  SDL_PixelFormat *video_buffer, nom::Color c );

    static unsigned int mapRGBA ( SDL_PixelFormat *video_buffer, nom::Color c );

  private:
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;
  };
}

#endif // GAMELIB_COLOR_HEADERS defined
