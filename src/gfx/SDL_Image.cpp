/******************************************************************************

    SDL image class interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Image.hpp"

nom::SDL_Image::SDL_Image ( nom::int32 flags )  : image_buffer ( nullptr )
{
NOMLIB_LOG_INFO;

  if ( IMG_Init ( flags ) != flags )
  {
NOMLIB_LOG_ERR ( IMG_GetError() );
  }

  atexit ( IMG_Quit );
}

nom::SDL_Image::~SDL_Image ( void )
{
NOMLIB_LOG_INFO;

  // We do not free the surface here because we pass ownership
  // (and thus, responsibility) to the calling class
  this->image_buffer = nullptr; // ...better safe than sorry!
}

// This is important to return precisely as a SDL_Surface for it changes nullptr
// result potentially
void* nom::SDL_Image::get ( void ) const
{
  return static_cast<SDL_Surface*> ( this->image_buffer );
}

bool nom::SDL_Image::valid ( void ) const
{
  if ( static_cast<SDL_Surface*> ( this->image_buffer ) != nullptr )
    return true;
  else
    return false;
}

bool nom::SDL_Image::loadFromFile ( const std::string& filename )
{
  this->image_buffer = IMG_Load ( filename.c_str() );

  if ( ! this->valid() )
  {
NOMLIB_LOG_ERR ( IMG_GetError() );
    return false;
  }

  return true;
}

bool nom::SDL_Image::loadFromFile_BMP ( const std::string& filename )
{
  this->image_buffer = SDL_LoadBMP ( filename.c_str() );

  if ( ! this->valid() )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
    return false;
  }

  return true;
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
  SDL_Surface* buffer = ( SDL_Surface* ) image_buffer;

  return nom::Coords ( 0, 0, buffer->w, buffer->h );
}
