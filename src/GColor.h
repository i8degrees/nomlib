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

#include "gamelib.h"

class GColor
{
  public:
    GColor ( void );
    GColor ( unsigned int r, unsigned int g, unsigned int b, unsigned int a = -1 ); // Reconsider a = -1
    ~GColor ( void );

    GColor getColor ( void );
    unsigned int getRed ( void );
    unsigned int getGreen ( void );
    unsigned int getBlue ( void );
    unsigned int getAlpha ( void );

    void setRed ( unsigned int r );
    void setGreen ( unsigned int g );
    void setBlue ( unsigned int b );
    void setAlpha ( unsigned int a = -1 ); // Reconsider

    void setColor ( unsigned int r, unsigned int g, unsigned int b, unsigned int a = -1 ); // Reconsider a = -1

    // SDLColor: public GColor ?
    static unsigned int mapRGB (  SDL_PixelFormat *video_buffer, unsigned int r,
                                  unsigned int g, unsigned int b );

    static unsigned int mapRGBA ( SDL_PixelFormat *video_buffer, unsigned int r,
                                  unsigned int g, unsigned int b, unsigned int a );

    static unsigned int mapRGB (  SDL_PixelFormat *video_buffer, GColor c );

    static unsigned int mapRGBA ( SDL_PixelFormat *video_buffer, GColor c );

  private:
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;
};

#endif // GAMELIB_COLOR_HEADERS defined
