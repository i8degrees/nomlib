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
#include "nomlib/gui/UIEventCallback.hpp"

namespace nom {

UIEventCallback::UIEventCallback( void ) :
  method_{ nullptr }
{
  // NOM_LOG_TRACE( NOM );
}

UIEventCallback::~UIEventCallback( void )
{
  // NOM_LOG_TRACE( NOM );
}

UIEventCallback::UIEventCallback( const ValueType& callback ) :
  method_( callback )
{
  // NOM_LOG_TRACE( NOM );
}

UIEventCallback::UIEventCallback( const SelfType& copy )
{
  this->method_ = copy.method();
}

UIEventCallback::SelfType& UIEventCallback::operator =( const SelfType& other )
{
  this->method_ = other.method();

  return *this;
}

UIEventCallback::RawPtr UIEventCallback::get( void )
{
  return this;
}

bool UIEventCallback::valid( void ) const
{
  if( this->method_ != nullptr ) return true;

  return false;
}

const UIEventCallback::ValueType& UIEventCallback::method( void ) const
{
  return this->method_;
}

void UIEventCallback::operator() ( event_type& ev ) const
{
  this->method_( ev );
}

void UIEventCallback::execute( event_type& ev )
{
  this->method_( ev );
}

} // namespace nom
