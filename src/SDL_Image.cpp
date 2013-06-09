/******************************************************************************
    SDL_Image.cpp

    SDL image class interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Image.hpp"

nom::SDL_Image::SDL_Image ( void )  : image_buffer ( nullptr )
{
  #ifdef DEBUG_SDL_IMAGE_OBJ
    std::cout << "SDL_Image::SDL_Image(): Hello, world!" << std::endl << std::endl;
  #endif
}

nom::SDL_Image::~SDL_Image ( void )
{
  #ifdef DEBUG_SDL_IMAGE_OBJ
    std::cout << "SDL_Image::~SDL_Image(): Goodbye cruel world!" << std::endl << std::endl;
  #endif

  this->image_buffer = nullptr; // ...better safe than sorry?
}

bool nom::SDL_Image::loadFromFile ( const std::string& filename, const Color& colorkey, uint32_t flags )
{
  this->image_buffer = IMG_Load ( filename.c_str() );

  if ( this->image_buffer == nullptr )
  {
    #ifdef DEBUG_SDL_IMAGE
      std::cout << "ERR in nom::SDL_Image::LoadImage() at IMG_Load(): " << IMG_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

void* nom::SDL_Image::get ( void ) const
{
  return static_cast<SDL_Surface*> ( this->image_buffer );
}
