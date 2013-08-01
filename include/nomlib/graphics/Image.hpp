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

#include "nomlib/config.hpp"
#include "nomlib/system/Logger.hpp"
#include "nomlib/sdl/utils.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/graphics/Canvas.hpp"

namespace nom {

class Image
{
  public:
    /// Default constructor; initializes SDL_image extension
    Image ( int32 img_flags = IMG_INIT_PNG );
    /// Copy constructor
    Image ( const Image& other );
    virtual ~Image ( void );

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    /// Supports every file type that the libSDL_image extension has been
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
    Image& operator = ( const Image& other );

  private:
    std::shared_ptr<void> image_buffer; // SDL_Surface*
};


} // namespace nom

#endif // NOMLIB_SDL_IMAGE_HEADERS defined
