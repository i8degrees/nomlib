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
#include "nomlib/graphics/Rectangle.hpp"

namespace nom {

Rectangle::Rectangle ( void )
{
  this->coords = Coords ( 0, 0, 0, 0 );
  this->color = Color ( 0, 0, 0 );
}

Rectangle::Rectangle ( const Rectangle& rect )
{
  this->coords = rect.coords;
  this->color = rect.color;
}

Rectangle::Rectangle ( const Coords& coords, const Color& color )
{
  this->coords = coords;
  this->color = color;
}

Rectangle::~Rectangle ( void )
{
  // ...
}

void Rectangle::Update ( void )
{
  // Stub
}

void Rectangle::Draw ( void* video_buffer ) const
{
  SDL_Rect rectangle = getSDL_Rect ( this->coords );

  uint32 rectangle_color = 0;
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( video_buffer );

  rectangle_color = getColorAsInt ( buffer->format, this->color );

  if ( SDL_FillRect ( buffer, &rectangle, rectangle_color ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return;
  }

#ifndef NOM_PLATFORM_LINUX
  // Destruct as soon as we are done to free memory
  // that could otherwise be held too long before
  // cleaned up, resulting in slow, but steady climb
  // in memory usage
  Rectangle::~Rectangle();
#endif
  //SDL_FreeSurface ( buffer );
}


} // namespace nom
