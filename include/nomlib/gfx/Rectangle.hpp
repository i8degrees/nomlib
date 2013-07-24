/******************************************************************************

    SDL Rectangle Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_RECTANGLE_HEADERS
#define NOMLIB_SDL_RECTANGLE_HEADERS

#include "nomlib/config.hpp"
#include "nomlib/sys/Logger.hpp"
#include "nomlib/sdl/utils.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/gfx/IDrawable.hpp"

namespace nom {

class Rectangle:  public IDrawable,     // "is a" relationship
                  public Transformable  // "has a" relationship

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
