/******************************************************************************
    gfx.h

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_GFX_HEADERS
#define GAMELIB_GFX_HEADERS

#include <iostream>
#include <string>

#include "SDL_image.h"

#include "gamelib.h"

class GColor;

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

    static SDL_Surface *LoadImage ( std::string filename );

    static SDL_Surface *LoadImage ( std::string filename, GColor color,
                                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    /* static */ bool DrawSurface ( SDL_Surface *video_buffer, unsigned int x, unsigned int y );

    /* static */ bool DrawSurface ( SDL_Surface *video_buffer, unsigned int x, unsigned int y,
                                    unsigned int x_offset, unsigned int y_offset,
                                    unsigned int width_offset, unsigned int height_offset );

    // SDLVideo
    bool UpdateScreen ( void );

    // SDLGfx
    bool DrawRectangle (  unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height, GColor color );

    // SDLApp
    static void setTitle ( std::string app_name );

    bool setIcon (  std::string app_icon, GColor color,
                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    static unsigned int getPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y );



    SDL_Surface *screen; // primary (think: visible) video memory
  private:
    //
};

#endif // GAMELIB_GFX_HEADERS defined
