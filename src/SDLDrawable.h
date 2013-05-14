/******************************************************************************
    SDLDrawable.h

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_SDL_DRAWABLE_HEADERS
#define GAMELIB_SDL_DRAWABLE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

//#include "GCoords.h"

#define DEBUG_SDL_DRAWABLE
#define DEBUG_SDL_DRAWABLE_OBJ

/*

  std::list<SDLDrawable*> drawables;

class SDLDrawable
{
  public:
    // ...
  private:
    // ...
};
class SDLGradient: public SDLDrawable
{
  public:
    // ...
  private:
    // ...
};
class SDLFont: public SDLDrawable
{
  public:
    // ...
  private:
    // ...
};

*/

class SDLDrawable
{
  public:
    virtual bool Draw ( Gfx *engine, unsigned int x, unsigned int y ) const = 0;
};

#endif // GAMELIB_SDL_DRAWABLE_HEADERS defined
