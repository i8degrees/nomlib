/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_SDL_GRADIENT_HEADERS
#define NOMLIB_SDL_GRADIENT_HEADERS

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/Rectangle.hpp"
//#include "nomlib/graphics/Line.hpp"

namespace nom {

class Gradient//: //public IDrawable,     // "is a" relationship
                //public Transformable  // "has a" relationship
{
  public:
    Gradient( void );

    Gradient  (
                const Color& starting_color, const Color& ending_color,
                int32 x, int32 y, int32 width, int32 height,
                uint32 direction = 0, uint32 x_margin = 0, uint32 y_margin = 0
              );

    virtual ~Gradient ( void );

    const Coords getPosition ( void ) const;
    void setPosition ( const Coords& pos );

    const Coords getSize ( void ) const;
    void setSize ( const Coords& size );

    Color getStartColor ( void ) const;
    Color getEndColor ( void ) const;

    void setStartColor ( const Color& starting_color );
    void setEndColor ( const Color& ending_color );

    uint32 getFillDirection ( void ) const;
    void setFillDirection ( const uint32 direction );

    void Update ( void );
    void Draw ( void* video_buffer ) const;

  private:
    std::vector<std::shared_ptr<Rectangle>> lines;

    /// gradient[0] = starting Color
    /// gradient[1] = ending Color
    Color gradient[2];

    /// X, Y, width & height container
    Coords coords;

    /// x coordinate offset
    int32 x_margin;

    /// y coordinate offset
    int32 y_margin;

    /// color fill direction:
    ///
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
