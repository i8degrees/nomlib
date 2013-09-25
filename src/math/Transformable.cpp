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
#include "nomlib/math/Transformable.hpp"

namespace nom {

Transformable::Transformable ( void )  :  coords ( 0, 0, 0, 0 ),
                                          color ( 0, 0, 0, 255 )
{
  // Nothing to initialize
}

Transformable::Transformable ( const Coords& coords ):  coords ( coords ) {}

Transformable::Transformable ( const Coords& coords, const Color& color )
{
  this->coords = coords;
  this->color = color;
}

Transformable::~Transformable ( void ) {}

const Coords Transformable::getPosition ( void ) const
{
  return Coords ( this->coords.x, this->coords.y );
}

const Coords Transformable::getSize ( void ) const
{
  return Coords ( this->coords.width, this->coords.height );
}

// Variant #1 setter for position coordinates
void Transformable::setPosition ( int32 x, int32 y )
{
  this->coords.setPosition ( x, y );
}

// Variant #2 setter for position coordinates
void Transformable::setPosition ( int32 x, int32 y, int32 width, int32 height )
{
  this->coords = Coords ( x, y, width, height );
}

// Variant #3 setter for position coordinates; do we have enough options yet???
void Transformable::setPosition ( const Coords& coords )
{
  this->setPosition ( coords.x, coords.y, coords.width, coords.height );
}

void Transformable::setSize ( int32 width, int32 height )
{
  this->coords.setSize ( width, height );
}

void Transformable::setSize ( const Coords& size )
{
  this->setSize ( size.width, size.height );
}

void Transformable::move ( uint32 x, uint32 y )
{
  this->coords.x += x;
  this->coords.y += y;
}

void Transformable::move ( const Coords& offsets )
{
  this->coords.x += offsets.x;
  this->coords.y += offsets.y;
}

const Color& Transformable::getColor ( void ) const
{
  return this->color;
}

void Transformable::setColor ( uint8 red, uint8 green, uint8 blue, uint8 alpha )
{
  this->color = Color ( red, green, blue, alpha );
}

void Transformable::setColor ( const Color& color )
{
  this->setColor ( color.red, color.green, color.blue, color.alpha );
}


} // namespace nom
