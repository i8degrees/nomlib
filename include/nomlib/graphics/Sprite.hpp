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
#ifndef NOMLIB_SPRITE_HEADERS
#define NOMLIB_SPRITE_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Rect-inl.hpp"
#include "nomlib/graphics/Canvas.hpp"

namespace nom {

/// \brief Sprite object container
class Sprite:
              public Transformable
{
  public:
    /// Default construct for initializing instance variables to their
    /// respective defaults.
    Sprite ( void );

    /// Construct a Sprite object, initializing the width & height coordinates.
    Sprite ( int32 width, int32 height );

    /// Construct a Sprite object, initializing it with a Canvas object.
    /// \FIXME
    ///Sprite ( const Canvas& copy );

    /// Copy assignment operator.
    Sprite& operator = ( const Sprite& other );

    /// Destructor.
    virtual ~Sprite ( void );

    const Coords getSize ( void ) const;

    /// Get the object's state.
    uint32 getState ( void ) const;

    /// Set a new state.
    void setState ( uint32 state );

    /// Load a new image onto the Sprite.
    bool load (
                const std::string& filename, const Color& colorkey,
                bool use_cache = false,
                uint32 flags = SDL_SRCCOLORKEY | SDL_RLEACCEL
              );

    void Update ( void );
    void Draw ( void* video_buffer ) const;

    /// Rescale the font with a chosen resizing algorithm
    bool resize ( enum ResizeAlgorithm scaling_algorithm );

    bool resize ( const Vector2f& scale_factor );

  protected:
    /// Object that holds our sprite image
    Canvas sprite;

    /// Convenience instance variable (user-defined)
    uint32 state;

    /// Scale factor applied if the resize method is called
    int32 scale_factor;
};


} // namespace nom

#endif // NOMLIB_SPRITE_HEADERS defined
