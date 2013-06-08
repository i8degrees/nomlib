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

#include "SDL_Drawable.hpp"
#include "SDL_Canvas.hpp"

namespace nom
{
  class SDL_Font: public SDL_Drawable
  {
    public:
      SDL_Font ( void );
      ~SDL_Font ( void );

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

      const nom::Color& getTextColor ( void ) const;
      void setTextColor ( const nom::Color &color );

      bool Load ( std::string filename, uint32_t size );
      void Draw ( void* video_buffer );

    private:
      nom::SDL_Canvas font_buffer; /// surface where font gets rendered to
      TTF_Font *font; /// the font file data
      nom::Color text_color; /// the color of the font
      std::string text_buffer; /// holds contents of text as a string
      nom::Coords coords; /// X, Y, width & height blitting coordinates
  };
}
#endif // NOMLIB_SDL_TTF_FONT_HEADERS defined
