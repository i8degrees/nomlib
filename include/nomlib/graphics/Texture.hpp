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
#ifndef NOMLIB_SDL2_TEXTURE_HEADERS
#define NOMLIB_SDL2_TEXTURE_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/system/make_unique.hpp"
#include "nomlib/math/helpers.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Rect-inl.hpp"
#include "nomlib/math/Point2-inl.hpp"
#include "nomlib/system/ObjectCache.hpp"
#include "nomlib/graphics/smart_ptr.hpp"
#include "nomlib/graphics/Image.hpp"
#include "nomlib/graphics/Pixel.hpp"
#include "nomlib/graphics/shapes/Rectangle.hpp"
#include "nomlib/graphics/Window.hpp"

//#define NOM_DEBUG_SDL_TEXTURE

namespace nom {

/// Available rescaling algorithms
enum ResizeAlgorithm
{
  None = 0, // No resizing is applied
  scale2x,
  scale3x, // Reserved for future implementation
  scale4x, // Reserved for future implementation
  hq2x,
  hq3x,
  hq4x,
  Stretch
};

class Texture
{
  public:
    typedef std::shared_ptr<Texture> SharedPtr;

    /// Default constructor; initializes object to its respective defaults
    Texture ( void );

    /// Lazy destructor -- does nothing.
    ~Texture ( void );

    /// Constructor variant for creating a video surface object with existing
    /// video surface memory.
    Texture ( SDL_Surface* video_buffer );

    //Texture ( SDL_Texture* video_buffer );

    /// Copy constructor; create a video surface object from an existing Texture
    /// object.
    Texture ( const Texture& other );

    /// Constructor variant for creating a fresh, empty video surface. You should
    /// be sure to read over the details gory details scattered throughout the
    /// documentation for SDL.
    ///
    /// The SDL_SWSURFACE flag is passed by default if an argument is not
    /// specified -- this is optimal for writing to the video memory buffer
    /// directly (pixel manipulation).
    ///
    /// As per libSDL docs, this must be called only after video initialization;
    /// (SDL_SetVideoMode).
    ///
    /// See http://sdl.beuc.net/sdl.wiki/SDL_CreateRGBSurface
    Texture ( int32 width, int32 height, uint8 bitsPerPixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask, uint32 flags = SDL_SWSURFACE );

    /// Construct a video surface object using the data from an existing surface;
    /// you can think of this as a copy method. As with the previous method, you
    /// ought to read over the technicalities that the SDL library documentation
    /// covers.
    ///
    /// You remain the owner of the pointer leading to your existing data and
    /// therefore are responsible for memory management.
    ///
    /// See http://sdl.beuc.net/sdl.wiki/SDL_CreateRGBSurfaceFrom
    Texture ( void* pixels, int32 width, int32 height, int32 depth, uint16 pitch, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask );

    void initialize ( uint32 flags, int32 width, int32 height, uint8 bitsPerPixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask );

    bool initialize ( SDL_Surface* video_buffer );

    /// Return a std::shared_ptr copy of this instance
    /// \todo Test me out!
    Texture::SharedPtr clone ( void ) const;

    /// Copy assignment operator
    Texture& operator = ( const Texture& other );

    const Point2i& position ( void ) const;

    const Coords& bounds ( void ) const;

    /// Get the video memory surface of the Texture object
    SDL_Texture* texture ( void ) const;

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    /// Reset the video surface object with another video surface object.
    /// \todo Relocate me to a surface class
    ///void setTexture ( const Texture& surface );

    void set_position ( const Point2i& pos );
    void set_bounds ( const Coords& clip );

    int32 width ( void ) const;
    int32 height ( void ) const;
    uint16 pitch ( void ) const;
    void* pixels ( void ) const;

    uint8 bytes_per_pixel ( void ) const;

    uint32 pixel_format ( void ) const;

    /// Obtain the overall surface alpha value
    //const uint8 getTextureAlphaValue ( void ) const;

    /// Obtain the video surface's red *ALPHA* mask
    //const uint32 getTextureRedMask ( void ) const;

    /// Obtain the video surface's green *ALPHA* mask
    //const uint32 getTextureGreenMask ( void ) const;

    /// Obtain the video surface's blue *ALPHA* mask
    //const uint32 getTextureBlueMask ( void ) const;

    /// Obtain the video surface's alpha mask
    //const uint32 getTextureAlphaMask ( void ) const;

