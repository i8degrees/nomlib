/******************************************************************************
    gfx.h

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_GFX_HEADERS
#define GAMELIB_GFX_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "SDL_image.h"

#include "GameState.h"
#include "GColor.h"

class GColor;

#include "gamelib.h"

class Gfx
{
  public:
    // SDLVideo
    Gfx ( unsigned int sdl_flags = SDL_INIT_EVERYTHING,
          unsigned int img_flags = IMG_INIT_PNG
        );

    ~Gfx ( void );

    bool SetVideoMode ( unsigned int screen_width = 320,
                        unsigned int screen_height = 240,
                        unsigned int screen_bpp = 8,
                        unsigned int video_flags = 0
                      );

    // SDLSurface
    static bool setTransparent (  SDL_Surface *video_buffer, GColor color,
                                  unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    static SDL_Surface *LoadImage ( std::string filename, GColor colorkey,
                                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    static SDL_Surface *LoadImage ( std::string filename,
                                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    /* static */ bool DrawSurface ( SDL_Surface *video_buffer, unsigned int x, unsigned int y );

    /* static */ bool DrawSurface ( SDL_Surface *video_buffer, unsigned int x, unsigned int y,
                                    unsigned int x_offset, unsigned int y_offset,
                                    unsigned int width_offset, unsigned int height_offset );

    // SDLVideo
    bool UpdateScreen ( void );

    static bool UpdateScreen ( SDL_Surface *video_buffer );

    static bool DrawRectangle ( SDL_Surface *video_buffer, unsigned int x, unsigned int y,
                                unsigned int width, unsigned int height,
                                unsigned int r, unsigned int g, unsigned int b );

    // SDLGfx
    bool DrawRectangle (  unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height, unsigned int r,
                          unsigned int g, unsigned int b );

    // SDLApp
    static void setTitle ( std::string app_name );

    bool setIcon (  std::string app_icon, GColor color,
                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    static unsigned int getPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y );
    static void setPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y, GColor color );

    static bool lockSurface ( SDL_Surface *video_buffer );
    static bool unlockSurface ( SDL_Surface *video_buffer );

    static void drawLine ( SDL_Surface *video_buffer, float x1, float y1, float x2, float y2, GColor color );



    SDL_Surface *screen; // primary (think: visible) video memory
  private:
    //
};

#endif // GAMELIB_GFX_HEADERS defined
