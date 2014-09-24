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
#include "nomlib/math/Transformable.hpp"

namespace nom {

Transformable::Transformable( void )  :
  position_( Point2i::null ),
  size_( Size2i::null )
{
  //NOM_LOG_TRACE(NOM);
}

Transformable::~Transformable( void )
{
  //NOM_LOG_TRACE(NOM);
}

Transformable::Transformable( const Point2i& pos, const Size2i& size ) :
  position_( pos ),
  size_( size )
{
  //NOM_LOG_TRACE(NOM);
}

Transformable::Transformable( const IntRect& bounds ) :
  position_{ bounds.x, bounds.y },
  size_{ bounds.w, bounds.h }
{
  //NOM_LOG_TRACE(NOM);
}

ObjectTypeInfo Transformable::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

const Point2i& Transformable::position( void ) const
{
  return this->position_;
}

const Size2i& Transformable::size( void ) const
{
  return this->size_;
}

void Transformable::set_position( const Point2i& pos )
{
  this->position_ = pos;
}

void Transformable::set_size( const Size2i& size )
{
  this->size_ = size;
}

void Transformable::set_bounds( const IntRect& bounds )
{
  this->position_ = bounds.position();
  this->size_ = bounds.size();
}

void Transformable::set_position( int x, int y )
{
  this->set_position( Point2i( x, y ) );
}

void Transformable::set_size( int w, int h )
{
  this->set_size( Size2i( w, h ) );
}

void Transformable::move( int x, int y )
{
  this->position_.x += x;
  this->position_.y += y;
}

} // namespace nom
