/******************************************************************************
    SDL_Image.cpp

    SDL image class interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Image.hpp"

nom::SDL_Image::SDL_Image ( uint32_t flags )  : image_buffer ( nullptr )
{
  #ifdef DEBUG_SDL_IMAGE_OBJ
    std::cout << "SDL_Image::SDL_Image(): Hello, world!" << std::endl << std::endl;
  #endif

  if ( IMG_Init ( flags ) != flags )
  {
    #ifdef DEBUG_SDL_IMAGE_OBJ
      std::cout << "ERR in SDL_Image::SDL_Image() at IMG_Init(): " << IMG_GetError() << std::endl;
    #endif
  }
}

nom::SDL_Image::~SDL_Image ( void )
{
  #ifdef DEBUG_SDL_IMAGE_OBJ
    std::cout << "SDL_Image::~SDL_Image(): Goodbye cruel world!" << std::endl << std::endl;
  #endif

  this->image_buffer = nullptr; // ...better safe than sorry?

  IMG_Quit();
}

// This is important to return precisely as a SDL_Surface for it changes nullptr
// result potentially
void* nom::SDL_Image::get ( void ) const
{
  return static_cast<SDL_Surface*> ( this->image_buffer );
}

bool nom::SDL_Image::loadFromFile ( const std::string& filename )
{
  this->image_buffer = IMG_Load ( filename.c_str() );

  if ( static_cast<SDL_Surface*> ( this->image_buffer ) == nullptr )
  {
    #ifdef DEBUG_SDL_IMAGE
      std::cout << "ERR in nom::SDL_Image::loadFromFile() at IMG_Load(): " << IMG_GetError() << std::endl << std::endl;
    #endif
    return false;
  }

  return true;
}

bool nom::SDL_Image::loadFromFile_BMP ( const std::string& filename )
{
  this->image_buffer = SDL_LoadBMP ( filename.c_str() );

  if ( static_cast<SDL_Surface*> ( this->image_buffer ) == nullptr )
  {
    #ifdef DEBUG_SDL_IMAGE
      std::cout << "ERR in nom::SDL_Image::loadFromFile_BMP(): " << SDL_GetError() << std::endl << std::endl;
    #endif
    return false;
  }

  return true;
}

bool nom::SDL_Image::saveToFile ( const std::string& filename, void* video_buffer )
{
  if ( SDL_SaveBMP ( static_cast<SDL_Surface*> ( video_buffer ), filename.c_str() ) != 0 )
  {
    #ifdef DEBUG_SDL_IMAGE
      std::cout << "ERR in nom::SDL_Image::saveToFile(): " << SDL_GetError() << std::endl << std::endl;
    #endif
    return false;
  }

  return true;
}

const nom::Coords nom::SDL_Image::getSize ( void ) const
{
  SDL_Surface* buffer = ( SDL_Surface* ) image_buffer;

  return nom::Coords ( 0, 0, buffer->w, buffer->h );
}
