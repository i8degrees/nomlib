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
#include "nomlib/graphics/sprite/SpriteBatch.hpp"

// Forward declarations
#include "nomlib/graphics/Texture.hpp"

namespace nom {

SpriteBatch::SpriteBatch() :
  sheet_id_(0)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_RENDER, NOM_LOG_PRIORITY_VERBOSE);
}

SpriteBatch::~SpriteBatch()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_RENDER, NOM_LOG_PRIORITY_VERBOSE);
}

void SpriteBatch::set_sprite_sheet(const SpriteSheet& sheet)
{
  IntRect dims;

  this->sprite_sheet = sheet;

  dims = this->sprite_sheet.dimensions(0);

  Sprite::set_size( dims.size() );

  this->set_frame(0);

  this->update();
}

ObjectTypeInfo SpriteBatch::type() const
{
  return NOM_OBJECT_TYPE_INFO(self_type);
}

Transformable* SpriteBatch::clone() const
{
  return( new SpriteBatch(*this) );
}

int32 SpriteBatch::frame() const
{
  return this->sheet_id_;
}

int32 SpriteBatch::frames() const
{
  return this->sprite_sheet.frames();
}

void SpriteBatch::set_frame(int32 id)
{
  this->sheet_id_ = id;

  this->update();
}

void SpriteBatch::draw(IDrawable::RenderTarget& target) const
{
  if( this->frame() >= 0 )
  {
    Sprite::draw(target);
  }
}

void SpriteBatch::draw(IDrawable::RenderTarget& target, const double angle) const
{
  if( this->frame() >= 0 )
  {
    Sprite::draw(target, angle);
  }
}

// Private scope

void SpriteBatch::update()
{
  NOM_ASSERT(this->texture_ != nullptr);

  int scale_factor = 1;

  if( this->texture_ != nullptr ) {
    scale_factor = this->texture_->scale_factor();
  }

  if( this->frame() >= 0 ) {

    Sprite::set_position( this->position() );

    IntRect dims = this->sprite_sheet.dimensions( this->frame() );

    this->offsets.x = dims.x * scale_factor;
    this->offsets.y = dims.y * scale_factor;
    this->offsets.w = dims.w * scale_factor;
    this->offsets.h = dims.h * scale_factor;

    Sprite::set_size( Size2i(offsets.w, offsets.h) );
    if( this->texture_ != nullptr ) {
      this->texture_->set_bounds(this->offsets);
    }
  }
}

} // namespace nom
