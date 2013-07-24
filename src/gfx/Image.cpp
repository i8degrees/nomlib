/******************************************************************************

    SDL image class interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/gfx/Image.hpp"

namespace nom {

Image::Image ( int32 flags )  : image_buffer ( nullptr, nom::priv::Canvas_FreeSurface )
{
NOMLIB_LOG_INFO;

  if ( IMG_Init ( flags ) != flags )
  {
NOMLIB_LOG_ERR ( IMG_GetError() );
  }

  atexit ( IMG_Quit );
}

Image::Image ( const Image& other )  : image_buffer ( static_cast<SDL_Surface*> ( other.image_buffer.get() ), nom::priv::Canvas_FreeSurface )
{
NOMLIB_LOG_INFO;
}

Image::~Image ( void )
{
NOMLIB_LOG_INFO;

  this->image_buffer.reset(); // ...better safe than sorry!
}

bool Image::valid ( void ) const
{
  if ( this->image_buffer.get() != nullptr )
    return true;
  else
    return false;
}

std::shared_ptr<void> Image::loadFromFile ( const std::string& filename )
{
  this->image_buffer = std::shared_ptr<void> ( IMG_Load ( filename.c_str() ), nom::priv::Canvas_FreeSurface );

  if ( ! this->valid() )
  {
NOMLIB_LOG_ERR ( IMG_GetError() );
    return nullptr;
  }

  return this->image_buffer;
}

std::shared_ptr<void> Image::loadFromFile_BMP ( const std::string& filename )
{
  this->image_buffer = std::shared_ptr<void> ( SDL_LoadBMP ( filename.c_str() ), nom::priv::Canvas_FreeSurface );

  if ( ! this->valid() )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
    return nullptr;
  }

  return this->image_buffer;
}

bool Image::saveToFile ( const std::string& filename, void* video_buffer )
{
  if ( SDL_SaveBMP ( static_cast<SDL_Surface*> ( video_buffer ), filename.c_str() ) != 0 )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
    return false;
  }

  return true;
}

const Coords Image::getSize ( void ) const
{
  SDL_Surface *buffer = static_cast<SDL_Surface*> ( this->image_buffer.get() );

  return Coords ( 0, 0, buffer->w, buffer->h );
}

Image& Image::operator = ( const Image& other )
{
  this->image_buffer = other.image_buffer;

  return *this;
}


} // namespace nom
