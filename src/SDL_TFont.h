/******************************************************************************
    SDL_TFont.h

  SDL-based TrueType Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_SDL_TTF_FONT_HEADERS
#define GAMELIB_SDL_TTF_FONT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "Coords.h"

#include "gfx.h"

#include "Font.h"
// TODO: namespace nom
// Rename to SDL_Font
class SDL_TFont: public Font
{
  public:
    SDL_TFont ( void );
    ~SDL_TFont ( void );

    signed int getTextWidth ( void );
    signed int getTextHeight ( void );
    void setText ( std::string text );

    bool Load ( std::string filename, unsigned int size );
    bool Draw ( SDL_Surface *video_buffer );

  private:
    SDL_Surface *font_buffer; // surface where font gets rendered to
    TTF_Font *font; // the font file data
};

#endif // GAMELIB_SDL_TTF_FONT_HEADERS defined
