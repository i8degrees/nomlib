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
#include "nomlib/graphics/sprite/Sprite.hpp"

// Private headers
#include "nomlib/graphics/shapes/Rectangle.hpp"

// Forward declarations
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/RenderWindow.hpp"

namespace nom {

inline static
void ReferenceDeleter(Texture* tex)
{
  // Do nothing; sprite's texture is **not** owned by us
}

Sprite::Sprite() :
  Transformable(Point2i::zero, Size2i::zero)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_RENDER, NOM_LOG_PRIORITY_VERBOSE);
}

Sprite::~Sprite()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_RENDER, NOM_LOG_PRIORITY_VERBOSE);
}

Sprite::Sprite(Texture& tex)
{
  this->set_texture(tex);
}

Sprite::Sprite(Texture* tex)
{
  this->set_texture(tex);
}

Sprite::Sprite(std::shared_ptr<Texture>& tex)
{
  this->set_texture(tex);
}

bool Sprite::init_with_color(const Color4i& color, const Size2i& dims)
{
  Point2i rect_pos(Point2i::zero);
  Size2i rect_dims(dims);
  IntRect rect_bounds(rect_pos, rect_dims);

  Rectangle rect(rect_bounds, color);
  auto tex = rect.texture();

  return this->set_texture(tex);
}

ObjectTypeInfo Sprite::type() const
{
  return NOM_OBJECT_TYPE_INFO(self_type);
}

void Sprite::set_position(const Point2i& pos)
{
  Transformable::set_position(pos);

  if( this->texture_ != nullptr ) {
    this->texture_->set_position( this->position() );
  }
}

void Sprite::set_size(const Size2i& dims)
{
  Transformable::set_size(dims);

  if( this->texture_ != nullptr ) {
    this->texture_->set_size( this->size() );
  }
}

std::unique_ptr<Sprite> Sprite::clone() const
{
  return( nom::make_unique<self_type>( self_type(*this) ) );
}

std::shared_ptr<Texture> Sprite::texture() const
{
  return this->texture_;
}

bool Sprite::valid() const
{
  if( this->texture_ != nullptr ) {
    return this->texture_->valid();
  } else {
    return false;
  }
}

uint8 Sprite::alpha() const
{
  uint8 alpha = 0; // default return

  if( this->valid() == true ) {
    alpha = this->texture_->alpha();
  } else {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not get the alpha value of the sprite." );
  }

  return alpha;
}

Color4i Sprite::color() const
{
  Color4i color = Color4i::null; // default return

  if( this->valid() == true ) {
    color = this->texture_->color_modulation();
  } else {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not get the color value of the sprite." );
  }

  return color;
}

BlendMode Sprite::color_blend_mode() const
{
  // default return
  BlendMode mode = BlendMode::BLEND_MODE_NONE;

  if( this->valid() == true ) {
    mode = nom::blend_mode( this->texture_->blend_mode() );
  } else {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not get the color blending mode of the sprite." );
  }

  return mode;
}

bool Sprite::set_texture(Texture& tex)
{
  this->texture_.reset(&tex, ReferenceDeleter);

  NOM_ASSERT(this->texture_ != nullptr);
  if( this->texture_ != nullptr ) {
    this->set_position( this->texture_->position() );
    this->set_size( this->texture_->size() );
    return true;
  } else {
    // Err; out of memory???
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not allocate a texture for the sprite." );
    return false;
  }
}

bool Sprite::set_texture(Texture* tex)
{
  this->texture_.reset(tex);

  NOM_ASSERT(this->texture_ != nullptr);
  if( this->texture_ != nullptr ) {
    this->set_position( this->texture_->position() );
    this->set_size( this->texture_->size() );
    return true;
  } else {
    // Err; out of memory???
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not allocate a texture for the sprite." );
    return false;
  }
}

bool Sprite::set_texture(std::shared_ptr<Texture>& tex)
{
  this->texture_ = tex;

  NOM_ASSERT(this->texture_ != nullptr);
  if( this->texture_ != nullptr ) {
    this->set_position( this->texture_->position() );
    this->set_size( this->texture_->size() );
    return true;
  } else {
    // Err; out of memory???
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not allocate texture for sprite." );
    return false;
  }
}

bool Sprite::set_alpha(uint8 opacity)
{
  NOM_ASSERT(this->texture_ != nullptr);
  if( this->texture_ != nullptr ) {
    return this->texture_->set_alpha(opacity);
  } else {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not set the alpha value of the sprite." );
    return false;
  }
}

bool Sprite::set_color(const Color4i& color)
{
  NOM_ASSERT(this->texture_ != nullptr);
  if( this->valid() == true ) {
    return this->texture_->set_color_modulation(color);
  } else {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not set the color of the sprite." );
    return false;
  }
}

bool Sprite::set_color_blend_mode(BlendMode blend)
{
  SDL_BlendMode mode = nom::SDL_blend_mode(blend);

  NOM_ASSERT(this->texture_ != nullptr);
  if( this->texture_ != nullptr ) {
    return this->texture_->set_blend_mode(mode);
  } else {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not set the color blending mode of the sprite." );
    return false;
  }
}

void Sprite::release_texture()
{
  this->texture_.reset();
}

void Sprite::draw(RenderTarget& target) const
{
  if( this->valid() == true ) {
    this->texture_->draw( target.renderer() );
  }
}

void Sprite::draw(RenderTarget& target, real64 degrees) const
{
  if( this->valid() == true ) {
    this->texture_->draw(target.renderer(), degrees);
  }
}

// Private scope

void Sprite::update()
{
  // Stub
}

} // namespace nom
