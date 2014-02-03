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
#include "nomlib/gui/UIWidgetEvent.hpp"

namespace nom {

UIWidgetEvent::UIWidgetEvent( void ) :
  index_{ -1 },
  id_{ -1 }
{
  // NOM_LOG_TRACE( NOM );
}

UIWidgetEvent::~UIWidgetEvent( void )
{
  // NOM_LOG_TRACE( NOM );
}

UIWidgetEvent::UIWidgetEvent( const self_type& copy )
{
  // NOM_LOG_TRACE( NOM );

  this->set_index( copy.index() );
  this->set_text( copy.text() );
  this->set_id( copy.id() );
}

UIWidgetEvent::self_type& UIWidgetEvent::operator =( const self_type& other )
{
  this->set_index( other.index() );
  this->set_text( other.text() );
  this->set_id( other.id() );

  return *this;
}

UIWidgetEvent::UIWidgetEvent( const Event& ev )
{
  // NOM_LOG_TRACE( NOM );

  this->set_event( ev );
  this->set_index( -1 );
  this->set_id( -1 );
}

UIWidgetEvent::UIWidgetEvent( sint index, const std::string& text, const Event& ev )
{
  // NOM_LOG_TRACE( NOM );

  this->set_index( index );
  this->set_text( text );
  this->set_event( ev );
  this->set_id( -1 );
}

UIWidgetEvent::UIWidgetEvent  (
                                sint index,
                                const std::string& text,
                                const Event& ev,
                                int64 id
                              )
{
  // NOM_LOG_TRACE( NOM );

  this->set_index( index );
  this->set_text( text );
  this->set_event( ev );
  this->set_id( id );
}

UIWidgetEvent::RawPtr UIWidgetEvent::get( void )
{
  return this;
}

bool UIWidgetEvent::valid( void ) const
{
  NOM_STUBBED( NOM );

  return false;
}

sint UIWidgetEvent::index( void ) const
{
  return this->index_;
}

const std::string& UIWidgetEvent::text( void ) const
{
  return this->text_;
}

const Event& UIWidgetEvent::event( void ) const
{
  return this->event_;
}

int64 UIWidgetEvent::id( void ) const
{
  return this->id_;
}

void UIWidgetEvent::set_index( int idx )
{
  this->index_ = idx;
}

void UIWidgetEvent::set_text( const std::string& text )
{
  this->text_ = text;
}

void UIWidgetEvent::set_event( const Event& ev )
{
  this->event_ = ev;
}

void UIWidgetEvent::set_id( int64 id )
{
  this->id_ = id;
}

} // namespace nom
