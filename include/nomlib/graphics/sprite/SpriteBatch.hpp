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
#ifndef NOMLIB_GRAPHICS_SPRITE_BATCH_HPP
#define NOMLIB_GRAPHICS_SPRITE_BATCH_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/graphics/sprite/Sprite.hpp"
#include "nomlib/graphics/sprite/SpriteSheet.hpp"

namespace nom {

// TODO: Consider compositing from nom::Sprite instead of inheriting!

/// \brief Extended sprite rendering using sprite sheets
class SpriteBatch: public Sprite
{
  public:
    typedef SpriteBatch self_type;
    typedef Sprite derived_class;

    /// Default construct for initializing instance variables to their
    /// respective defaults.
    ///
    /// \todo Consider initializing sheet_id_ to -1..?
    SpriteBatch();

    /// Destructor.
    virtual ~SpriteBatch();

    /// \brief Use the sprite frames from an existing SpriteSheet object.
    ///
    /// \param sheet The pre-loaded sprite sheet instance to use the frames
    /// from.
    ///
    /// \remarks The dimensions of this object's Sprite instance are initialized
    /// to the first frame of the sprite sheet source.
    ///
    /// \see nom::SpriteSheet::load_file.
    virtual void set_sprite_sheet(const SpriteSheet& sheet);

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type() const;

    /// \brief Implements the required IDrawable::clone method.
    SpriteBatch* clone() const;

    /// Get the object's current sheet_id.
    virtual int32 frame() const;

    /// Obtain the number of frames available
    int32 frames() const;

    /// \brief Set a new frame ID to render.
    ///
    /// \param id The frame identifier (from the sprite sheet) to use in
    /// rendering.
    ///
    /// \remarks Setting the frame number to negative one (-1) will prevent
    /// updating and rendering of the object. This can be used to effectively
    /// toggle visibility of a sprite without the need of using a placeholder
    /// frame in the image source.
    ///
    /// \see ::update.
    void set_frame(int32 id);

    /// \brief Render the sprite frame.
    ///
    /// \remarks The sprite is not rendered when the frame number is negative
    /// one (-1).
    ///
    /// \note Re-implements Sprite::draw.
    virtual void draw(IDrawable::RenderTarget& target) const override;

    /// \brief Render the sprite frame at an angle.
    ///
    /// \remarks The sprite is not rendered when the frame number is negative
    /// one (-1).
    ///
    /// \note Re-implements Sprite::draw.
    virtual void draw(RenderTarget& target, real64 angle) const override;

  protected:
    /// Source (input) coordinates -- used for sprite sheet positioning
    IntRect offsets;

    /// Our attached sprite sheet object
    SpriteSheet sprite_sheet;

    /// The sheet's frame ID presently in use
    int32 sheet_id_;

  private:
    /// \brief Update the sprite for rendering with regard to positioning
    /// coordinates and target frame ID.
    ///
    /// \remarks The sprite is not updated when the frame number is negative
    /// one (-1).
    virtual void update() override;
};

} // namespace nom

#endif // include guard defined
