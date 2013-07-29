/******************************************************************************

    SDL Point (Pixel) Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/gfx/Pixel.hpp"

namespace nom {

Pixel::Pixel ( void )
{
  this->setPosition ( 0, 0 );
  this->setColor ( Color ( 0, 0, 0, 255 ) );
}

Pixel::~Pixel ( void )
{
  // Do nothing
}

Pixel::Pixel ( const Coords& coords, const Color& color )
{
  this->setPosition ( coords );
  this->setColor ( color );
}

Pixel::Pixel ( int32 x, int32 y, const Color& color )
{
  this->setPosition ( x, y );
  this->setColor ( color );
}

void Pixel::Update ( void )
{
  // Do nothing
}

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
NOM_LOG_ERR ( "Could not determine color depth -- aborting call." );
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
