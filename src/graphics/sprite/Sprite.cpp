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

Sprite::Sprite ( void ) :
  Transformable { Point2i(0, 0), Size2i(0, 0) },
  state_ ( 0 ),
  scale_factor ( 1 )
{
// NOM_LOG_TRACE ( NOM );
}

Sprite::~Sprite ( void )
{
// NOM_LOG_TRACE ( NOM );
}

Sprite::Sprite ( int32 width, int32 height )  :
  Transformable { Point2i(0, 0), Size2i(width, height) },
  state_ ( 0 ),
  scale_factor ( 1 )

{
// NOM_LOG_TRACE ( NOM );
}

Sprite& Sprite::operator = ( const Sprite& other )
{
  this->sprite_ = other.sprite_;
  this->set_position ( other.position() );
  this->set_state ( other.state() );
  this->scale_factor = other.scale_factor;

  return *this;
}

IDrawable::raw_ptr Sprite::clone( void ) const
{
  return Sprite::raw_ptr( new Sprite( *this ) );
}

ObjectTypeInfo Sprite::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

SDL_TEXTURE::RawPtr Sprite::texture ( void ) const
{
  return this->sprite_.texture();
}

const Size2i Sprite::size ( void ) const
{
  // FIXME
  return Size2i ( this->sprite_.width(), this->sprite_.height() );
}

uint32 Sprite::state ( void ) const
{
  return this->state_;
}

void Sprite::set_state ( uint32 state )
{
  this->state_ = state;
}

bool Sprite::load (
                    const std::string& filename,
                    bool use_cache,
                    enum Texture::Access type
                  )
{
  if ( this->sprite_.load ( filename, use_cache, type ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not load sprite image file: " + filename );
    return false;
  }

  this->set_size ( Size2i ( this->sprite_.width(), this->sprite_.height() ) );

  this->update();

  return true;
}

void Sprite::update ( void )
{
  // FIXME
  this->sprite_.set_position ( Point2i ( this->position().x, this->position().y ) );
}

void Sprite::draw ( RenderTarget& target ) const
{
  NOM_ASSERT ( this->sprite_.valid() );

  if ( this->sprite_.valid() )
  {
    // FIXME: this->update();
    this->sprite_.draw ( target.renderer() );
  }
}

void Sprite::draw ( RenderTarget& target, const double degrees ) const
{
  NOM_ASSERT ( this->sprite_.valid() );

  if ( this->sprite_.valid() )
  {
    // FIXME: this->update();
    this->sprite_.draw ( target.renderer(), degrees );
  }
}

bool Sprite::resize ( enum Texture::ResizeAlgorithm scaling_algorithm )
{
  if ( this->sprite_.valid() == false )
  {
NOM_LOG_ERR ( NOM, "Video surface is invalid." );
    return false;
  }

  if ( this->sprite_.resize ( scaling_algorithm ) == false )
  {
NOM_LOG_ERR ( NOM, "Failed to resize the video surface." );
    return false;
  }

  this->scale_factor = this->sprite_.scale_factor ( scaling_algorithm );
  this->update();

  return true;
}

} // namespace nom
