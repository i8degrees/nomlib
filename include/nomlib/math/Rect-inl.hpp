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
#ifndef NOMLIB_MATH_RECT_HEADERS
#define NOMLIB_MATH_RECT_HEADERS

#include "nomlib/config.hpp"

namespace nom {

/// \brief Rectangle class container (template version)
template<class T>
class Rect
{
  public:
    Rect ( void ) : left(), top(), right(), bottom() {}

    Rect ( T left, T top, T right, T bottom)  :
      left ( left ), top ( top ), right ( right ), bottom ( bottom ) {}

    template<class Point>
    Rect ( Point p, T width, T height ) :
      left ( p.x ), top ( p.y ), right ( p.x + width ), bottom ( p.y + height ) {}

    /// SDL backwards-compatibility wrappers for nomlib
    ///
    /// Returns a SDL_Rect structure of a nom::Coords object
    static inline SDL_Rect asSDLRect ( const Coords& coords )
    {
      SDL_Rect r;

      r.x = coords.x;
      r.y = coords.y;
      r.w = coords.width;
      r.h = coords.height;

      return r;
    }

  public:
    T left;
    T top;
    T right;
    T bottom;
};

typedef Rect<int32> IntRect;
typedef Rect<float> FloatRect;


} // namespace nom

#endif // NOMLIB_VECTOR4_HEADERS defined
