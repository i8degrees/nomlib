/******************************************************************************
    SDL_MessageBox.hpp

  Simple UI interface for drawing a styled message box

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_MESSAGEBOX_HEADERS
#define NOMLIB_SDL_MESSAGEBOX_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "Color.h"
#include "Coords.h"
#include "Transformable.hpp"
#include "SDL_Drawable.hpp"
#include "SDL_Canvas.hpp"
#include "SDL_Gradient.hpp"
#include "SDL_Line.hpp"
#include "gamelib.h"

namespace nom
{
  class SDL_MessageBox: public nom::SDL_Drawable,   // "is a" relationship
                        public nom::Transformable   // "has a" relationship
  {
    public:
      SDL_MessageBox ( void );
      ~SDL_MessageBox ( void );

      void Init ( int32_t x, int32_t y, int32_t width, int32_t height, const std::vector<nom::Color> border_colors, nom::SDL_Gradient *gradient = NULL );

      bool isEnabled ( void );
      void disable ( void );
      void enable ( void );

      void Update ( void );
      void Draw ( void* video_buffer );

    private:
      typedef std::vector<std::shared_ptr<nom::SDL_Drawable>> drawable_t;
      drawable_t lines;
      nom::SDL_Gradient* background;
      nom::SDL_Canvas box;

      std::vector<nom::Color> window_borders;
      // top border, left border, bottom border, right border; each defined border
      // has two rows of pixels that can be set to a custom color, so ...
      // window_border[0] && window_border[1] contains the top borders,
      // window_border[2] && window_border[3] contains the left borders, and so on.
      //
      // Each border color has three values: red, green & blue.
      //
      // window_border[0].getRed(), window_border[0].getGreen(), window_border[0].getBlue()
      //
      bool enabled; // shown or not
  };
}

#endif // NOMLIB_SDL_MESSAGEBOX_HEADERS defined
