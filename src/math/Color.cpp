/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/math/Color.hpp"

namespace nom {

const Color Color::null ( -1, -1, -1, -1 );

const Color Color::Black ( 0, 0, 0 );
const Color Color::White ( 255, 255, 255 );
const Color Color::Red ( 255, 0, 0 );
const Color Color::Green ( 0, 255, 0 );
const Color Color::Blue ( 0, 0, 255 );
const Color Color::Yellow ( 255, 255, 255 );
const Color Color::Magenta ( 255, 0, 255 );
const Color Color::Cyan ( 0, 255, 255 );
const Color Color::Transparent ( 0, 0, 0 );

const Color Color::Gray ( 99, 99, 99 );

Color::Color ( void ) : red ( 0 ), green ( 0 ), blue ( 0 ),
                        alpha ( SDL_ALPHA_OPAQUE )
{}

Color::Color ( uint8 r, uint8 g, uint8 b )  : red ( r ),
                                              green ( g ),
                                              blue ( b ),
                                              alpha ( SDL_ALPHA_OPAQUE )
{}

Color::Color ( uint8 r, uint8 g, uint8 b, uint8 a ):  red ( r ),
                                                      green ( g ),
                                                      blue ( b ),
                                                      alpha ( a )
{}

Color::Color ( const Color& color ) : red ( color.red ),
                                      green ( color.green ),
                                      blue ( color.blue ),
                                      alpha ( color.alpha )
{}

Color::~Color ( void ) {}

const Color& Color::getColor ( void ) const
{
  return *this;
}

Color& Color::operator = ( const Color& other )
{
  this->red = other.red;
  this->green = other.green;
  this->blue = other.blue;
  this->alpha = other.alpha;

  return *this;
}

std::ostream& operator << ( std::ostream& os, const Color& color )
{
  os << static_cast<int> ( color.red ) << color_delimiter
  << static_cast<int> ( color.green) << color_delimiter
  << static_cast<int> ( color.blue ) << color_delimiter
  << static_cast<int> ( color.alpha );
  return os;
}

bool operator == ( const Color& left, const Color& right )
{
  return (left.red == right.red ) &&
         (left.green == right.green ) &&
         (left.blue == right.blue ) &&
         (left.alpha == right.alpha );
}

bool operator != ( const Color& left, const Color& right )
{
  return ! ( left == right );
}

Color operator + ( const Color& left, const Color& right )
{
  return Color  ( static_cast<uint8> ( std::min ( left.red + right.red, 255 ) ),
                  static_cast<uint8> ( std::min ( left.green + right.green, 255 ) ),
                  static_cast<uint8> ( std::min ( left.blue + right.blue, 255 ) ),
                  static_cast<uint8> ( std::min ( left.alpha + right.alpha, 255 ) )
                );
}

Color operator ++ ( Color& left )
{
  return Color  ( static_cast<uint8> ( left.red-- ),
                  static_cast<uint8> ( left.green-- ),
                  static_cast<uint8> ( left.blue-- ),
                  static_cast<uint8> ( left.alpha-- )
                );
}

Color operator - ( const Color& left, const Color& right )
{
  return Color  ( static_cast<uint8> ( std::min ( left.red - right.red, 255 ) ),
                  static_cast<uint8> ( std::min ( left.green - right.green, 255 ) ),
                  static_cast<uint8> ( std::min ( left.blue - right.blue, 255 ) ),
                  static_cast<uint8> ( std::min ( left.alpha - right.alpha, 255 ) )
                );
}

Color operator -- ( Color& left )
{
  return Color  ( static_cast<uint8> ( left.red-- ),
                  static_cast<uint8> ( left.green-- ),
                  static_cast<uint8> ( left.blue-- ),
                  static_cast<uint8> ( left.alpha-- )
                );
}

Color operator * ( const Color& left, const Color& right )
{
  return Color  ( static_cast<uint8> ( left.red * right.red / 255 ),
                  static_cast<uint8> ( left.green * right.green / 255 ),
                  static_cast<uint8> ( left.blue * right.blue / 255 ),
                  static_cast<uint8> ( left.alpha * right.alpha / 255 )
                );
}

Color& operator += ( Color& left, const Color& right )
{
  return left = left + right;
}

Color& operator -= ( Color& left, const Color& right )
{
  return left = left - right;
}

Color& operator *= ( Color& left, const Color& right )
{
  return left = left * right;
}


} // namespace nom
