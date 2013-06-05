/******************************************************************************
    SDL_Font.h

  SDL-based TrueType Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_TTF_FONT_HEADERS
#define NOMLIB_SDL_TTF_FONT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

#include "gfx.h"

#include "Font.h"

namespace nom
{
  class SDL_Font: public Font
  {
    public:
      SDL_Font ( void );
      ~SDL_Font ( void );

      signed int getTextWidth ( void );
      signed int getTextHeight ( void );
      void setText ( std::string text );

      bool Load ( std::string filename, unsigned int size );
      bool Draw ( SDL_Surface *video_buffer );

    private:
      SDL_Surface *font_buffer; // surface where font gets rendered to
      TTF_Font *font; // the font file data
  };
}
#endif // NOMLIB_SDL_TTF_FONT_HEADERS defined
