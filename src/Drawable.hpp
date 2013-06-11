/******************************************************************************
    Drawable.hpp

    Drawables abstract class interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_DRAWABLE_HEADERS
#define NOMLIB_DRAWABLE_HEADERS

#include <iostream>
#include <string>

#include "gamelib.h"

// Example usage:
// https://gist.github.com/i8degrees/5711070

namespace nom
{
  // This is an inheritance-only class
  class Drawable
  {
    protected:
      virtual void Update ( void ) = 0;
      virtual void Draw ( void* ) = 0;

      Drawable ( void )
      {
        #ifdef DEBUG_DRAWABLE_OBJ
          std::cout << "Drawable::Drawable(): Hello, world!" << std::endl << std::endl;
        #endif
      }

      virtual ~Drawable ( void )
      {
        #ifdef DEBUG_DRAWABLE_OBJ
          std::cout << "Drawable::~Drawable(): Goodbye cruel world!" << std::endl << std::endl;
        #endif
      }
  };
}

#endif // NOMLIB_DRAWABLE_HEADERS defined
