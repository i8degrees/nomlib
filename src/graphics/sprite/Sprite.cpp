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

Sprite::Sprite ( void ) : state_ ( 0 ), scale_factor ( 1 )
{
NOM_LOG_TRACE ( NOM );
}

Sprite::~Sprite ( void )
{
NOM_LOG_TRACE ( NOM );
}

Sprite::Sprite ( int32 width, int32 height )  :
  Transformable { Coords ( 0, 0, width, height ) }, state_ ( 0 ),
  scale_factor ( 1 )

{
NOM_LOG_TRACE ( NOM );
}

Sprite& Sprite::operator = ( const Sprite& other )
{
  this->sprite_ = other.sprite();
  this->set_position ( other.position().x, other.position().y );
  this->set_state ( other.state() );
  this->scale_factor = other.scale_factor;

  return *this;
}

const Texture& Sprite::sprite ( void ) const
{
  return this->sprite_;
}

const Point2i Sprite::size ( void ) const
{
  return Point2i ( this->sprite_.width(), this->sprite_.height() );
}

const Point2i Sprite::position ( void ) const
{
  // FIXME
  return Point2i ( this->position_.x, this->position_.y );
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
                    const std::string& filename, const Color4u& colorkey,
                    bool use_cache
                  )
{
  this->sprite_.load ( filename, SDL_TEXTUREACCESS_STREAMING, use_cache );

  if ( this->sprite_.valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not load sprite image file: " + filename );
    return false;
  }

  this->setSize ( this->sprite_.width(), this->sprite_.height() );

  this->sprite_.set_colorkey ( colorkey );

  return true;
}

void Sprite::update ( void )
{
  // FIXME
  this->sprite_.set_position ( Point2i ( this->position_.x, this->position_.y ) );
}

void Sprite::draw ( RenderTarget target ) const
{
NOM_ASSERT ( this->sprite_.valid() );

  this->sprite_.draw ( target.renderer() );
}

void Sprite::draw ( RenderTarget target, const double degrees ) const
{
  this->sprite_.draw ( target.renderer(), degrees );
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

  this->scale_factor = this->sprite_.getResizeScaleFactor ( scaling_algorithm );
  this->update();

  return true;
}

} // namespace nom
