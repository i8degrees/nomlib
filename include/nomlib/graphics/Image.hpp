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
#include "nomlib/math/Point2-inl.hpp"
#include "nomlib/SDL_helpers.hpp"

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
    SDL_Surface* image ( void ) const;

    /// Is this object initialized? Valid when *NOT* nullptr
    bool valid ( void ) const;

    int32 width ( void ) const;
    int32 height ( void ) const;
    void* pixels ( void ) const;
    uint16 pitch ( void ) const;

    /// \todo Rename me to bppp
    uint8 bits_per_pixel ( void ) const;

    const SDL_PixelFormat* pixel_format ( void ) const;

    /// Obtain the video surface's red alpha mask
    const uint32 red_mask ( void ) const;

    /// Obtain the video surface's green alpha mask
    const uint32 green_mask ( void ) const;

    /// Obtain the video surface's blue alpha mask
    const uint32 blue_mask ( void ) const;

    /// Obtain the video surface's alpha mask
    const uint32 alpha_mask ( void ) const;

    /// I think that we are accessing the value of an
    /// (internal?) property of the SDL_Surface structure that is described as being
    /// "private" as per the docs.
    ///
    /// Return value of this internal property is presumed to be boolean -- no
    /// verification has been made of this. Testing of this method *appears*
    /// to be in working order.
    //bool getCanvasLock ( void ) const;

    /// Lock the display context's video surface; this must be done before you
    /// attempt to write directly to video memory, such as when you are
    /// manipulating surfaces at the pixel level.
    //bool lock ( void ) const;

    /// Unlocks the display context's video surface; this must be done after you
    /// are finished writing to the video buffer. During the time that the video
    /// surface is locked, no updates (think: rendering) outside of your local
    /// access can occur until the surfaces affected by the lock are relinquished.
    //void unlock ( void ) const;

    const Coords bounds ( void ) const;

    void set_bounds ( const Coords& clip_bounds );

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
    const Point2i size ( void ) const;

    /// Obtain the set color key for this image
    ///
    /// \return Returns new nom::Color on success; nom::Color::null on failure
    const Color colorkey ( void ) const;

    /// Obtain the blend mode used for blitting
    const SDL_BlendMode blend_mode ( void ) const;

    /// Set a new color key on the image loaded into memory.
    ///
    /// \param colorkey     Pixel color to mark transparent
    /// \param flag         TRUE to enable color key; FALSE to disable color key
    bool set_colorkey ( const Color& colorkey, bool flag );

    /// Set RLE acceleration for this image
    ///
    /// If enabled, color keying and alpha blending blits are much faster, but
    /// at the cost of requiring surface locking before directly accessing
    /// pixels.
    ///
    /// \param      TRUE to enable RLE acceleration; FALSE to disable
    bool RLE ( bool flag );

    /// Pixel reading -- supports 8-bit, 15/16-bit, 24-bit & 32-bit color modes
    ///
    /// Returns -1 on error
    ///
    /// You are responsible for locking & unlocking of the Image before-hand
    ///
    /// \todo Test 8-bit, 15/16-bit & 24-bit pixels
    uint32 pixel ( int32 x, int32 y );

    /// Set a new blending mode for blitting
    bool set_blend_mode ( const SDL_BlendMode blend );

  private:
    std::shared_ptr<SDL_Surface> image_;
};


} // namespace nom

#endif // NOMLIB_SDL_IMAGE_HEADERS defined
