/******************************************************************************
    gfx.h

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston

******************************************************************************/
#ifndef NOMLIB_GFX_HEADERS
#define NOMLIB_GFX_HEADERS

#include <iostream>
#include <string>
#include <cmath>

#include "SDL_image.h"

#include "Color.h"
#include "Coords.h"
#include "gamelib.h"

class Gfx
{
  public:
    static bool setTransparent (  void* video_buffer, const nom::Color& color,
                                  unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    static bool setAlpha (  void* video_buffer, unsigned char opacity,
                            unsigned int flags = SDL_SRCALPHA );

    static void* LoadImage ( std::string filename, const nom::Color& colorkey = nom::Color ( -1, -1, -1, -1 ),
                                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    static bool DrawSurface (   void* source_buffer, void* video_buffer,
                                const nom::Coords &coords = nom::Coords ( 0, 0, -1, -1 ), const nom::Coords& offsets = nom::Coords ( 0, 0, -1, -1 ) );

    static bool updateSurface ( void* video_buffer );

    static unsigned int getPixel ( void* video_buffer, unsigned int x, unsigned int y );
    static void setPixel ( void* video_buffer, unsigned int x, unsigned int y, nom::Color color );
    static bool lockSurface ( void* video_buffer );
    static bool unlockSurface ( void* video_buffer );
    static void drawLine ( void* video_buffer, float x1, float y1, float x2, float y2, nom::Color color );
};

#endif // NOMLIB_GFX_HEADERS defined
