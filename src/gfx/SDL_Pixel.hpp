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
    virtual ~Pixel ( void );
    Pixel ( const Coords& coords, const Color& color );
    Pixel ( int32_t x, int32_t y, const Color& color );
    void Update ( void );

    void Draw ( void* video_buffer ) const;
};


} // namespace nom

#endif // NOMLIB_SDL_PIXEL_HEADERS defined
