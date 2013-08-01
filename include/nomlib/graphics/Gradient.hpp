/******************************************************************************

    Gradient effects

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_GRADIENT_HEADERS
#define NOMLIB_SDL_GRADIENT_HEADERS

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/system/Logger.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/Rectangle.hpp"

namespace nom {

class Gradient: public IDrawable,     // "is a" relationship
                public Transformable  // "has a" relationship
{
  public:
    Gradient ( void );
    Gradient  ( const Color& starting_color, const Color& ending_color,
                    int32 x = 0, int32 y = 0,
                    int32 width = 0, int32 height = 0,
                    uint32 direction = 0,  uint32 x_margin = 0,
                    uint32 y_margin = 0
                  );
    virtual ~Gradient ( void );

    Color getStartColor ( void ) const;
    Color getEndColor ( void ) const;

    void setStartColor ( const Color& starting_color );
    void setEndColor ( const Color& ending_color );

    uint32 getFillDirection ( void ) const;
    void setFillDirection ( const uint32 direction );

    void Update ( void );
    void Draw ( void* video_buffer ) const;

  private:
    std::vector<std::shared_ptr<IDrawable>> rectangles;
    /// gradient[0] = starting Color
    /// gradient[1] = ending Color
    Color gradient[2];
    /// x coordinate offset
    int32 x_margin;
    /// y coordinate offset
    int32 y_margin;
    /// color fill direction:
    /// direction = 0 is ending color to starting color
    /// direction = 1 is starting color to ending color
    uint32 direction;
};


} // namespace nom
/*

  #include <nomlib/graphics.hpp>

  nom::Gradient linear;

  linear.setEndColor ( nom::Color ( 99, 99, 99, 255 ) );
  linear.setStartColor ( nom::Color ( 67, 67, 67, 255 ) );

  // Alternative
  linear = SDL_Gradient ( nom::Color ( 67, 67, 67, 255 ), nom::Color ( 99, 99, 99, 255 ), 104, 194, 176, 24, 0, 3, 4 );

  linear.Draw ( video_buffer );

*/

#endif // NOMLIB_SDL_GRADIENT_HEADERS defined
