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
#include "nomlib/graphics/shapes/Line.hpp"

namespace nom {

Line::Line ( void ) {}
Line::~Line ( void ) {}

Line::Line ( const Coords& coords, const Color4u& color )
{
  this->coords = coords;
  this->color = color;
}

Line::Line ( int32 x, int32 y, int32 width, int32 height, const Color4u& color )
{
  this->coords = Coords ( x, y, width, height );
  this->color = color;
}

void Line::update ( void ) { /* NO-OP */ }

void Line::draw ( RenderTarget target ) const
{
  if ( SDL_SetRenderDrawColor ( target.renderer(), this->color.r, this->color.g, this->color.b, this->color.a ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return;
  }

  if ( SDL_RenderDrawLine ( target.renderer(), this->coords.x, this->coords.y, this->coords.w, this->coords.h ) != 0 )
  {
NOM_LOG_ERR ( NOM, "Could not render 3D SDL line: " + std::string (SDL_GetError()) );
    return;
  }
}


} // namespace nom
