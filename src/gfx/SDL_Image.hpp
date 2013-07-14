/******************************************************************************

    SDL image class interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_IMAGE_HEADERS
#define NOMLIB_SDL_IMAGE_HEADERS

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "config.hpp"
#include "sys/Logger.hpp"
#include "sdl/utils.hpp"
#include "math/Color.hpp"
#include "math/Coords.hpp"
#include "gfx/SDL_Canvas.hpp"

namespace nom {

class SDL_Image
{
  public:
    /// Default constructor; initializes SDL_image extension
    SDL_Image ( int32 img_flags = IMG_INIT_PNG );
    /// Copy constructor
    SDL_Image ( const SDL_Image& other );
    ~SDL_Image ( void );

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    /// Supports every file type that the SDL_image extension has been
    /// compiled with
    std::shared_ptr<void> loadFromFile ( const std::string& filename );

    /// Uses SDL's built-in BMP file loader; no alpha channeling support ...
    /// perfect for setting window icons!
    std::shared_ptr<void> loadFromFile_BMP ( const std::string& filename );

    /// Saves as an uncompressed RGB Windows Bitmap (BMP)
    ///
    /// NOTE: AFAIK, no existing file handling / overwriting checks are done
    /// whatsoever
    bool saveToFile ( const std::string& filename, void* video_buffer );

    const Coords getSize ( void ) const;
    /// Copy assignment constructor
    SDL_Image& operator = ( const SDL_Image& other );

  private:
    std::shared_ptr<void> image_buffer; // SDL_Surface*
};


} // namespace nom

#endif // NOMLIB_SDL_IMAGE_HEADERS defined
