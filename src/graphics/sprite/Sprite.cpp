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

namespace nom {

Sprite::Sprite() :
  Transformable( Point2i(0, 0), Size2i(0, 0) ),
  texture_(nullptr)
{
// NOM_LOG_TRACE ( NOM );
}

Sprite::~Sprite ( void )
{
// NOM_LOG_TRACE ( NOM );

}

Sprite::Sprite(const Size2i& dims) :
  Transformable( Point2i(0, 0), dims ),
  texture_(nullptr)
{
  // NOM_LOG_TRACE ( NOM );
}

void Sprite::set_position(const Point2i& pos)
{
  Transformable::set_position(pos);

  Sprite::update();
}

IDrawable* Sprite::clone() const
{
  return( new Sprite( *this ) );
}

ObjectTypeInfo Sprite::type() const
{
  return NOM_OBJECT_TYPE_INFO(self_type);
}

SDL_Texture* Sprite::texture() const
{
  NOM_ASSERT(this->texture_ != nullptr);

  if(this->texture_ != nullptr) {
    return this->texture_->texture();
  }

  // Invalid
  return nullptr;
}

void Sprite::set_texture(/*const*/ Texture& tex)
{
  this->texture_ = &tex;
}

void Sprite::draw( RenderTarget& target ) const
{
  NOM_ASSERT(this->texture_ != nullptr);

  if(this->texture_ != nullptr) {
    this->texture_->draw( target.renderer() );
  }
}

void Sprite::draw(RenderTarget& target, const double degrees) const
{
  NOM_ASSERT(this->texture_ != nullptr);

  if(this->texture_ != nullptr) {
    this->texture_->draw(target.renderer(), degrees);
  }
}

// Protected scope

void Sprite::update()
{
  NOM_ASSERT(this->texture_ != nullptr);

  if(this->texture_ != nullptr) {
    this->texture_->set_position( Point2i( this->position().x, this->position().y ) );
  }
}

} // namespace nom
