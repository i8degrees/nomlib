/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
#ifndef NOMLIB_GUI_GRAY_WINDOW_HPP
#define NOMLIB_GUI_GRAY_WINDOW_HPP

#include "SDL.h" // SDL2

#include "nomlib/config.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/graphics/RenderWindow.hpp"
#include "nomlib/graphics/shapes/Line.hpp"

namespace nom {

class GrayWindow: public Transformable
{
  public:
    typedef std::shared_ptr<GrayWindow> SharedPtr;
    typedef std::unique_ptr<GrayWindow> UniquePtr;
    typedef GrayWindow* RawPtr;

    GrayWindow ( void );
    virtual ~GrayWindow ( void );

    //GrayWindow& operator = ( const GrayWindow& other );

    GrayWindow ( const IntRect& bounds, int pad );

    GrayWindow ( const Point2i& pos, const Size2i& size, int pad );

    /*const IntRect&*/int padding ( void ) const;

    void set_padding ( int pad );

    // Re-implemented from IDrawable
    void update ( void );

    // Re-implemented from IDrawable
    void draw ( RenderTarget& target ) const;

  private:
    /// Holds our line objects used for rendering the object.
    IDrawable::UniqueDrawables frame_;

    /// Deprecated; this will be removed in a future version?
    /*IntRect&*/int padding_;

    /// Track object logic changes for updating its rendering
    bool updated_;
};

} // namespace nom

#endif // include guard defined
