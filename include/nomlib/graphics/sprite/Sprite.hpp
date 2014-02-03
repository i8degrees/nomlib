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
class Sprite: public Transformable
{
  public:
    typedef Sprite self_type;
    typedef Transformable derived_class;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// Default construct for initializing instance variables to their
    /// respective defaults.
    Sprite ( void );

    /// Destructor.
    virtual ~Sprite ( void );

    /// Construct a Sprite object, initializing the width & height coordinates.
    Sprite ( int32 width, int32 height );

    /// Copy assignment operator.
    Sprite& operator = ( const Sprite& other );

    /// \brief Implements the required IDrawable::clone method.
    IDrawable::raw_ptr clone( void ) const;

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    SDL_TEXTURE::RawPtr texture ( void ) const;

    const Size2i size ( void ) const;

    /// Get the object's state.
    uint32 state ( void ) const;

    /// Set a new state.
    void set_state ( uint32 state );

    /// \brief Initialize a Sprite from an image file.
    ///
    /// \param type nom::Texture::Access enumeration type
    ///
    /// \remarks  In order to use the resize method of this class, you must load
    ///           the sprite image with nom::Texture::Access::Streaming access.
    bool load ( const std::string& filename,
                bool use_cache = false,
                enum Texture::Access type = Texture::Access::Static
              );

    virtual void update ( void );

    void draw ( RenderTarget& target ) const;

    /// Draw a rotated nom::Sprite on a nom::RenderWindow
    ///
    /// \param  target  Reference to an active nom::RenderWindow
    /// \param  angle   Rotation angle in degrees
    ///
    void draw ( RenderTarget& target, const double angle ) const;

    /// \brief Rescale the sprite with the chosen resizing algorithm
    ///
    /// \remarks See nom::Texture::resize
    bool resize ( enum Texture::ResizeAlgorithm scaling_algorithm );

  protected:
    /// Object that holds our sprite image
    Texture sprite_;

    /// Convenience instance variable (user-defined)
    uint32 state_;

    /// Scale factor applied if the resize method is called
    int scale_factor;
};


} // namespace nom

#endif // include guard defined
