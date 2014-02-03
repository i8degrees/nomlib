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
#include "nomlib/gui/UILayoutItem.hpp"

namespace nom {

const Size2i UILayoutItem::layout_max_ = Size2i( 16777215, 16777215 );

UILayoutItem::UILayoutItem( void ) :
  alignment_{ Anchor::TopLeft }
{
  // NOM_LOG_TRACE( NOM );
}

UILayoutItem::~UILayoutItem( void )
{
  // NOM_LOG_TRACE( NOM );
}

UILayoutItem::UILayoutItem( enum Anchor align ) :
  alignment_{ align }
{
  // NOM_LOG_TRACE( NOM );
}

void UILayoutItem::invalidate( void )
{
  NOM_STUBBED( NOM );
}

enum Anchor UILayoutItem::alignment( void ) const
{
  return this->alignment_;
}

UILayout* UILayoutItem::layout( void )
{
  return nullptr;
}

void UILayoutItem::set_alignment( enum Anchor align )
{
  this->alignment_ = align;
}

UISpacerItem* UILayoutItem::spacer_item( void )
{
  return nullptr;
}

UIWidget* UILayoutItem::widget( void ) const
{
  return nullptr;
}

} // namespace nom
