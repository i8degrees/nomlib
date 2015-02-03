/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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

#include <string>
#include <memory>

// TODO: Forward declare
#include <SDL.h>

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Rect.hpp"

namespace nom {

/// \brief Bitmap storage container
class Image
{
  public:
    typedef Image* RawPtr;
    typedef std::shared_ptr<Image> SharedPtr;

    /// Default constructor -- initializes to sane defaults.
    Image ( void );

    /// \brief Destructor.
    ///
    /// \remarks The video surface -- if locked -- will be unlocked at this
    /// time before freeing its memory.
    ~Image( void );

    /// Copy constructor
    Image ( const Image& copy );

    /// Copy assignment constructor
    Image& operator = ( const Image& other );

    /// \brief Make a copy of the existing surface
    ///
    /// \returns Pointer to a new SDL_Surface structure
    SDL_Surface* clone ( void ) const;

    /// Obtain the SDL_Surface struct used in this object instance
    SDL_Surface* image ( void ) const;

    SDL_Texture* texture ( void ) const;

    /// Is this object initialized? Valid when *NOT* nullptr
    bool valid ( void ) const;

    /// Initialize an image surface using the data from an existing pixel
    /// buffer (from a SDL_Surface).
    ///
    /// \param pixels           Typically a void pointer cast to nom::uint16 or nom::uint32
    /// \param bits_per_pixel   Bits per pixel
    ///
    /// \note You maintain ownership of your pixel buffer pointer; don't forget
    /// to clean up after you're done!
    bool initialize ( void* pixels, int32 width, int32 height,
                      int bits_per_pixel, uint16 pitch,
                      uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask
                    );

    /// Constructor variant for creating a fresh, empty video surface. You should
    /// be sure to read over the details gory details scattered throughout the
    /// documentation for SDL.
    bool initialize ( int32 width, int32 height, int bits_per_pixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask );

    /// \brief Initialize a nom::Image from an existing SDL_Surface
    ///
    /// \param source   Existing SDL_Surface structure
    bool initialize ( SDL_Surface* source );

    /// \brief Create a new memory buffer
    ///
    /// \param size         Point2i initialized with the width & height in pixels
    /// \param pixel_format Pixel format to use for the memory buffer.
    ///
    /// \deprecated
    bool create( const Point2i& size, uint32 pixel_format );
    bool create( const Size2i& size, uint32 pixel_format );

    /// \brief  Create a new memory buffer using the most optimal pixel format
    ///         detected for your graphics hardware.
    ///
    /// \param  size  Point2i initialized with the width & height in pixels
    ///
    /// \remarks  Unless you have specialized needs, this is probably the method
    ///           call you want.
    bool initialize ( const Point2i& size );

    int32 width ( void ) const;
    int32 height ( void ) const;
    void* pixels ( void ) const;
    uint16 pitch ( void ) const;

    uint8 bits_per_pixel ( void ) const;

    const SDL_PixelFormat* pixel_format ( void ) const;

    /// Obtain the video surface's red alpha mask
    uint32 red_mask ( void ) const;

    /// Obtain the video surface's green alpha mask
    uint32 green_mask ( void ) const;

    /// Obtain the video surface's blue alpha mask
    uint32 blue_mask ( void ) const;

    /// Obtain the video surface's alpha mask
    uint32 alpha_mask ( void ) const;

    const IntRect bounds ( void ) const;

    /// Check to see if the video surface needs locking
    bool must_lock ( void ) const;

    /// Obtain the locked status of the video surface memory
    ///
    /// \note This method accesses internal (private) data within our
    /// SDL_Surface structure.
    ///
    /// \todo Verify if the SDL_MUSTLOCK macro does the same thing here
    bool locked( void ) const;

    /// Obtain the alpha value of this video surface
    uint8 alpha ( void ) const;

    const Point2i position ( void ) const;

    void set_bounds ( const IntRect& bounds );

    /// Supports every file type that the libSDL_image extension has been
    /// compiled with
    bool load( const std::string& filename, uint32 pixel_format = SDL_PIXELFORMAT_ARGB8888 );

    /// \brief Load an image surface from a memory source.
    ///
    /// \param buffer       The input buffer contents.
    /// \param buffer_size  The integral size of the image data.
    /// \param ext          The file extension of the image data.
    /// \param pixel_format The pixel format of the image data.
    ///
    /// \note Uses SDL2_image extension.
    bool load_memory( const char* buffer,
                      int buffer_size,
                      const std::string& ext,
                      uint32 pixel_format = SDL_PIXELFORMAT_ARGB8888 );

    /// Uses SDL's built-in BMP file loader; no alpha channeling support ...
    /// perfect for setting window icons!
    ///
    /// (I have yet to find success using IMG_Load to load an ordinary bitmap
    /// file, whereas SDL_LoadBMP does load fine).
    bool load_bmp ( const std::string& filename, uint32 pixel_format = SDL_PIXELFORMAT_ARGB8888 );

