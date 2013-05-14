/******************************************************************************
    Font.h

    SDL-based Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_FONT_HEADERS
#define GAMELIB_FONT_HEADERS

#include <iostream>
#include <string>

#include "SDL_ttf.h"

#include "gfx.h"

//class Gfx;

#include "SDLDrawable.h"

class Font: public SDLDrawable
{
  public:
    std::string getTextBuffer ( void ) const;
    bool Draw ( Gfx *engine, unsigned int x, unsigned int y ) const;
  private:
    TTF_Font *font;
    unsigned int text_color;
    std::string text_buffer;
};

#endif // GAMELIB_FONT_HEADERS defined
