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
#include "nomlib/math/Transformable.hpp"

namespace nom {

Transformable::Transformable ( void )  :  position_ ( 0, 0, 0, 0 ),
  color_ ( 0, 0, 0, ALPHA_OPAQUE ) {}

Transformable::~Transformable ( void ) {}

Transformable::Transformable ( const Coords& coords ):  position_ ( coords ) {}

Transformable::Transformable ( const Coords& coords, const Color4u& color ) :
  position_ ( coords ), color_ ( color ) {}

const Coords Transformable::position ( void ) const
{
  return Coords ( this->position_.x, this->position_.y );
}

const Coords Transformable::getSize ( void ) const
{
  return Coords ( this->position_.width, this->position_.height );
}

void Transformable::set_position ( int32 x, int32 y )
{
  this->position_.setPosition ( x, y );
}

void Transformable::set_position ( int32 x, int32 y, int32 width, int32 height )
{
  this->position_ = Coords ( x, y, width, height );
}

void Transformable::set_position ( const Coords& coords )
{
  this->set_position ( coords.x, coords.y, coords.width, coords.height );
}

void Transformable::setSize ( int32 width, int32 height )
{
  // FIXME
  this->position_.setSize ( width, height );
}

void Transformable::setSize ( const Coords& size )
{
  this->setSize ( size.width, size.height );
}

void Transformable::move ( uint32 x, uint32 y )
{
  this->position_.x += x;
  this->position_.y += y;
}

void Transformable::move ( const Coords& offsets )
{
  this->position_.x += offsets.x;
  this->position_.y += offsets.y;
}

const Color4u& Transformable::color ( void ) const
{
  return this->color_;
}

void Transformable::set_color ( uint8 red, uint8 green, uint8 blue, uint8 alpha )
{
  this->color_ = Color4u ( red, green, blue, alpha );
}

void Transformable::set_color ( const Color4u& color )
{
  this->set_color ( color.red, color.green, color.blue, color.alpha );
}

} // namespace nom
