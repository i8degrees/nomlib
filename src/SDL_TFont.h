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

#include "Coords.h"

#include "gfx.h"

// TODO: namespace nom
class SDL_TFont
{
  public:
    SDL_TFont ( void );
    ~SDL_TFont ( void );

    signed int getX ( void );
    signed int getY ( void );
    nom::Coords getXY ( void );
    void setX ( signed int x_ );
    void setY ( signed int y_ );
    void setXY ( signed int x_, signed int y_ );

    std::string getText ( void );
    signed int getTextWidth ( void );
    signed int getTextHeight ( void );

    void setText ( std::string text );
    nom::Color getTextColor ( void );
    void setTextColor ( unsigned r, unsigned g, unsigned b );
    void setTextColor ( const nom::Color &color );

    bool Load ( std::string filename, unsigned int size );
    bool Draw ( SDL_Surface *video_buffer );

  private:
    //SDL_Surface *ttf_font;
    TTF_Font *font;
    nom::Color text_color;
    std::string text_buffer;
    nom::Coords coords; // X, Y blitting coordinates
};

#endif // GAMELIB_SDL_TTF_FONT_HEADERS defined