    /// Save a screen shot of this image surface as an uncompressed BMP file
    ///
    /// \param filename       Output filename path
    ///
    /// \note SDL2 appears to be saving bitmap files with ARGB pixel ordering;
    /// SDL_PIXELFORMAT_ARGB8888
    bool save_bmp ( const std::string& filename ) const;

    /// Save a screen shot as a Portable Network Graphics (PNG) file.
    ///
    /// \param filename       Output filename path
    ///
    /// \note SDL2_image appears to be saving PNG files with ARGB pixel
    /// ordering; SDL_PIXELFORMAT_ARGB8888
    bool save_png ( const std::string& filename ) const;

    /// Obtain the width and height (in pixels) of the stored bitmap buffer
    Size2i size() const;

    /// Obtain the set color key for this image
    ///
    /// \returns  nom::Color4i on success; nom::Color4i::null on err
    const Color4i colorkey ( void ) const;

    /// Obtain the blend mode used for blitting
    const SDL_BlendMode blend_mode ( void ) const;

    /// \brief  Obtain the additional color value multiplied into render copy
    ///         operations.
    ///
    /// \returns  nom::Color4i on success; nom::Color4i::null on err
    const Color4i color_modulation ( void ) const;

    /// Set a new color key on the image loaded into memory.
    ///
    /// \param colorkey     Pixel color to mark transparent
    /// \param flag         TRUE to enable color key; FALSE to disable color key
    bool set_colorkey ( const Color4i& colorkey, bool flag );

    /// Set RLE acceleration for this image
    ///
    /// If enabled, color keying and alpha blending blits are much faster, but
    /// at the cost of requiring surface locking before directly accessing
    /// pixels.
    ///
    /// \param      TRUE to enable RLE acceleration; FALSE to disable
    bool RLE ( bool flag );

    /// \brief Read a RGBA pixel from the video surface.
    ///
    /// \returns  A 32-bit encoded color value, dependent upon the pixel format.
    ///
    /// \remarks You are responsible for any necessary video surface locking
    /// and unlocking.
    ///
    /// \note The color depth is determined by the existing color depth of the
    ///       nom::Image -- 8-bit, 15/16-bit, 24-bit or 32-bit bits per pixel
    ///       depths are supported. When a color depth cannot be determined, it
    ///       is assumed 32-bit.
    ///
    /// \todo Test 8-bit, 15/16-bit & 24-bit pixel blits
    uint32 pixel( int x, int y ) const;

    /// \brief Read a RGBA pixel from the video surface.
    ///
    /// \see Image::pixel.
    Color4i color4i_pixel( int x, int y ) const;

    /// \brief Read a RGBA pixel from the video surface.
    ///
    /// \see Image::color4i_pixel, Image::pixel.
    Color4i pixel( const Point2i& pos ) const;

    /// \brief Blit a RGBA pixel onto the video surface
    ///
    /// \remarks You are responsible for any necessary video surface locking
    /// and unlocking.
    ///
    /// \note The color depth is determined by the existing color depth of the
    ///       nom::Image -- 8-bit, 15/16-bit, 24-bit or 32-bit bits per pixel
    ///       depths are supported.
    ///
    /// \todo Test 8-bit, 15/16-bit & 24-bit pixel blits
    void set_pixel( int x, int y, const Color4i& color );

    /// Set a new blending mode for blitting
    bool set_blend_mode ( const SDL_BlendMode blend );

    /// Lock the video surface; this must be done before you attempt to write
    /// directly to video memory (pixel manipulation).
    bool lock ( void ) const;

    /// Unlocks the video surface; this must be done after you are finished
    /// writing to the video buffer. During the time that the video surface is
    /// locked, no updates (think: rendering) outside of your local access can
    /// occur until the surfaces affected by the lock are relinquished.
    void unlock ( void ) const;

    bool set_alpha ( uint8 opacity );

    void set_position ( const Point2i& pos );

    /// Render a video surface
    ///
    /// \todo This method needs to mimic nom::Texture::draw
    void draw ( SDL_Surface* destination, const IntRect& bounds ) const;

    /// \brief    Set an additional color value multiplied into render copy
    ///           operations
    ///
    /// \param    nom::Color4i  red, green & blue values multiplied into
    ///                         color operations
    ///
    /// \returns  TRUE on success; FALSE on failure
    ///
    /// \remarks  SDL2 color modulation formula:
    ///           srcC = srcC * ( color / 255 )
    bool set_color_modulation ( const Color4i& color );

  private:
    /// Container for our image pixels buffer
    std::shared_ptr<SDL_Surface> image_;

    Point2i position_;  // Not implemented
    IntRect bounds_;    // Not implemented
};


} // namespace nom

#endif // NOMLIB_SDL_IMAGE_HEADERS defined
