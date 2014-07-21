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

// Forward declarations
#include "nomlib/system/IObject.hpp"

namespace nom {

UIWidgetEvent::UIWidgetEvent( void ) :
  index_{ -1 }
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
  this->set_id( -1 );
}

UIWidgetEvent::~UIWidgetEvent( void )
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

sint UIWidgetEvent::index( void ) const
{
  return this->index_;
}

const std::string& UIWidgetEvent::text( void ) const
{
  return this->text_;
}

void UIWidgetEvent::set_index( int idx )
{
  this->index_ = idx;
}

void UIWidgetEvent::set_text( const std::string& text )
{
  this->text_ = text;
}

// UIWidgetTreeEvent

UIWidgetTreeEvent::UIWidgetTreeEvent( void ) :
  column_{ -1 },
  data_{ nullptr }
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

UIWidgetTreeEvent::~UIWidgetTreeEvent( void )
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

int UIWidgetTreeEvent::column( void ) const
{
  return this->column_;
}

IObject* UIWidgetTreeEvent::data( void ) const
{
  return this->data_;
}

void UIWidgetTreeEvent::set_column( int col )
{
  this->column_ = col;
}

void UIWidgetTreeEvent::set_data( IObject* obj )
{
  this->data_ = obj;
}

// UIWidgetResizeEvent

UIWidgetResizeEvent::UIWidgetResizeEvent( void )
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

UIWidgetResizeEvent::~UIWidgetResizeEvent( void )
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

const IntRect& UIWidgetResizeEvent::bounds( void ) const
{
  return this->bounds_;
}

void UIWidgetResizeEvent::set_bounds( const IntRect& bounds )
{
  this->bounds_ = bounds;
}

} // namespace nom
