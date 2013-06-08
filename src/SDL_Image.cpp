/******************************************************************************
    SDL_Image.cpp

    SDL image class interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Image.hpp"

nom::SDL_Image::SDL_Image ( void )
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
}

void* nom::SDL_Image::loadImageFromFile ( const std::string& filename, const Color& colorkey, uint32_t flags )
{
  SDL_Surface *temp_buffer = NULL;
  SDL_Surface *video_buffer = NULL;

  temp_buffer = IMG_Load ( filename.c_str() );

  if ( temp_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::LoadImage() at IMG_Load(): " << IMG_GetError() << std::endl;
    #endif

    return NULL;
  }

  Gfx::setTransparent ( temp_buffer, colorkey, flags );

  // Add check in for if SDL_SRCALPHA flag is set
  if ( colorkey.getAlpha() != -1 )
    video_buffer = SDL_DisplayFormatAlpha ( temp_buffer );
  else
    video_buffer = SDL_DisplayFormat ( temp_buffer );

  SDL_FreeSurface ( temp_buffer );
  temp_buffer = NULL;

  return video_buffer;
}
