/******************************************************************************

    Drawable interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_DRAWABLE_HEADERS
#define NOMLIB_SDL_DRAWABLE_HEADERS

#include "gfx/IDrawable.hpp"
#include "nomlib_config.hpp"

namespace nom
{
  // This is an inheritance-only class
  class SDL_Drawable: public nom::IDrawable
  {
    public:
      virtual void Update ( void ) = 0;
      virtual void Draw ( void* ) const = 0;

      SDL_Drawable ( void ) {}
      virtual ~SDL_Drawable ( void ) {}
  };
}

#endif // NOMLIB_SDL_DRAWABLE_HEADERS defined
