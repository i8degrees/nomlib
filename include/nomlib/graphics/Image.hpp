/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_SDL2_IMAGE_HEADERS
#define NOMLIB_SDL2_IMAGE_HEADERS

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

#include "SDL.h"
#include "SDL_image.h"

#include "nomlib/config.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Rect-inl.hpp"
#include "nomlib/graphics/smart_ptr.hpp"

namespace nom {

/// \brief Bitmap storage container
class Image
{
  public:
    /// Default constructor -- initializes to sane defaults.
    Image ( void );

    /// Destructor.
    virtual ~Image ( void );

    /// Construct an Image object with specified flags passed to the library
    /// extension(s) used in this object.
    Image ( uint32 img_flags );

    /// Copy constructor
    Image ( const Image& other );

    /// Copy assignment constructor
    Image& operator = ( const Image& other );

    /// Obtain the SDL_Surface struct used in this object instance
    SDL_Surface* get ( void ) const;

    /// Is this object initialized? Valid when *NOT* nullptr
    bool valid ( void ) const;

    /// Supports every file type that the libSDL_image extension has been
    /// compiled with
    bool load ( const std::string& filename );

    /// Uses SDL's built-in BMP file loader; no alpha channeling support ...
    /// perfect for setting window icons!
    ///
    /// (I have yet to find success using IMG_Load to load an ordinary bitmap
    /// file, whereas SDL_LoadBMP does load fine).
    bool load_bmp ( const std::string& filename );

    /// Saves as an uncompressed RGB Windows Bitmap (BMP)
    ///
    /// NOTE: AFAIK, no existing file handling / overwriting checks are done
    /// whatsoever
    bool save ( const std::string& filename, SDL_Surface* video_buffer );

    /// Obtain the width and height (in pixels) of the stored bitmap buffer
    const Coords getSize ( void ) const;

  private:
    std::shared_ptr<SDL_Surface> image_buffer;
};


} // namespace nom

#endif // NOMLIB_SDL_IMAGE_HEADERS defined
