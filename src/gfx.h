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
    // SDL_Display
    Gfx (   unsigned int sdl_flags = SDL_INIT_EVERYTHING,
            unsigned int img_flags = IMG_INIT_PNG
        );

    ~Gfx ( void );

    static bool SetVideoMode  ( unsigned int screen_width = 320,
                                unsigned int screen_height = 240,
                                unsigned int screen_bpp = 8,
                                unsigned int video_flags = 0
                              );

    // SDL_Display
    static SDL_Surface* getDisplay ( void );
    static signed int getDisplayWidth ( void );
    static signed int getDisplayHeight ( void );
    static signed int getDisplayColorBits ( void );
    static SDL_PixelFormat* getDisplayPixelFormat ( void );
    static unsigned int getDisplayFlags ( void );
    static Uint16 getDisplayPitch ( void );

    // SDL_Display
    static bool updateDisplay ( void );

    // SDL_Display
    static void setTitle ( std::string app_name );

    static bool setIcon ( std::string app_icon, nom::Color color,
                          unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    // SDL_Display::Surface ( coords ); AKA defaults to blitting to public interface

    // SDLSurface
    static bool setTransparent (  SDL_Surface *video_buffer, nom::Color color,
                                  unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );
    static bool setAlpha (  SDL_Surface *video_buffer, unsigned char opacity,
                            unsigned int flags = SDL_SRCALPHA );

    // SDL_Image: public SDL_Drawable
    static SDL_Surface *LoadImage ( std::string filename, nom::Color colorkey,
                                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    static SDL_Surface *LoadImage ( std::string filename,
                                    unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    // SDL_Surface
    static bool DrawSurface (   SDL_Surface *source_buffer, SDL_Surface *video_buffer,
                                const nom::Coords &coords, const nom::Coords &offsets );

    // SDL_Surface
    static bool updateSurface ( SDL_Surface *video_buffer );

    // SDL_Shapes: SDL_Drawable
    static unsigned int getPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y );
    static void setPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y, nom::Color color );
    static bool lockSurface ( SDL_Surface *video_buffer );
    static bool unlockSurface ( SDL_Surface *video_buffer );
    static void drawLine ( SDL_Surface *video_buffer, float x1, float y1, float x2, float y2, nom::Color color );
    static bool drawRect  ( SDL_Surface *video_buffer, const nom::Coords &coords,
                            const nom::Color &color
                          );

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
