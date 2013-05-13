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
    //GColor window_borders[8];
    // top border, left border, bottom border, right border; each defined border
    // has two rows of pixels that can be set to a custom color, so ...
    // window_border[0] && window_border[1] contains the top borders,
    // window_border[2] && window_border[3] contains the left borders, and so on.
    //
    // Each border color has three values: red, green & blue.
    //
    // window_border[0].getRed(), window_border[0].getGreen(), window_border[0].getBlue()
    //
};

#endif // GAMELIB_MESSAGEBOX_HEADERS defined
