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
#include "nomlib/system/events/EventDispatcher.hpp"

namespace nom {

EventDispatcher::EventDispatcher( void )
{
  // NOM_LOG_TRACE( NOM );
}

EventDispatcher::~EventDispatcher( void )
{
  // NOM_LOG_TRACE( NOM );
}

bool EventDispatcher::dispatch( const UserEvent& ev )
{
  if( this->push_event( ev ) == false )
  {
    NOM_LOG_ERR( NOM, "ERROR: Could not dispatch event." );
    return false;
  }

  return true;
}

bool EventDispatcher::push_event( const UserEvent& ev )
{
  // uint32 event_type = this->register_events( 1 );

  // if( event_type == ( (uint32) - 1 ) ) // Error
  // {
    // NOM_LOG_ERR( NOM, "ERROR: Out of room for user-defined events." );
    // return false;
  // }

  SDL_Event event;
  SDL_zero( event ); // Initialize our event structure

  // event.type = event_type;
  // event.user.type = event_type;
  event.type = SDL_USEREVENT;
  event.user.type = SDL_USEREVENT;
  event.user.code = ev.code;
  event.user.data1 = ev.data1;
  event.user.data2 = ev.data2;
  event.user.timestamp = ev.timestamp;
  event.user.windowID = ev.window_id;

  if( SDL_PushEvent( &event ) != 1 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

uint32 EventDispatcher::register_events( int num_events )
{
  return SDL_RegisterEvents( num_events );
}

} // namespace nom
