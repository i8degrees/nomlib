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
