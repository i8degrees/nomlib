/******************************************************************************
    SDL_Image.hpp

    SDL image class interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_IMAGE_HEADERS
#define NOMLIB_SDL_IMAGE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"

#include "Coords.h"
#include "Color.h"
#include "gamelib.h"

namespace nom
{
  class SDL_Image
  {
    public:
      SDL_Image ( void );
      ~SDL_Image ( void );

      bool loadFromFile ( const std::string& filename, const nom::Color&
                                colorkey = nom::Color ( 0, 0, 0, -1 ),
                                uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY
                              );

      void* get ( void ) const;

      // SDL_SaveBMP
      // SDL_LoadBMP
      // getSize

      void Draw ( void* video_buffer );

    private:
      void* image_buffer; // SDL_Surface*
  };
}

#endif // NOMLIB_SDL_IMAGE_HEADERS defined
