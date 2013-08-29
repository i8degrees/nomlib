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
#include <algorithm>

#include <SDL/SDL.h>

#include "nomlib/config.hpp"
//#include "nomlib/graphics/ICanvas.hpp"
#include "nomlib/sdl/utils.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/system/ObjectCache.hpp"
#include "nomlib/graphics/Image.hpp"
#include "nomlib/graphics/Rectangle.hpp"

// Optional library dependencies
#if defined ( HQX_FOUND )
    #include <hqx.h>
#endif

// C Macros used solely in Canvas::scale2x method
#define SCALE2x_READINT24(x) \
  ((x)[0]<<16 | (x)[1]<<8 | (x)[2])

#define SCALE2x_WRITEINT24(x, i) \
  {(x)[0]=i>>16; (x)[1]=(i>>8)&0xff; x[2]=i&0xff; }

namespace nom {
  namespace priv {

/// Custom deleter for SDL_Surface* smart pointers; can be used as a debugging
/// aid.
void Canvas_FreeSurface ( SDL_Surface* );

  } // namespace priv
} // namespace nom


namespace nom {

class Canvas
{
  public:
    /// Default constructor; initializes object to its respective defaults
    Canvas ( void );
    /// Constructor variant for setting the canvas with existing data
    Canvas ( void* video_buffer );
    /// Copy constructor
    Canvas ( const Canvas& other );

    /// Constructor variant for setting the canvas with an empty surface
    ///
    /// As per libSDL docs, this must be called only after video initialization;
    /// (SDL_SetVideoMode)
    ///
    /// http://sdl.beuc.net/sdl.wiki/SDL_CreateRGBSurface
    Canvas ( uint32 flags, int32 width, int32 height, uint8 bitsPerPixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask );

    /// Constructor variant for setting the canvas with existing pixel data
    ///
    /// http://sdl.beuc.net/sdl.wiki/SDL_CreateRGBSurfaceFrom
    ///
    /// Note that we are responsible for freeing our own pixels data
    Canvas ( Pixels pixels, int32 width, int32 height, int32 depth, int32 pitch, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask );

    /// Lazy destructor -- does nothing.
    ~Canvas ( void );

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    /// Set the canvas with existing surface data
    void setCanvas ( void* video_buffer );

    void setPosition ( const Coords& coords_ );
    void setOffsets ( const Coords& offsets_ );

    const int32_t getCanvasWidth ( void ) const;
    const int32_t getCanvasHeight ( void ) const;
    uint32_t getCanvasFlags ( void ) const;
    u_short getCanvasPitch ( void ) const;
    const Pixels getCanvasPixels ( void ) const;

    const uint8 getCanvasBitsPerPixel ( void ) const;
    const Pixels getCanvasPixelsFormat ( void ) const;
    const uint32 getCanvasRedMask ( void ) const;
    const uint32 getCanvasGreenMask ( void ) const;
    const uint32 getCanvasBlueMask ( void ) const;
    const uint32 getCanvasAlphaMask ( void ) const;

    const Coords getCanvasBounds ( void ) const;
    void setCanvasBounds ( const Coords& clip_bounds );

    /// Convenience Helper method for calculating the canvas color depth; for
    /// the time being, this is merely for the convenience of readability in
    /// our code.
    ///
    /// Returns -1 on err -- perhaps an unsupported color depth?
    ///
    /// Returns 8, 16, 24 or 32 on successful determination of depth
    ///
    int32 getCanvasColorDepth ( void ) const;

    /// I think that we are accessing the value of an
    /// (internal?) property of the SDL_Surface structure that is described as being
    /// "private" as per the docs.
    ///
    /// Return value of this internal property is presumed to be boolean -- no
    /// verification has been made of this. Testing of this method *appears*
    /// to be in working order.
    bool getCanvasLock ( void ) const;

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
                colorkey = Color ( -1, -1, -1, -1 ),
                bool use_cache = 0,
                uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY
              );

    bool Update ( void* video_buffer );
    void Draw ( void* video_buffer ) const;

    bool setAlpha ( uint8_t opacity, uint32_t flags = SDL_SRCALPHA );

    bool setTransparent ( const Color& color = Color ( 0, 0, 0, -1 ),
                          uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY
                        );

    /// As per libSDL docs, we must first initialize the video subsystem before using
    /// this method call, otherwise an access violation fault is sure to occur.
    bool displayFormat ( void );
    /// As per libSDL docs, we must first initialize the video subsystem before using
    /// this method call, otherwise an access violation fault is sure to occur.
    bool displayFormatAlpha ( void );

    /// \internal
    /// Note: this method is not meant to be called inside a loop; memory usage may
    /// run a mock (seems to be fixed by Rectangle::~Rectangle() inside the
    /// Rectangle class, although it eludes me as to why precisely
    /// \endinternal
    void clear ( const Color& color = Color::Blue ) const;

    /// Pixel reading -- supports 8-bit, 15/16-bit, 24-bit & 32-bit color modes
    ///
    /// Returns -1 on error
    ///
    /// You are responsible for locking & unlocking of the canvas before-hand
    ///
    /// \todo Test 8-bit, 15/16-bit & 24-bit pixels
    int32 getPixel ( int32 x, int32 y );

    /// Uses the AdvanceMAME bitmap scaling algorithm known as scale2x to scale
    /// a surface while maintaining the quality pixel art feel of the original
    /// art. The algorithm is designed to be fast enough to process 256x256
    /// bitmaps in real-time.
    ///
    /// This method re-implements the function scale2x found in the contrib/sdl
    /// directory of the scale2x distribution.
    ///
    /// This requires a destination surface already setup to be twice as
    /// large as the source. Surface formats must match. No err checks are done
    /// to ensure this.
    ///
    /// See http://scale2x.sourceforge.net/
    void scale2x ( int32 source_width, int32 source_height, Pixels source_buffer, Pixels destination_buffer );

    /// Use the hqx bitmap algorithm to scale a source buffer by 2x. hqx is a
    /// fast, high-quality magnification filter designed for pixel art. Compared
    /// to scale2x, you can generally expect similar results but with additional
    /// anti-aliasing applied. This makes the algorithm likely to be preferable
    /// for vastly increased resolution support (think: Apple Retina displays).
    ///
    /// The algorithm is designed to be fast enough to process 256x256 bitmaps
    /// in real-time.
    ///
    /// The supplied arguments are manipulated at the pixel-level, and are
    /// static casted to the appropriate fixed-integer size as necessary. If
    /// you are using libSDL, you ought to be able to pass the surface's pixels
    /// directly, as in the following:
    ///
    /// SDL_Surface* src;
    /// SDL_Surface* dst;
    ///
    /// hq2x ( src.pixels, dst.pixels );
    ///
    /// You are responsible for locking and unlocking the surfaces as need be.
    ///
    /// See https://code.google.com/p/hqx/wiki/ReadMe
    ///
    /// \todo FIXME; due to some bizarre linking issue resulting in unresolved
    /// symbols upon trying to use any of the function calls (such as hqxInit),
    /// I am unable to implement this method (or at least test it, anyway...).
    void hq2x ( int32 source_width, int32 source_height, Pixels source_buffer, Pixels destination_buffer );

    /// Copy assignment constructor
    Canvas& operator = ( const Canvas& other );

  private:
    /// SDL_Surface*
    std::shared_ptr<void> canvas_buffer;
    /// Holds surface position
    Coords coords;
    /// Holds surface bounds (input clipping)
    Coords offsets;
};


} // namespace nom

#endif // NOMLIB_SDL_CANVAS_HEADERS defined
