/******************************************************************************
    SDL_BitmapFont.h

  SDL-based Bitmap Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

  Portions Copyright Lazy Foo' Productions 2004-2013
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_SDL_BITMAP_FONT_HEADERS
#define GAMELIB_SDL_BITMAP_FONT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"

#include "Color.h"

#include "gfx.h"

#include "Font.h"
// TODO: namespace nom
// Rename to SDL_BitmapFont
class SDLBitmapFont: public Font
{
  public:
    SDLBitmapFont ( void );
    ~SDLBitmapFont ( void );

    signed int getTextWidth ( void );
    signed int getTextHeight ( void );

    void setText ( std::string text );

    unsigned int getSpacing ( void );
    void setSpacing ( unsigned int spaces );

    unsigned int getNewline ( void );
    void setNewline ( unsigned int newline );

    void greyedOutText ( unsigned char opacity );

    bool Load ( std::string filename, nom::Color colorkey, unsigned int sheet_width = 16,
                unsigned int sheet_height = 16 );

    bool Draw ( SDL_Surface *video_buffer );

  private:
    SDL_Surface *bitmap_font; // pointer reference holding our bitmap font image sheet
    nom::Coords chars[256]; // individual chars within *bitmap_font
    unsigned int newline, spacing; // spacing variables

};

#endif // GAMELIB_SDL_BITMAP_FONT_HEADERS defined
