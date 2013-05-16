/******************************************************************************
    SDLGradient.h

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_SDL_GRADIENT_HEADERS
#define GAMELIB_SDL_GRADIENT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "GCoords.h"
#include "GColor.h"
#include "gfx.h"

//#define DEBUG_GRADIENT
//#define DEBUG_GRADIENT_OBJ

class SDLGradient
{
  public:
    SDLGradient ( void );
    ~SDLGradient ( void );

    // gradient[0] = starting color
    // gradient[1] = ending color

    void Init ( GColor starting_color, GColor ending_color,
                unsigned int x = 0, unsigned int y = 0,
                unsigned int width = 0, unsigned int height = 0,
                unsigned int direction = 0,  unsigned int x_margin = 3,
                unsigned int y_margin = 4 );
                // These default margin offsets are used in TTcards Info Boxes
                // If they are not set here, they default to zero (0)

    // direction { 0 } = ending color to starting color
    // direction { 1 } = starting color to ending color

    void Draw ( SDL_Surface *video_buffer, unsigned int x, unsigned int y,
                unsigned int width, unsigned int height, unsigned int direction = 0
              );

    //void Draw ( SDL_Surface *video_buffer );

  private:
    GColor gradient[2]; // holds R, G, B * 2
    GCoords coords; // x, y, width, height coords
    unsigned int x_margin; // x coordinate offset
    unsigned int y_margin; // y coordinate offset
    unsigned int direction;
};

/*

  SDLGradient linear;

  GColor starting ( 66, 66, 66 );
  GColor ending ( 99, 99, 99 );

  linear.Init ( starting, ending, 104, 194, 176, 24, 0, 3, 4 );
  linear.Draw ( this->engine->screen, 104, 194, 176, 24, 0 );

*/

#endif // GAMELIB_SDL_GRADIENT_HEADERS defined
