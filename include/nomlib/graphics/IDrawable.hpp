/******************************************************************************

    Drawable abstract class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_IDRAWABLE_HEADERS
#define NOMLIB_IDRAWABLE_HEADERS

#include "nomlib/config.hpp"

namespace nom {

// This is an inheritance-only class
class IDrawable
{
  public:
    IDrawable ( void ) {}
    virtual ~IDrawable ( void ) {}

    /// Intended to be used internally by the class re-implementing this method;
    /// for use in preparing the buffer to be drawn using the Draw method call.
    /// Whereas both Update & Draw are typically publicly exposed to the
    /// end-user, only the Draw method call should be expected of the end-user
    /// to be explicitly called.
    virtual void Update ( void ) = 0;

    /// Draw the prepared video buffer contents of this object
    virtual void Draw ( void* ) const = 0;
};


} // namespace nom

#endif // NOMLIB_IDRAWABLE_HEADERS defined
