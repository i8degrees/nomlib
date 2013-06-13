/******************************************************************************
    SDL_Pixel.cpp

  SDL Point (Pixel) Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Pixel.hpp"

nom::Pixel::Pixel ( void )
{
  this->setPosition ( 0, 0 );
  this->setColor ( nom::Color ( 0, 0, 0, 255 ) );
}

nom::Pixel::~Pixel ( void )
{
  // ...
}

nom::Pixel::Pixel ( const nom::Coords& coords, const nom::Color& color )
{
  this->setPosition ( coords );
  this->setColor ( color );
}

nom::Pixel::Pixel ( int32_t x, int32_t y, const nom::Color& color )
{
  this->setPosition ( x, y );
  this->setColor ( color );
}

void nom::Pixel::Update ( void )
{
  // Stub
}

// 32-bit bpp
void nom::Pixel::Draw ( void* video_buffer ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( video_buffer );

  u_char* pixel = ( u_char* ) buffer->pixels;

  pixel += ( this->coords.y * buffer->pitch ) + ( this->coords.x * sizeof ( uint32_t ) );

  *( ( uint32_t* ) pixel ) = this->color.getColorAsInt ( buffer->format );
}
