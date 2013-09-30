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
#ifndef NOMLIB_SDL_CANVAS_HEADERS
#define NOMLIB_SDL_CANVAS_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include "SDL.h"

#include "nomlib/config.hpp"
//#include "nomlib/graphics/ICanvas.hpp"
#include "nomlib/system/make_unique.hpp"
#include "nomlib/math/helpers.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Rect-inl.hpp"
#include "nomlib/math/Point2-inl.hpp"
#include "nomlib/system/ObjectCache.hpp"
#include "nomlib/graphics/smart_ptr.hpp"
#include "nomlib/graphics/Pixel.hpp"
#include "nomlib/graphics/Image.hpp"
#include "nomlib/graphics/Rectangle.hpp"

//#define NOM_DEBUG_SDL_CANVAS

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

class Canvas
{
  public:
    typedef std::shared_ptr<Canvas> SharedPtr;
    typedef SDL_Texture* RawPtr;

    /// Default constructor; initializes object to its respective defaults
    Canvas ( void );

    /// Constructor variant for creating a video surface object with existing
    /// video surface memory.
    ///
    /// \deprecated Likely to be removed in the future.
    //Canvas ( SDL_Surface* video_buffer );

    Canvas ( SDL_Texture* video_buffer );

    /// Copy constructor; create a video surface object from an existing Canvas
    /// object.
    Canvas ( const Canvas& other );

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
    Canvas ( int32 width, int32 height, uint8 bitsPerPixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask, uint32 flags = SDL_SWSURFACE );

    /// Construct a video surface object using the data from an existing surface;
    /// you can think of this as a copy method. As with the previous method, you
    /// ought to read over the technicalities that the SDL library documentation
    /// covers.
    ///
    /// You remain the owner of the pointer leading to your existing data and
    /// therefore are responsible for memory management.
    ///
    /// See http://sdl.beuc.net/sdl.wiki/SDL_CreateRGBSurfaceFrom
    Canvas ( void* pixels, int32 width, int32 height, int32 depth, uint16 pitch, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask );

    /// Lazy destructor -- does nothing.
    ~Canvas ( void );

    void initialize ( uint32 flags, int32 width, int32 height, uint8 bitsPerPixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask );

    /// Return a std::shared_ptr copy of this instance
    /// \todo Test me out!
    Canvas::SharedPtr clone ( void ) const;

    /// Copy assignment operator
    Canvas& operator = ( const Canvas& other );

    const Coords& getPosition ( void ) const;

    /// Get the video memory surface of the canvas object
    Canvas::RawPtr get ( void ) const;

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    /// Reset the video surface object with another video surface object.
    void setCanvas ( const Canvas& surface );

    void setPosition ( const Coords& pos );
    void setOffsets ( const Coords& clip );

    const int32 getCanvasWidth ( void ) const;
    const int32 getCanvasHeight ( void ) const;
    uint32 getCanvasFlags ( void ) const;
    uint16 getCanvasPitch ( void ) const;
    void* getCanvasPixels ( void ) const;
    const uint8 getCanvasBitsPerPixel ( void ) const;

    /// \todo Rename to getCanvasPixelFormat
    SDL_PixelFormat* getCanvasPixelsFormat ( void ) const;

    /// Obtain the pixel value of the set transparent color
    const Color getCanvasColorKey ( void ) const;

    /// Obtain the overall surface alpha value
    const uint8 getCanvasAlphaValue ( void ) const;

    /// Obtain the video surface's red *ALPHA* mask
    const uint32 getCanvasRedMask ( void ) const;

    /// Obtain the video surface's green *ALPHA* mask
    const uint32 getCanvasGreenMask ( void ) const;

    /// Obtain the video surface's blue *ALPHA* mask
    const uint32 getCanvasBlueMask ( void ) const;

    /// Obtain the video surface's alpha mask
    const uint32 getCanvasAlphaMask ( void ) const;

    const Coords getCanvasBounds ( void ) const;
    void setCanvasBounds ( const Coords& clip_bounds );

    /// Convenience Helper method for calculating the canvas color depth; for
    /// the time being, this is merely for the convenience of readability in
    /// our code.
    ///
    /// Returns an integer of 8, 16, 24 or 32 upon successful determination of
    /// the current color depth.
    ///
    /// Returns -1 on err; perhaps an unsupported color depth or an
    /// uninitialized video surface?
    int32 getCanvasColorDepth ( void ) const;

    /// I think that we are accessing the value of an
    /// (internal?) property of the SDL_Surface structure that is described as being
    /// "private" as per the docs.
    ///
    /// Return value of this internal property is presumed to be boolean -- no
    /// verification has been made of this. Testing of this method *appears*
    /// to be in working order.
    bool getCanvasLock ( void ) const;

    /// Lock the video surface for writing directly to video memory.
    bool lock ( void ) const;

    /// Unlocks the video surface; this must be done after you are finished
    /// writing to the video buffer. During the time that the video surface is
    /// locked, no updates (think: rendering) outside of your local access can
    /// occur until the surfaces affected by the lock are relinquished.
    void unlock ( void ) const;

    /// Sets transparency only if colorkey Color alpha value is -1, also
    /// important to note is that we only have an alpha channel surface set
    /// if Color value is not -1 (the default)
    ///
    /// \internal
    /// Clean up this documentation note and also verify that it is
    /// sane to assume that you would not use transparency when you have alpha
    /// surface enabled
    /// \endinternal
    bool load ( const std::string& filename, const Color&
                colorkey = Color::null,
                bool use_cache = false,
                uint32 flags = SDL_RLEACCEL | SDL_TRUE//SRCCOLORKEY
              );

    void Update ( SDL_Renderer* );
    void Draw ( SDL_Surface* video_buffer ) const;

    bool setAlpha ( uint8 opacity );//, uint32 flags/* = SDL_SRCALPHA*/ );

    bool setTransparent ( const Color& color = Color::null,
                          uint32 flags = SDL_RLEACCEL | SDL_TRUE//SDL_SRCCOLORKEY
                        );

    /// As per libSDL docs, we must first initialize the video subsystem before using
    /// this method call, otherwise an access violation fault is sure to occur.
    bool displayFormat ( void );
    /// As per libSDL docs, we must first initialize the video subsystem before using
    /// this method call, otherwise an access violation fault is sure to occur.
    bool displayFormatAlpha ( void );

    /// Pixel reading -- supports 8-bit, 15/16-bit, 24-bit & 32-bit color modes
    ///
    /// Returns -1 on error
    ///
    /// You are responsible for locking & unlocking of the canvas before-hand
    ///
    /// \todo Test 8-bit, 15/16-bit & 24-bit pixels
    uint32 getPixel ( int32 x, int32 y );

    /// Resize the video surface with the chosen rescaling algorithm.
    ///
    /// See the ResizeAlgorithm enum for available rescaling algorithms
    bool resize ( enum ResizeAlgorithm scaling_algorithm );

    bool resize ( const Point2f& scale_factor );

    /// Return the correct scaling factor of the chosen algorithm
    int32 getResizeScaleFactor ( enum ResizeAlgorithm scaling_algorithm );

  private:
    /// Internal method used for checking to see if the video surface actually
    /// needs to be locked before doing so for performance sake.
    bool mustLock ( void ) const;

    std::shared_ptr<SDL_Texture> canvas_buffer;
    /// Holds surface position
    Coords coords;
    /// Holds surface bounds (input clipping)
    Coords offsets;
};


} // namespace nom

#endif // NOMLIB_SDL_CANVAS_HEADERS defined
