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
#include "nomlib/gui/Decorator.hpp"

namespace nom {

Decorator::Decorator( void )
{
  // NOM_LOG_TRACE( NOM );
}

Decorator::~Decorator( void )
{
  // NOM_LOG_TRACE( NOM );
}

IntRect Decorator::margins( void ) const
{
  return this->margins_;
}

IntRect Decorator::frame_bounds( void ) const
{
  return this->frame_bounds_;
}

void Decorator::invalidate( void )
{
  // Flag the decorator dirty; need to rebuild its object state.
  this->drawables_.set_updated( false );

  this->update();
}

ObjectTypeInfo Decorator::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( Decorator );
}

void Decorator::set_bounds( const IntRect& bounds )
{
  Transformable::set_bounds( bounds );

  this->invalidate();
}

void Decorator::set_margins( const IntRect& margins )
{
  this->margins_ = margins;

  this->invalidate();
}

void Decorator::set_frame_bounds( const IntRect& bounds )
{
  this->frame_bounds_ = bounds;
}

} // namespace nom
