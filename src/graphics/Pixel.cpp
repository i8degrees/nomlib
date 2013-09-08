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
#include "nomlib/graphics/Pixel.hpp"

namespace nom {

Pixel::Pixel ( void ) {}

Pixel::~Pixel ( void ) {}

Pixel::Pixel ( const Coords& coords, const Color& color )
{
  this->coords = coords;
  this->color = color;
}

Pixel::Pixel ( int32 x, int32 y, const Color& color )
{
  this->coords = Coords ( x, y );
  this->color = color;
}

void Pixel::Update ( void ) {}

void Pixel::Draw ( void* video_buffer ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( video_buffer );

  // FIXME: Change me to Display::getCanvasColorDepth or such when we get
  // around to implementing our object wrapper for our rendering context --
  // SDL_Surface* screen (nom::Display is our future candidate...)
  switch ( buffer->format->BytesPerPixel )
  {
    default:
    {
NOM_LOG_ERR ( NOM, "Could not determine color depth -- aborting call." );
      return;
    }
    break; // Unsupported color depth?

    case 8:
    {
      uint8* pixels = static_cast<uint8*> ( buffer->pixels );
      uint32 pixel_color = getColorAsInt ( buffer->format, this->color );

      pixels[ ( this->coords.y * buffer->pitch ) + this->coords.x ] = pixel_color;
    }
    break;

    case 2: // 15/16-bit BPP
    {
      uint16* pixels = static_cast<uint16*> ( buffer->pixels );
      uint32 pixel_color = getColorAsInt ( buffer->format, this->color );

      pixels[ ( this->coords.y * buffer->pitch/2 ) + this->coords.x ] = pixel_color;
    }
    break;

    case 3: // 24-bit BPP
    {
      uint8* pixels = static_cast<uint8*> ( buffer->pixels );
      uint32 pixel_color = getColorAsInt ( buffer->format, this->color );

      pixels[ ( this->coords.y * buffer->pitch ) + this->coords.x ] = pixel_color;
      *(pixels + buffer->format->Rshift/8 ) = this->color.red;
      *(pixels + buffer->format->Gshift/8 ) = this->color.green;
      *(pixels + buffer->format->Bshift/8 ) = this->color.blue;
      //*(pixels + buffer->format->Ashift/8 ) = this->color.alpha;
    }
    break;

    case 4: // 32-bit BPP
    {
      uint32* pixels = static_cast<uint32*> ( buffer->pixels );
      uint32 pixel_color = getColorAsInt ( buffer->format, this->color );

      pixels[ ( this->coords.y * buffer->pitch/4 ) + this->coords.x ] = pixel_color;
    }
    break;
  } // end switch
}


} // namespace nom
