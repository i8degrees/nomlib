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
#include "nomlib/gui/UIEvent.hpp"

namespace nom {

UIEvent::UIEvent( void ) :
  type_{ 0 },
  id_{ -1 }
{
  // NOM_LOG_TRACE( NOM );
}

UIEvent::~UIEvent( void )
{
  // NOM_LOG_TRACE( NOM );
}

UIEvent::UIEvent( uint32 type ) :
  type_{ type }
{
  // NOM_LOG_TRACE( NOM );
}

UIEvent* UIEvent::clone( void ) const
{
  return new UIEvent( *this );
}

uint32 UIEvent::type( void ) const
{
  return this->type_;
}

const Event& UIEvent::event( void ) const
{
  return this->event_;
}

int64 UIEvent::id( void ) const
{
  return this->id_;
}

void UIEvent::set_type( uint32 type )
{
  this->type_ = type;
}

void UIEvent::set_event( const Event& ev )
{
  this->event_ = ev;
}

void UIEvent::set_id( int64 id )
{
  this->id_ = id;
}

bool UIEvent::operator <( const self_type& rhs ) const
{
  return( this->type() < rhs.type() );
}

bool UIEvent::operator ==( const self_type& rhs ) const
{
  return( this->type() == rhs.type() );
}

} // namespace nom
