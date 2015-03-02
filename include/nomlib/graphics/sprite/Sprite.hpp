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
#ifndef NOMLIB_GRAPHICS_SPRITE_HPP
#define NOMLIB_GRAPHICS_SPRITE_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/system/SDL_helpers.hpp"

namespace nom {

// Forward declarations
class Texture;
class RenderWindow;
typedef const RenderWindow RenderTarget;

/// \brief Base class for bitmap objects
class Sprite: public Transformable
{
  public:
    typedef Sprite self_type;
    typedef Transformable derived_class;

    /// Default construct for initializing instance variables to their
    /// respective defaults.
    Sprite();

    /// Destructor.
    virtual ~Sprite();

    /// \brief Construct a sprite from an existing texture source.
    ///
    /// \see ::set_texture
    Sprite(Texture& tex);

    /// \brief Construct a sprite from an existing texture source.
    ///
    /// \see ::set_texture
    Sprite(Texture* tex);

    /// \brief Construct a sprite from an existing texture source.
    ///
    /// \see ::set_texture
    Sprite(std::shared_ptr<Texture>& tex);

    bool init_with_color(const Color4i& color, const Size2i& dims);

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type() const override;

    /// \brief Re-implements Transformable::set_position.
    virtual void set_position(const Point2i& pos) override;

    /// \brief Re-implements Transformable::set_size.
    virtual void set_size(const Size2i& dims) override;

    /// \brief Implements the required IDrawable::clone method.
    Sprite* clone() const;

    std::shared_ptr<Texture>& texture();

    bool valid() const;

    /// \brief Get the texture color of the sprite.
    Color4i color() const;

    /// \brief Get the color blend mode of the sprite.
    BlendMode color_blend_mode() const;

    /// \brief Get the alpha value of the sprite.
    uint8 alpha() const;

    /// \brief Set the texture source for the sprite.
    ///
    /// \param tex  A valid nom::Texture reference.
    ///
    /// \remarks You are responsible for ensuring that the nom::Texture object
    /// outlives the destruction of this instance.
    ///
    /// \see nom::Texture::load
    bool set_texture(Texture& tex);

    /// \brief Set the texture source for the sprite.
    ///
    /// \param tex  A valid nom::Texture pointer.
    ///
    /// \remarks The ownership of the pointer is transferred to this instance.
    ///
    /// \see nom::Texture::load
    bool set_texture(Texture* tex);

    /// \brief Set the texture source for the sprite.
    ///
    /// \param tex  A valid nom::Texture std::shared_ptr.
    ///
    /// \see nom::Texture::load
    bool set_texture(std::shared_ptr<Texture>& tex);

    bool set_alpha(uint8 opacity);

    bool set_color(const Color4i& color);

    bool set_color_blend_mode(BlendMode blend);

    /// \brief Free the stored texture.
    ///
    /// \remarks This decrements the shared reference count, potentially
    /// freeing the resource.
    void release_texture();

    virtual void draw(RenderTarget& target) const override;

    /// Draw a rotated nom::Sprite on a nom::RenderWindow
    ///
    /// \param  target  Reference to an active nom::RenderWindow
    /// \param  angle   Rotation angle in degrees
    virtual void draw(RenderTarget& target, real64 angle) const;

  protected:
    /// \brief The underlying texture for the sprite.
    std::shared_ptr<Texture> texture_;

  private:
    virtual void update() override;
};

} // namespace nom

#endif // include guard defined
