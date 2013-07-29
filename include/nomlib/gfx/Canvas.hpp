/******************************************************************************

    Abstraction of SDL Surfaces

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_CANVAS_HEADERS
#define NOMLIB_SDL_CANVAS_HEADERS

#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

#include <SDL/SDL.h>

#include "nomlib/config.hpp"
#include "nomlib/sys/Logger.hpp"
//#include "nomlib/gfx/ICanvas.hpp"
#include "nomlib/sdl/utils.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/sys/ObjectCache.hpp"
#include "nomlib/gfx/Image.hpp"
#include "nomlib/gfx/Rectangle.hpp"

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
    Canvas ( uint32_t flags, int32_t width, int32_t height, int32_t bitsPerPixel, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask );
    /// Constructor variant for setting the canvas with existing pixel data
    ///
    /// http://sdl.beuc.net/sdl.wiki/SDL_CreateRGBSurfaceFrom
    ///
    /// Note that we are responsible for freeing our own pixels data
    Canvas ( void* pixels, int32_t width, int32_t height, int32_t depth, int32_t pitch, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask );
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
    void* getCanvasPixels ( void ) const;
    void* getCanvasPixelsFormat ( void ) const;
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
    nom::int32 getCanvasColorDepth ( void ) const;

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
    bool loadFromImage  ( const std::string& filename, const Color&
                          colorkey = Color ( -1, -1, -1, -1 ),
                          bool use_cache = 0,
                          uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

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

    bool Lock ( void* video_buffer ) const;
    void Unlock ( void* video_buffer ) const;

    /// Pixel reading -- supports 8-bit, 15/16-bit, 24-bit & 32-bit color modes
    ///
    /// Returns -1 on error
    ///
    /// You are responsible for locking & unlocking of the canvas before-hand
    ///
    /// \todo Test 8-bit, 15/16-bit & 24-bit pixels
    int32 getPixel ( int32 x, int32 y );

    /// Uses the AdvanceMAME bitmap scaling algorithm known as scale2x to resize
    /// a surface without blurring in real-time -- up to 256x256 without
    /// incurring "heavy" performance hits.
    ///
    /// This method re-implements the function scale2x found in the contrib/sdl
    /// directory of the scale2x distribution.
    ///
    /// This requires a destination surface already setup to be twice as
    /// large as the source. Surface formats must match. No err checks are done
    /// to ensure this.
    ///
    /// See http://scale2x.sourceforge.net/
    void scale2x ( SDL_Surface* source_buffer, SDL_Surface* destination_buffer );

    /// Copy assignment constructor
    Canvas& operator = ( const Canvas& other );

  private:
    bool mustLock ( void* video_buffer ) const;
    /// SDL_Surface*
    std::shared_ptr<void> canvas_buffer;
    /// Holds surface position
    Coords coords;
    /// Holds surface bounds (input clipping)
    Coords offsets;
};


} // namespace nom

#endif // NOMLIB_SDL_CANVAS_HEADERS defined
