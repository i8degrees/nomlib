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
#include "nomlib/graphics/shapes/Shape.hpp"

namespace nom {

Shape::Shape ( void ) :
  color_ { Color4i::Black }
{
  //NOM_LOG_TRACE ( NOM );
}

Shape::~Shape ( void )
{
  //NOM_LOG_TRACE ( NOM );
}

Shape::Shape ( const Shape& copy )  :
  Transformable { copy.position(), copy.size() }, // Our base class
  color_ { copy.color() }
{
  //NOM_LOG_TRACE(NOM);
}

/*
Shape& Shape::operator = ( const Shape& other )
{
  this->color_ = other.color();

  return *this;
}
*/

Shape::Shape ( const Point2i& pos, const Size2i& size, const Color4i& color ) :
  Transformable { pos, size }, // Our base class
  color_ ( color )
{
  //NOM_LOG_TRACE ( NOM );
}

const Color4i& Shape::color ( void ) const
{
  return this->color_;
}

} // namespace nom
