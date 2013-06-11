/******************************************************************************
    SDL_Drawable.hpp

    Drawables interface implementation

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_DRAWABLE_HEADERS
#define NOMLIB_SDL_DRAWABLE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Drawable.hpp"
#include "gamelib.h"

// Example usage:
// https://gist.github.com/i8degrees/5711070

namespace nom
{
  // This is an inheritance-only class
  class SDL_Drawable: public nom::Drawable
  {
    public:
      virtual void Draw ( void* ) = 0;

      SDL_Drawable ( void )
      {
        #ifdef DEBUG_SDL_DRAWABLE_OBJ
          std::cout << "SDL_Drawable::SDL_Drawable(): Hello, world!" << std::endl << std::endl;
        #endif
      }
      virtual ~SDL_Drawable ( void )
      {
        #ifdef DEBUG_SDL_DRAWABLE_OBJ
          std::cout << "SDL_Drawable::~SDL_Drawable(): Goodbye cruel world!" << std::endl << std::endl;
        #endif
      }
  };
}

#endif // NOMLIB_SDL_DRAWABLE_HEADERS defined
