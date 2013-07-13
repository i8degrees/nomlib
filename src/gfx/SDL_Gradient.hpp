/******************************************************************************

    SDL Gradient effects

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_SDL_GRADIENT_HEADERS
#define GAMELIB_SDL_GRADIENT_HEADERS

#include <iostream>
#include <string>

#include "config.hpp"
#include "sys/Logger.hpp"
#include "math/Coords.hpp"
#include "math/Color.hpp"
#include "math/Transformable.hpp"
#include "gfx/SDL_Drawable.hpp"
#include "gfx/SDL_Rectangle.hpp"

namespace nom
{
  class SDL_Gradient//: public nom::SDL_Drawable // "is a" relationship
  {
    public:
      SDL_Gradient ( void );
      SDL_Gradient  ( const nom::Color& starting_color, const nom::Color& ending_color,
                      int32_t x = 0, int32_t y = 0,
                      int32_t width = 0, int32_t height = 0,
                      uint32_t direction = 0,  uint32_t x_margin = 0,
                      uint32_t y_margin = 0
                    );

      virtual ~SDL_Gradient ( void );

      nom::Color getStartColor ( void ) const;
      nom::Color getEndColor ( void ) const;

      void setStartColor ( const nom::Color& starting_color );
      void setEndColor ( const nom::Color& ending_color );

      uint32_t getFillDirection ( void ) const;
      void setFillDirection ( const uint32_t direction );

      void Update ( void );
      void Draw ( void* video_buffer ); /* const */

    private:
      nom::Rectangle rectangle;
      /// gradient[0] = starting nom::Color
      /// gradient[1] = ending nom::Color
      nom::Color gradient[2];
      /// geometry coordinates
      nom::Coords coords;
      /// x coordinate offset
      int32_t x_margin;
      /// y coordinate offset
      int32_t y_margin;
      /// color fill direction:
      /// direction = 0 is ending color to starting color
      /// direction = 1 is starting color to ending color
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
