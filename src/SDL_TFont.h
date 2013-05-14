/******************************************************************************
    SDL_TFont.h

  SDL-based TrueType Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_SDL_TTF_FONT_HEADERS
#define GAMELIB_SDL_TTF_FONT_HEADERS

#include <iostream>
#include <string>

#include "SDL_ttf.h"

#include "GCoords.h"

#include "gfx.h"
class Gfx;

class SDL_TFont
{
  public:
    SDL_TFont ( void );
    ~SDL_TFont ( void );

    unsigned int getTextWidth ( void );
    unsigned int getTextHeight ( void );
    SDL_Color getTextColor ( void );
    std::string getTextBuffer ( void );

    void setTextColor ( unsigned r, unsigned g, unsigned b );
    void setTextBuffer ( std::string text );

    bool Load ( std::string filename, unsigned int size );
    bool Draw ( Gfx *engine, unsigned int x, unsigned int y );

  private:
    TTF_Font *font;
    SDL_Color text_color;
    std::string text_buffer;
    GCoords coords;
};

#endif // GAMELIB_SDL_TTF_FONT_HEADERS defined
