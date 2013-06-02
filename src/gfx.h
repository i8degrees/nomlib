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

#include "SDL_image.h"

#include "GameState.h"
#include "GColor.h"
#include "GCoords.h"
#include "timer.h"

class GColor;

#include "gamelib.h"

class Gfx
{
  public:
    // SDLDisplay
    Gfx (   unsigned int sdl_flags = SDL_INIT_EVERYTHING,
            unsigned int img_flags = IMG_INIT_PNG
        );

    ~Gfx ( void );

    static bool SetVideoMode  ( unsigned int screen_width = 320,
                                unsigned int screen_height = 240,
                                unsigned int screen_bpp = 8,
                                unsigned int video_flags = 0
                              );

    // SDLDisplay
    static SDL_Surface* getDisplay ( void );
    static const signed int getDisplayWidth ( void );
    static const signed int getDisplayHeight ( void );
    static const signed int getDisplayColorBits ( void );
    static /*const*/ SDL_PixelFormat* getDisplayPixelFormat ( void );
    static const unsigned int getDisplayFlags ( void );
    // surface->pitch
    // surface->pixels

    // SDLDisplay
    // TODO: rename to updateDisplay()
    static bool UpdateScreen ( void );

    // SDLDisplay
    static void setTitle ( std::string app_name );

    static bool setIcon ( std::string app_icon, GColor color,
                          unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    // SDLDisplay::Surface ( coords ); AKA defaults to blitting to public interface

    // SDLSurface
    static bool setTransparent (  SDL_Surface *video_buffer, GColor color,
                                  unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );
    static bool setAlpha (  SDL_Surface *video_buffer, unsigned char opacity,
                            unsigned int flags = SDL_SRCALPHA );

    // SDLCache
    static SDL_Surface *LoadImage ( std::string filename, GColor colorkey,
                                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    static SDL_Surface *LoadImage ( std::string filename,
                                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    // SDLSurface
                // Deprecated
    //static bool DrawSurface ( SDL_Surface *video_buffer, unsigned int x, unsigned int y );

    // SDLSurface
                // Transitional; phasing out
    //static bool DrawSurface ( SDL_Surface *video_buffer, unsigned int x, unsigned int y,
                                    //unsigned int x_offset, unsigned int y_offset,
                                    //unsigned int width_offset, unsigned int height_offset );

    // SDLSurface
                // Transitional temp
    //static bool DrawSurface ( SDL_Surface *source_buffer, SDL_Surface *video_buffer,
                              //unsigned int x, unsigned int y,
                              //unsigned int x_offset = 0, unsigned int y_offset = 0,
                              //unsigned int width_offset = 0, unsigned int height_offset = 0
                            //);

    // SDLSurface
                // Future; phasing in
    static bool DrawSurface (   SDL_Surface *source_buffer, SDL_Surface *video_buffer,
                                const GCoords &coords, const GCoords &offsets );

    // SDLSurface
    static bool drawRect (  SDL_Surface *video_buffer, const GCoords &coords,
                            const GColor &color
                  );

    // SDLSurface
    // TODO: rename to updateSurface()
    static bool UpdateScreen ( SDL_Surface *video_buffer );

    // SDLSurface
    static unsigned int getPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y );
    static void setPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y, GColor color );

    static bool lockSurface ( SDL_Surface *video_buffer );
    static bool unlockSurface ( SDL_Surface *video_buffer );

    static void drawLine ( SDL_Surface *video_buffer, float x1, float y1, float x2, float y2, GColor color );

/*
  class GameApp: public SDLInput
    public:
*/
    void ChangeState ( std::unique_ptr<GameState> state );
    void PushState ( std::unique_ptr<GameState> state );
    void PopState ( void );
    void PopStateThenChangeState ( std::unique_ptr<GameState> state );

    void HandleInput ( void );
    void Update ( void );
    void Draw ( SDL_Surface *video_buffer );

    bool isRunning ( void );
    void Run ( void );
    void Quit ( void );

    unsigned int getTicks ( void );

    bool isFullScreen ( void );
    void setFullScreen ( bool toggle );
    bool getShowFPS ( void );
    void showFPS ( bool toggle );
    void toggleFPS ( void );

  private:
    bool running; // global app state
    bool show_fps; // fps counter
    bool fullscreen; // toggle window & fullscreen states
    std::vector<std::unique_ptr<GameState>> states;
    Timer appTime;
};

#endif // GAMELIB_GFX_HEADERS defined
