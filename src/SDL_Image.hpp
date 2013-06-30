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

#include "Coords.hpp"
#include "Color.hpp"
#include "gamelib.h"

namespace nom
{
  class SDL_Image
  {
    public:
      SDL_Image ( uint32_t img_flags = IMG_INIT_PNG );
      ~SDL_Image ( void );

      /// Obtains raw pointer to the object's video surface buffer
      ///
      /// Returns ( SDL_Surface* )
      ///
      void* get ( void ) const;

      /// Is this SDL_Surface* initialized ( != nullptr )?
      bool valid ( void ) const;

      /// Supports every file type that your SDL_image extension has been
      /// compiled
      bool loadFromFile ( const std::string& filename );

      /// Uses SDL's built-in BMP file loader; no alpha channeling support ...
      /// perfect for setting window icons!
      bool loadFromFile_BMP ( const std::string& filename );

      /// Saves as an uncompressed RGB Windows Bitmap (BMP)
      ///
      /// NOTE: AFAIK, no existing file handling / overwriting checks are done
      /// whatsoever
      bool saveToFile ( const std::string& filename, void* video_buffer );

      const nom::Coords getSize ( void ) const;

    private:
      void* image_buffer; // SDL_Surface*
  };
}

#endif // NOMLIB_SDL_IMAGE_HEADERS defined
