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

#include "GColor.h"

#include "gfx.h"

class Gfx;

class SDLBitmapFont
{
  public:
    SDLBitmapFont ( void );
    ~SDLBitmapFont ( void );

    unsigned int getTextWidth ( void );
    unsigned int getTextHeight ( void );
    std::string getTextBuffer ( void );
    void setTextBuffer ( std::string text );

    unsigned int getSpacing ( void );
    void setSpacing ( unsigned int spaces );

    unsigned int getNewline ( void );
    void setNewline ( unsigned int newline );

    bool Load ( std::string filename, GColor colorkey, unsigned int sheet_width = 16,
                unsigned int sheet_height = 16 );

    bool Draw ( Gfx *engine, unsigned int x, unsigned int y );

  private:
    SDL_Surface *bitmap_font; // pointer reference holding our bitmap font image sheet

    SDL_Rect chars [ 256 ]; // individual chars within *bitmap_font
    std::string text_buffer; // string of text for blitting
    unsigned int newline, spacing; // spacing variables
};

#endif // GAMELIB_SDL_BITMAP_FONT_HEADERS defined