    //const Coords getTextureBounds ( void ) const;
    //void setTextureBounds ( const Coords& clip_bounds );

    /// Calculate this object's bits per pixel (color depth)
    ///
    /// \return Unsigned 8-bit integer (8, 16, 24 or 32 bits per pixel); zero (0)
    /// on error (unknown color depth).
    uint8 bits_per_pixel ( void ) const;

    /// I think that we are accessing the value of an
    /// (internal?) property of the SDL_Surface structure that is described as being
    /// "private" as per the docs.
    ///
    /// Return value of this internal property is presumed to be boolean -- no
    /// verification has been made of this. Testing of this method *appears*
    /// to be in working order.
    /// \todo bool getTextureLock ( void ) const;

    /// Lock a portion of the texture for **write-only** pixel access.
    ///
    /// Texture must be created with SDL_TEXTUREACCESS_STREAMING
    /// \todo pixels access
    bool lock ( void );

    /// Lock a portion of this texture for write-only pixel access.
    ///
    /// \param lock_coords  The area encompassing the area to lock for pixel
    /// access
    bool lock ( const Coords& lock_coords );

    /// Unlock this texture, thereby uploading the necessary changes (if any)
    /// to video memory.
    void unlock ( void ) const;

    /// Sets transparency only if colorkey Color alpha value is -1, also
    /// important to note is that we only have an alpha channel surface set
    /// if Color value is not -1 (the default)
    ///
    /// \param filename         File path you wish to load into memory as a
    ///                         valid bitmap / texture
    ///                         (think: supported image file types).
    ///
    /// \param colorkey         The color key you wish to use to apply a color
    ///                         key to
    /// \param use_cache        Whether or not to use an internal object cache
    ///                         feature of nomlib. Defaults to off.
    ///
    /// \param flags            SDL_RLEACCEL
    ///
    /// \todo Test/Research Texture caching -- nom::ObjectCache worked
    /// beautifully with SDL_Surface, is the same true of SDL_Texture?
    /// \todo Consider removing SDL_RLEACCEL flag
    /// \todo Merge use_cache into flags variable
    bool load ( const std::string& filename, const Color&
                colorkey = Color::null,
                bool use_cache = false,
                uint32 flags = SDL_RLEACCEL
              );

    /// Stub for SDL_UpdateTexture implementation
    bool update ( const void* pixels, uint16 pitch, const Coords& update_area );

    /// \todo Remove me
    void draw ( SDL_Renderer* target ) const;

    void draw ( const Window& target ) const;

    bool set_alpha ( uint8 opacity );

    /// As per libSDL docs, we must first initialize the video subsystem before using
    /// this method call, otherwise an access violation fault is sure to occur.
    ///
    /// \todo Relocate to nom::Image or whichever class becomes home to surfaces
    /// (video memory buffers in system RAM)
    ///bool displayFormat ( void );

    /// As per libSDL docs, we must first initialize the video subsystem before using
    /// this method call, otherwise an access violation fault is sure to occur.
    ///
    /// \todo Relocate to nom::Image or whichever class becomes home to surfaces
    /// (video memory buffers in system RAM)
    ///bool displayFormatAlpha ( void );

    /// Pixel reading -- supports 8-bit, 15/16-bit, 24-bit & 32-bit color modes
    ///
    /// Returns -1 on error
    ///
    /// You are responsible for locking & unlocking of the Texture before-hand
    ///
    /// \todo Test 8-bit, 15/16-bit & 24-bit pixels
    /// \todo Relocate to nom::Image or whichever class becomes home to surfaces
    /// (video memory buffers in system RAM)
    uint32 getPixel ( int32 x, int32 y );

    /// Resize the video surface with the chosen rescaling algorithm.
    ///
    /// See the ResizeAlgorithm enum for available rescaling algorithms
    bool resize ( enum ResizeAlgorithm scaling_algorithm );

    bool resize ( const Point2f& scale_factor );

    /// Return the correct scaling factor of the chosen algorithm
    int32 getResizeScaleFactor ( enum ResizeAlgorithm scaling_algorithm );

  private:
    std::shared_ptr<SDL_Texture> texture_;
    void* pixels_;
    int* pitch_;

    /// Holds surface position (X, Y)
    Point2i position_;

    /// Holds surface offsets (clipping area: X, Y, width & height)
    Coords bounds_;
};


} // namespace nom

#endif // include guard defined
