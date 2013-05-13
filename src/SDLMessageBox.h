/******************************************************************************
    SDLMessageBox.h

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_MESSAGEBOX_HEADERS
#define GAMELIB_MESSAGEBOX_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "GColor.h"
#include "GCoords.h"

class SDLMessageBox
{
  public:
    SDLMessageBox ( void );
    ~SDLMessageBox ( void );

    SDLMessageBox ( GCoords coords, GColor window_top[], GColor window_bottom[], GColor window_sides[] );

    void Update ( void );
    void Draw ( void );

  private:
    bool enabled; // shown or not
    GCoords geomtry; // x, y, width & height
    std::string window_title;
    std::string message_text;

    GColor window_top[4];
    GColor window_bottom[4];
    GColor window_sides[4];
};

#endif // GAMELIB_MESSAGEBOX_HEADERS defined
