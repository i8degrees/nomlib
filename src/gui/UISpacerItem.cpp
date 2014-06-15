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
#include "nomlib/gui/UISpacerItem.hpp"

namespace nom {

UISpacerItem::UISpacerItem( void )
{
  // NOM_LOG_TRACE( NOM );
}

UISpacerItem::~UISpacerItem( void )
{
  // NOM_LOG_TRACE( NOM );
}

UISpacerItem::UISpacerItem( const Size2i& size, uint32 horiz_policy, uint32 vert_policy )
{
  this->set_size( size, horiz_policy, vert_policy );
}

bool UISpacerItem::empty( void ) const
{
  return true;
}

enum Orientations UISpacerItem::directions( void ) const
{
  return this->policy_.directions();
}

IntRect UISpacerItem::bounds( void ) const
{
  return this->bounds_;
}

Size2i UISpacerItem::maximum_size( void ) const
{
  // TODO: Size policy logic

  return UILayoutItem::layout_max_;
}

Size2i UISpacerItem::minimum_size( void ) const
{
  // TODO: Size policy logic

  // return Size2i( 0, 0 );

  return this->size_;
}

void UISpacerItem::set_bounds( const IntRect& rect )
{
  this->bounds_ = rect;
}

Size2i UISpacerItem::size_hint( void ) const
{
  return this->size_;
}

UISpacerItem::raw_ptr UISpacerItem::spacer_item( void )
{
  return this;
}

Size2i UISpacerItem::size( void ) const
{
  return this->size_;
}

void UISpacerItem::set_size( const Size2i& size, uint32 horiz_policy, uint32 vert_policy )
{
  this->size_ = size;

  this->policy_.set_horizontal_policy( horiz_policy );
  this->policy_.set_vertical_policy( vert_policy );
}

} // namespace nom
