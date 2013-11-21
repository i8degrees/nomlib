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
#ifndef NOMLIB_SDL2_SPRITE_HPP
#define NOMLIB_SDL2_SPRITE_HPP

#include <iostream>
#include <string>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/Renderer.hpp"

namespace nom {

/// \brief Base class for bitmap objects
class Sprite:
              public Transformable
{
  public:
    /// Default construct for initializing instance variables to their
    /// respective defaults.
    Sprite ( void );

    /// Construct a Sprite object, initializing the width & height coordinates.
    Sprite ( int32 width, int32 height );

    /// Copy assignment operator.
    Sprite& operator = ( const Sprite& other );

    /// Destructor.
    virtual ~Sprite ( void );

    const Point2i size ( void ) const;
    const Point2i position ( void ) const;

    /// Get the object's state.
    uint32 getState ( void ) const;

    /// Set a new state.
    void setState ( uint32 state );

    /// Load a new image onto the Sprite.
    bool load (
                const std::string& filename,
                const Color4u& colorkey,
                bool use_cache = false
              );

    virtual void update ( void );
    void draw ( RenderTarget target ) const;

    /// Draw a rotated nom::Sprite on a nom::Window
    ///
    /// \param  target  Reference to an active nom::Window
    /// \param  angle   Rotation angle in degrees
    ///
    void draw ( RenderTarget target, const double angle ) const;

    /// Rescale the font with a chosen resizing algorithm
    bool resize ( enum ResizeAlgorithm scaling_algorithm );

    bool resize ( const Point2f& scale_factor );

  protected:
    /// Object that holds our sprite image
    Texture sprite;

    /// Convenience instance variable (user-defined)
    uint32 state;

    /// Scale factor applied if the resize method is called
    int32 scale_factor;
};


} // namespace nom

#endif // include guard defined
