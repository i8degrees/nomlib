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
#include "nomlib/system/events/UserEvent.hpp"

namespace nom {

UserEvent::UserEvent( void ) :
  type( SDL_USEREVENT ),
  code( 0 ),
  data1( nullptr ),
  data2( nullptr ),
  window_id( 0 ),
  timestamp( 0 )
{
  // NOM_LOG_TRACE( NOM );
}

UserEvent::~UserEvent( void )
{
  // NOM_LOG_TRACE( NOM );
}

UserEvent::UserEvent( int32 code, void* data1, void* data2, uint32 window_id ) :
  type( SDL_USEREVENT ),
  code( code ),
  data1( data1 ),
  data2( data2 ),
  window_id( window_id ),
  timestamp{ ticks() }
{
  // NOM_LOG_TRACE( NOM );
}

EventCallback* UserEvent::get_callback( void ) const
{
  return NOM_SCAST( EventCallback*, data2 );
}

} // namespace nom
