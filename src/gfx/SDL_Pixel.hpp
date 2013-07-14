/******************************************************************************

    SDL Point (Pixel) Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_PIXEL_HEADERS
#define NOMLIB_SDL_PIXEL_HEADERS

#include "config.hpp"
#include "sdl/utils.hpp"
#include "math/Color.hpp"
#include "math/Coords.hpp"
#include "math/Transformable.hpp"
#include "gfx/SDL_Drawable.hpp"

namespace nom {

class Pixel:  public SDL_Drawable,   // "is a" relationship
              public Transformable   // "has a" relationship
{
  public:
    Pixel ( void );
    Pixel ( const Coords& coords, const Color& color );
    Pixel ( int32 x, int32 y, const Color& color );
    virtual ~Pixel ( void );

    void Update ( void );

    /// Pixel blitting -- supports 8-bit, 15/16-bit, 24-bit & 32-bit color modes
    ///
    /// If an unsupported canvas is detected, returns without attempting to
    /// write
    ///
    /// You are responsible for locking & unlocking of the canvas before-hand
    ///
    /// \todo Test 8-bit, 15/16-bit & 24-bit pixels
    void Draw ( void* video_buffer ) const;
};


} // namespace nom

#endif // NOMLIB_SDL_PIXEL_HEADERS defined
