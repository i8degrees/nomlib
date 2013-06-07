/******************************************************************************
    SDL_BitmapFont.h

  SDL-based Bitmap Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

  Portions Copyright Lazy Foo' Productions 2004-2013
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_BITMAP_FONT_HEADERS
#define NOMLIB_SDL_BITMAP_FONT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Color.h"

#include "gfx.h"

#include "SDLDrawable.h"

namespace nom
{
  class SDL_BitmapFont: public SDL_Drawable
  {
    public:
      SDL_BitmapFont ( void );
      ~SDL_BitmapFont ( void );

      const int32_t getX ( void ) const;
      const int32_t getY ( void ) const;
      const nom::Coords& getXY ( void ) const;
      void setX ( int32_t x_ );
      void setY ( int32_t y_ );
      void setXY ( int32_t x_, int32_t y_ );

      const std::string& getText ( void ) const;

      int32_t getTextWidth ( void );
      int32_t getTextHeight ( void );

      void setText ( std::string text );

      uint32_t getSpacing ( void );
      void setSpacing ( uint32_t spaces );

      uint32_t getNewline ( void );
      void setNewline ( uint32_t newline );

      void greyedOutText ( u_char opacity );

      bool Load ( std::string filename, const nom::Color& colorkey, uint32_t sheet_width = 16,
                  uint32_t sheet_height = 16 );

      void Draw ( void* video_buffer ) const;

    private:
      SDL_Surface *bitmap_font; /// pointer reference holding our bitmap font image sheet
      nom::Coords chars[256]; /// individual chars within *bitmap_font
      uint32_t newline, spacing; /// spacing variables
      std::string text_buffer; /// holds contents of text as a string
      nom::Coords coords; /// X, Y, width & height blitting coordinates
  };
}
#endif // NOMLIB_SDL_BITMAP_FONT_HEADERS defined
