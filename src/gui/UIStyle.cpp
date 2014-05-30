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
#include "nomlib/gui/UIStyle.hpp"

// Private headers
#include "nomlib/system/PlatformSettings.hpp"
#include "nomlib/graphics/Text.hpp"

namespace nom {

UIStyle::UIStyle( void ) :
  font_size_{ nom::DEFAULT_FONT_SIZE },
  font_color_{ Color4i::White },
  font_style_{ Text::Style::Normal }
{
  NOM_LOG_TRACE( NOM );
}

UIStyle::~UIStyle( void )
{
  NOM_LOG_TRACE( NOM );
}

int UIStyle::font_size( void ) const
{
  return this->font_size_;
}

const Color4i& UIStyle::font_color( void ) const
{
  return this->font_color_;
}

uint32 UIStyle::font_style( void ) const
{
  return this->font_style_;
}

void UIStyle::set_font_size( int point_size )
{
  this->font_size_ = point_size;
}

void UIStyle::set_font_color( const Color4i& color )
{
  this->font_color_ = color;
}

void UIStyle::set_font_style( uint32 style )
{
  this->font_style_ = style;
}

} // namespace nom
