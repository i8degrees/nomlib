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
#include "nomlib/graphics/Glyph.hpp"

namespace nom {

Glyph::Glyph ( void )  :
  advance ( 0 )
{
  //NOM_LOG_TRACE(NOM);
}

Glyph::~Glyph ( void )
{
  //NOM_LOG_TRACE(NOM);
}

std::ostream& operator << ( std::ostream& os, const Glyph& glyph )
{
  os
  << glyph.bounds.x
  << GLYPH_DELIMITER
  << glyph.bounds.y
  << GLYPH_DELIMITER
  << glyph.bounds.w
  << GLYPH_DELIMITER
  << glyph.bounds.h
  << GLYPH_DELIMITER
  << glyph.advance;

  return os;
}

bool operator == ( const Glyph& lhs, const Glyph& rhs )
{
  return  ( lhs.bounds.x == rhs.bounds.x )  &&  ( lhs.bounds.w == rhs.bounds.w )
                                            &&
          ( lhs.bounds.y == rhs.bounds.y )  &&  ( lhs.bounds.h == rhs.bounds.h );
}

bool operator < ( const Glyph& lhs, const Glyph& rhs )
{
  if ( lhs.bounds.w == rhs.bounds.w )
  {
    return lhs.bounds.x < rhs.bounds.x;
  }
  else
  {
    return lhs.bounds.w < rhs.bounds.w;
  }

  return lhs.bounds.w < rhs.bounds.w;
}

bool operator > ( const Glyph& lhs, const Glyph& rhs )
{
  return rhs < lhs;
}

} // namespace nom
