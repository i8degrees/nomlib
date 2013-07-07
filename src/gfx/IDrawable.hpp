/******************************************************************************

    Drawable abstract class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_IDRAWABLE_HEADERS
#define NOMLIB_IDRAWABLE_HEADERS

#include "nomlib_config.hpp"

namespace nom
{
  // This is an inheritance-only class
  class IDrawable
  {
    protected:
      virtual void Update ( void ) = 0;
      virtual void Draw ( void* ) const = 0;

      IDrawable ( void ) {}

      virtual ~IDrawable ( void ) {}
  };
}

#endif // NOMLIB_IDRAWABLE_HEADERS defined
