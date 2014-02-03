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
#include "nomlib/gui/UIEventHandler.hpp"

namespace nom {

UIEventHandler::UIEventHandler( void )
{
  // NOM_LOG_TRACE( NOM );

  this->set_dispatcher( new UIEventDispatcher() );
}

UIEventHandler::~UIEventHandler( void )
{
  // NOM_LOG_TRACE( NOM );
}

UIEventHandler::UIEventHandler( const self_type& rhs )
{
  // NOM_LOG_TRACE( NOM );

  this->set_dispatcher( rhs.dispatcher() );
}

UIEventHandler::self_type& UIEventHandler::operator =( const self_type& rhs )
{
  // NOM_LOG_TRACE( NOM );

  this->set_dispatcher( rhs.dispatcher() );

  return *this;
}

UIEventHandler::UIEventHandler( const UIEventDispatcher::raw_ptr dispatcher )
{
  // NOM_LOG_TRACE( NOM );

  this->set_dispatcher( dispatcher );
}

bool UIEventHandler::valid( void ) const
{
  if( this->dispatcher() != nullptr ) return true;

  return false;
}

UIEventDispatcher::raw_ptr UIEventHandler::dispatcher( void ) const
{
  // Sanity check; ensure that we are not an invalid object state (NULL).
  NOM_ASSERT( this->dispatcher_.get() != nullptr );

  return this->dispatcher_.get();
}

bool UIEventHandler::register_event_listener( const UIEventDispatcher::event_type& ev, const UIEventDispatcher::callback_type& observer )
{
  return this->dispatcher()->register_event_listener( ev, observer );
}

bool UIEventHandler::remove_event_listener( const UIEventDispatcher::event_type& key )
{
  return this->dispatcher()->remove_event_listener( key );
}

uint32 UIEventHandler::count( void ) const
{
  return this->dispatcher()->size();
}

bool UIEventHandler::emit( const UIEventDispatcher::event_type& ev, UIWidgetEvent& data ) const
{
  return this->dispatcher()->emit( ev, data );
}

bool UIEventHandler::find( const UIEventDispatcher::event_type& key ) const
{
  return this->dispatcher()->find( key );
}

bool UIEventHandler::process_event( const nom::Event& ev )
{
  // No processed events
  return false;
}

// Private scope

void UIEventHandler::set_dispatcher( const UIEventDispatcher::raw_ptr dispatcher )
{
  this->dispatcher_.reset( dispatcher );
}

} // namespace nom
