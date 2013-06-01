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

// TODO: namespace nom
class SDL_TFont
{
  public:
    SDL_TFont ( void );
    ~SDL_TFont ( void );

    signed int getX ( void );
    signed int getY ( void );
    GCoords getXY ( void );
    void setX ( signed int x );
    void setY ( signed int y );
    void setXY ( signed int x, signed int y );

    std::string getText ( void );
    unsigned int getTextWidth ( void );
    unsigned int getTextHeight ( void );
    GColor getTextColor ( void );

    void setText ( std::string text );
    void setTextColor ( unsigned r, unsigned g, unsigned b );

    bool Load ( std::string filename, unsigned int size );
    bool Draw ( SDL_Surface *video_buffer );

  private:
    //SDL_Surface *ttf_font;
    TTF_Font *font;
    GColor text_color;
    std::string text_buffer;
    GCoords coords; // X, Y blitting coordinates
};

#endif // GAMELIB_SDL_TTF_FONT_HEADERS defined
