/******************************************************************************
    SDLMessageBox.h

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_MESSAGEBOX_HEADERS
#define GAMELIB_MESSAGEBOX_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "SDL.h"

#include "gfx.h"
#include "GColor.h"
#include "GCoords.h"

class SDLGradient;

//#define DEBUG_MESSAGEBOX
#define DEBUG_MESSAGEBOX_OBJ

class SDLMessageBox
{
  public:
    SDLMessageBox ( void );
    ~SDLMessageBox ( void );

    void Init ( unsigned int x, unsigned int y, unsigned int width, unsigned int height );

    bool isEnabled ( void );
    void disable ( bool state );
    void enable ( bool state );

    void setBorder ( GColor &border_colors );
    void setBackground ( SDLGradient *gradient );

    void Update ( void );

    void Draw ( SDL_Surface *video_buffer, unsigned int x, unsigned int y, unsigned int width, unsigned int height );
    void Draw ( SDL_Surface *video_buffer );

  private:
    bool enabled; // shown or not
    GCoords geometry; // x, y, width & height
    SDLGradient *background;
    std::vector<GColor> window_borders;
};

#endif // GAMELIB_MESSAGEBOX_HEADERS defined
