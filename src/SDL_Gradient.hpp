/******************************************************************************
    SDL_Gradient.hpp

  SDL Gradient effects

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_SDL_GRADIENT_HEADERS
#define GAMELIB_SDL_GRADIENT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Coords.hpp"
#include "Color.hpp"
#include "SDL_Drawable.hpp"
#include "Transformable.hpp"
#include "SDL_Rectangle.hpp"
#include "gamelib.h"

namespace nom
{
  class SDL_Gradient: //public nom::SDL_Drawable, // "is a" relationship
                      public nom::Transformable // "has a" relationship
  {
    public:
      SDL_Gradient ( void );
      virtual ~SDL_Gradient ( void );

      /// gradient[0] = starting color
      /// gradient[1] = ending color

      void Init ( const nom::Color& starting_color, const nom::Color& ending_color,
                  unsigned int x = 0, unsigned int y = 0,
                  unsigned int width = 0, unsigned int height = 0,
                  unsigned int direction = 0,  unsigned int x_margin = 0,
                  unsigned int y_margin = 0 );

      /// direction { 0 } = ending color to starting color
      /// direction { 1 } = starting color to ending color

      void Draw ( void* video_buffer, unsigned int x, unsigned int y,
                  unsigned int width, unsigned int height, unsigned int direction = 0
                );

    private:
      nom::Color gradient[2]; /// holds R, G, B * 2
      //nom::Coords coords; /// x, y, width, height coords
      uint32_t x_margin; /// x coordinate offset
      uint32_t y_margin; /// y coordinate offset
      uint32_t direction;
  };
}
/*

  nom::SDL_Gradient linear;

  nom::Color starting ( 66, 66, 66 );
  nom::Color ending ( 99, 99, 99 );

  linear.Init ( starting, ending, 104, 194, 176, 24, 0, 3, 4 );
  linear.Draw ( screen, 104, 194, 176, 24, 0 );

*/

#endif // GAMELIB_SDL_GRADIENT_HEADERS defined
