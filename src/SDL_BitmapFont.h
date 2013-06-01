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

// TODO: namespace nom
class SDLBitmapFont
{
  public:
    SDLBitmapFont ( void );
    ~SDLBitmapFont ( void );

    signed int getX ( void );
    signed int getY ( void );
    GCoords getXY ( void );
    void setX ( signed int x );
    void setY ( signed int y );
    void setXY ( signed int x, signed int y );

    unsigned int getTextWidth ( void );
    unsigned int getTextHeight ( void );
    std::string getText ( void );
    void setText ( std::string text ); // TODO: rename to setText

    unsigned int getSpacing ( void );
    void setSpacing ( unsigned int spaces );

    unsigned int getNewline ( void );
    void setNewline ( unsigned int newline );

    void greyedOutText ( unsigned char opacity );

    bool Load ( std::string filename, GColor colorkey, unsigned int sheet_width = 16,
                unsigned int sheet_height = 16 );

    bool Draw ( SDL_Surface *video_buffer );

  private:
    SDL_Surface *bitmap_font; // pointer reference holding our bitmap font image sheet

    SDL_Rect chars [ 256 ]; // individual chars within *bitmap_font
    std::string text_buffer; // string of text for blitting
    unsigned int newline, spacing; // spacing variables
    GCoords coords; // x, y blitting coordinates
};

#endif // GAMELIB_SDL_BITMAP_FONT_HEADERS defined
