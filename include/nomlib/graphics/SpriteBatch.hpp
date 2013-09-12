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
#ifndef NOMLIB_SPRITE_BATCH_HEADERS
#define NOMLIB_SPRITE_BATCH_HEADERS

#include <iostream>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/graphics/Canvas.hpp"
#include "nomlib/graphics/Sprite.hpp"
#include "nomlib/graphics/SpriteSheet.hpp"

namespace nom {

/// \brief Sprite rendering with sprite sheets
class SpriteBatch:
                    public Sprite
{
  public:
    /// Default construct for initializing instance variables to their
    /// respective defaults.
    SpriteBatch ( void );

    /// Construct a SpriteBatch object, initializing it with a SpriteSheet
    /// object.
    SpriteBatch ( const SpriteSheet& sheet );

    /// Construct a SpriteBatch object from an existing sprite sheet filename.
    SpriteBatch ( const std::string& filename );

    /// Copy assignment operator.
    SpriteBatch& operator = ( const SpriteBatch& other );

    /// Destructor.
    virtual ~SpriteBatch ( void );

    /// Get the object's sheet_id.
    int32 getSheetID ( void ) const;

    /// Set a new frame ID to render
    void setSheetID ( int32 id );

    /// Update the sprite for rendering with regard to positioning coordinates
    /// and target frame ID.
    void Update ( void );

  protected:
    /// Source (input) coordinates -- used for sprite sheet positioning
    Coords offsets;

    /// Our attached sprite sheet object
    SpriteSheet sprite_sheet;

    /// The sheet's frame ID presently in use
    int32 sheet_id;
};


} // namespace nom

#endif // include guard defined
