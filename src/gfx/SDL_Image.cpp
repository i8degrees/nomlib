/******************************************************************************

    SDL image class interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Image.hpp"

nom::SDL_Image::SDL_Image ( nom::int32 flags )  : image_buffer ( nullptr, SDL_FreeSurface )
{
NOMLIB_LOG_INFO;

  if ( IMG_Init ( flags ) != flags )
  {
NOMLIB_LOG_ERR ( IMG_GetError() );
  }

  atexit ( IMG_Quit );
}

nom::SDL_Image::SDL_Image ( const SDL_Image& other )  : image_buffer ( static_cast<SDL_Surface*> ( other.image_buffer.get() ), SDL_FreeSurface )
{
NOMLIB_LOG_INFO;
}

nom::SDL_Image::~SDL_Image ( void )
{
NOMLIB_LOG_INFO;

  this->image_buffer.reset(); // ...better safe than sorry!
}

bool nom::SDL_Image::valid ( void ) const
{
  if ( this->image_buffer.get() != nullptr )
    return true;
  else
    return false;
}

std::shared_ptr<void> nom::SDL_Image::loadFromFile ( const std::string& filename )
{
  this->image_buffer = std::shared_ptr<void> ( IMG_Load ( filename.c_str() ), SDL_FreeSurface );

  if ( ! this->valid() )
  {
NOMLIB_LOG_ERR ( IMG_GetError() );
    return nullptr;
  }

  return this->image_buffer;
}

std::shared_ptr<void> nom::SDL_Image::loadFromFile_BMP ( const std::string& filename )
{
  this->image_buffer = std::shared_ptr<void> ( SDL_LoadBMP ( filename.c_str() ), SDL_FreeSurface );

  if ( ! this->valid() )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
    return nullptr;
  }

  return this->image_buffer;
}

bool nom::SDL_Image::saveToFile ( const std::string& filename, void* video_buffer )
{
  if ( SDL_SaveBMP ( static_cast<SDL_Surface*> ( video_buffer ), filename.c_str() ) != 0 )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
    return false;
  }

  return true;
}

const nom::Coords nom::SDL_Image::getSize ( void ) const
{
  SDL_Surface *buffer = static_cast<SDL_Surface*> ( this->image_buffer.get() );

  return nom::Coords ( 0, 0, buffer->w, buffer->h );
}

nom::SDL_Image& nom::SDL_Image::operator = ( const SDL_Image& other )
{
  this->image_buffer = other.image_buffer;

  return *this;
}
