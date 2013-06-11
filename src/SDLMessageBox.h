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

#include "Color.h"
#include "Coords.h"
#include "Transformable.hpp"
#include "SDL_Canvas.hpp"
#include "SDL_Gradient.hpp"
#include "SDL_Line.hpp"
#include "gamelib.h"

class SDLMessageBox:  public nom::Transformable // "has a" relationship
{
  public:
    SDLMessageBox ( void );
    ~SDLMessageBox ( void );

    void Init ( int32_t x, int32_t y, int32_t width, int32_t height, const std::vector<nom::Color> border_colors, nom::SDL_Gradient *gradient = NULL );

    bool isEnabled ( void );
    void disable ( void );
    void enable ( void );

    void Update ( void );
    void Draw ( void* video_buffer );

  private:
    bool enabled; // shown or not

    nom::SDL_Gradient* background;
    std::vector<nom::Color> window_borders;
    typedef std::vector<std::shared_ptr<nom::SDL_Drawable>> drawable_t;
    drawable_t lines;
    nom::SDL_Canvas box;
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
