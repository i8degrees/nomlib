/******************************************************************************

    Drawable interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_DRAWABLE_HEADERS
#define NOMLIB_SDL_DRAWABLE_HEADERS

#include "config.hpp"
#include "gfx/IDrawable.hpp"

namespace nom {

// This is an inheritance-only class
class SDL_Drawable: public IDrawable
{
  public:
    virtual void Update ( void ) = 0;
    virtual void Draw ( void* ) const = 0;

    SDL_Drawable ( void ) {}
    virtual ~SDL_Drawable ( void ) {}
};


} // namespace nom

#endif // NOMLIB_SDL_DRAWABLE_HEADERS defined
