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
#ifndef NOMLIB_GREYFRAME_HPP
#define NOMLIB_GREYFRAME_HPP

#include <iostream>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/graphics/Display.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/Line.hpp"
#include "nomlib/gui/IFrame.hpp"

namespace nom {
  namespace ui {

class GrayFrame: public IFrame
{
  public:
    GrayFrame ( void );
    GrayFrame ( int32 x, int32 y, int32 width, int32 height, int32 padding = 1 );
    ~GrayFrame ( void );

    // Re-implemented from IFrame
    void setPosition( int32 x, int32 y );

    // Re-implemented from IFrame
    void setSize( int32 width, int32 height, int32 padding = 1 );

    // Re-implemented from IDrawable
    void Update ( void );

    // Re-implemented from IDrawable
    void Draw ( void* ) const;

  private:
    /// Holds our line objects used for rendering the object
    std::vector<std::shared_ptr<Line>> frame;

    /// Rendering context
    Display context;

    /// Position & Size
    Coords coords;

    /// Track object logic changes for updating its rendering
    bool updated;

    /// Deprecated; this will be removed in a future version
    int32 padding;
};


  } // namespace ui
} // namespace nom

#endif // include guard defined
