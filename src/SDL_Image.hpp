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

#include "SDL_Drawable.hpp"
#include "Coords.h"
#include "Color.h"
#include "SDL_Canvas.hpp"
#include "gfx.h"

namespace nom
{
  class SDL_Image
  {
    public:
      SDL_Image ( void );
      ~SDL_Image ( void );

      void* loadImageFromFile ( const std::string& filename, const nom::Color&
                                colorkey = nom::Color ( -1, -1, -1, -1 ),
                                uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY
                              );

      void Draw ( void* video_buffer );

    private:
      // ...
  };
}

#endif // NOMLIB_SDL_IMAGE_HEADERS defined
