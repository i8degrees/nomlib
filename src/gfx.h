/******************************************************************************
    gfx.h

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston

******************************************************************************/
#ifndef GAMELIB_GFX_HEADERS
#define GAMELIB_GFX_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <memory>

#include "SDL_image.h"

#include "GameState.h"
#include "Color.h"
#include "Coords.h"
#include "timer.h"

#include "gamelib.h"

class Gfx
{
  public:
    Gfx ( unsigned int img_flags = IMG_INIT_PNG );
    ~Gfx ( void );

    // SDLSurface
    static bool setTransparent (  void* video_buffer, const nom::Color& color,
                                  unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    static bool setAlpha (  void* video_buffer, unsigned char opacity,
                            unsigned int flags = SDL_SRCALPHA );

    // SDL_Image: public SDL_Drawable
    static void* LoadImage ( std::string filename, const nom::Color& colorkey = nom::Color ( -1, -1, -1, -1 ),
                                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    // SDL_Surface
    static bool DrawSurface (   void* source_buffer, void* video_buffer,
                                const nom::Coords &coords, const nom::Coords &offsets = nom::Coords ( -1, -1, -1, -1 ) );

    // SDL_Display
    static bool updateSurface ( void* video_buffer );

    // SDL_Shapes: SDL_Drawable
    static unsigned int getPixel ( void* video_buffer, unsigned int x, unsigned int y );
    static void setPixel ( void* video_buffer, unsigned int x, unsigned int y, nom::Color color );
    static bool lockSurface ( void* video_buffer );
    static bool unlockSurface ( void* video_buffer );
    static void drawLine ( void* video_buffer, float x1, float y1, float x2, float y2, nom::Color color );
};

#endif // GAMELIB_GFX_HEADERS defined
