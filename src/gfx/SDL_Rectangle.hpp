/******************************************************************************

    SDL Rectangle Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_RECTANGLE_HEADERS
#define NOMLIB_SDL_RECTANGLE_HEADERS

#include "config.hpp"
#include "sys/Logger.hpp"
#include "sdl/utils.hpp"
#include "math/Color.hpp"
#include "math/Coords.hpp"
#include "math/Transformable.hpp"
#include "gfx/SDL_Drawable.hpp"

namespace nom {

class Rectangle:  public SDL_Drawable,   // "is a" relationship
                  public Transformable   // "has a" relationship

{
  public:
    Rectangle ( void );
    Rectangle ( const Rectangle& rect );
    Rectangle ( const Coords& coords, const Color& color );
    virtual ~Rectangle ( void );

    void Update ( void );
    void Draw ( void* ) const;
};


} // namespace nom

#endif // NOMLIB_SDL_RECTANGLE_HEADERS defined
