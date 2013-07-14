/******************************************************************************

    Simple UI interface for drawing a styled message box

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_MESSAGEBOX_HEADERS
#define NOMLIB_SDL_MESSAGEBOX_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "config.hpp"
#include "sys/Logger.hpp"
#include "math/Color.hpp"
#include "math/Coords.hpp"
#include "math/Transformable.hpp"
#include "gfx/SDL_Drawable.hpp"
#include "gfx/SDL_Canvas.hpp"
#include "gfx/SDL_Gradient.hpp"
#include "gfx/SDL_Line.hpp"

namespace nom {

class SDL_MessageBox:// public SDL_Drawable,   // "is a" relationship
                      public Transformable   // "has a" relationship
{
  public:
    SDL_MessageBox ( void );
    virtual ~SDL_MessageBox ( void );

    void Init ( int32_t x, int32_t y, int32_t width, int32_t height, const std::vector<Color> border_colors, const SDL_Gradient& background = SDL_Gradient() );

    bool isEnabled ( void );
    void disable ( void );
    void enable ( void );

    void Update ( void );
    void Draw ( void* video_buffer ); /* const */

  private:
    typedef std::vector<std::shared_ptr<SDL_Drawable>> drawable_t;
    drawable_t lines;
    SDL_Gradient background;
    SDL_Canvas box;

    std::vector<Color> window_borders;
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


} // namespace nom

#endif // NOMLIB_SDL_MESSAGEBOX_HEADERS defined
