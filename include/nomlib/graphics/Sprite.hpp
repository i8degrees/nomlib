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
#include "nomlib/graphics/Canvas.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/graphics/SpriteSheet.hpp"

namespace nom {

class Sprite:
              public Transformable
{
  public:
    /// Default construct for initializing instance variables to their
    // respective defaults.
    Sprite ( void );

    /// Construct a Sprite object, initializing the width & height coordinates.
    Sprite ( int32 width, int32 height );

    /// Construct a Sprite object, initializing it with a SpriteSheet object.
    Sprite ( const SpriteSheet* sheet );

    /// Construct a Sprite object, initializing it with a Canvas object.
    /// \FIXME
    //Sprite ( const Canvas& copy );

    /// Copy assignment operator.
    Sprite& operator = ( const Sprite& other );

    /// Destructor.
    virtual ~Sprite ( void );

    /// Get the object's state.
    uint32 getState ( void );

    /// Get the object's sheet_id.
    int32 getSheetID ( void );

    /// Set a new state.
    void setState ( uint32 state );

    /// Set a new sheet_id to render (only effective if a SpriteSheet is in use).
    void setSheetID ( int32 id );

    /// Load a new image onto the Sprite.
    bool load (
                std::string filename, Color colorkey,
                bool use_cache = false,
                uint32 flags = SDL_SRCCOLORKEY | SDL_RLEACCEL
              );

    void Update ( void );
    void Draw ( void* video_buffer ) const;

    /// Rescale the font with a chosen resizing algorithm
    bool resize ( enum ResizeAlgorithm scaling_algorithm );

  protected:
    /// Object that holds our sprite image
    Canvas sprite;

    /// alive, dying, dead, ...
    uint32 state;

    /// Source (input) coordinates, used for sprite sheet positioning
    Coords offsets;

    /// Our attached sprite sheet, if we have one
    std::shared_ptr<SpriteSheet> sprite_sheet;

    /// The sheet ID currently in use, if we have a sprite sheet loaded
    int32 sheet_id;

    /// Scale factor applied if the resize method is called
    int32 scale_factor;
};


} // namespace nom

#endif // NOMLIB_SPRITE_HEADERS defined
