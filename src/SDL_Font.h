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
#include "Coords.h"
#include "Color.h"

namespace nom
{
  class SDL_Font: public SDL_Drawable
  {
    public:

      /// Default constructor; we initialize the SDL_ttf extension here
      SDL_Font ( void );

      /// Default destructor; we shutdown the SDL_ttf extension here
      ~SDL_Font ( void );

      const int32_t getX ( void ) const;
      const int32_t getY ( void ) const;
      const nom::Coords& getXY ( void ) const;
      void setX ( int32_t x_ );
      void setY ( int32_t y_ );
      void setXY ( int32_t x_, int32_t y_ );

      /// Obtains set text string buffer; defaults to \0
      const std::string& getText ( void ) const;

      /// \brief Set a new text string for drawing; defaults to \0
      ///
      /// NOTE: We render the font drawing surface here
      void setText ( std::string text );

      /// Compute the width in pixels of the set text string; defaults to zero (0)
      int32_t getTextWidth ( void );

      /// Compute the height in pixels of the set text string; defaults to zero (0)
      int32_t getTextHeight ( void );

      /// Obtain the text color previously set; defaults to zero ( 0, 0, 0, -1 )
      const nom::Color& getTextColor ( void ) const;

      /// Set a new text color
      void setTextColor ( const nom::Color &color );

      /// \brief Load a new TTF or FON from a file
      ///
      /// Support for the file format is determined
      /// by the SDL_ttf extension
      bool Load ( std::string filename, uint32_t size );

      /// Draw the set text string to the video surface
      void Draw ( void* video_buffer );

    private:
      /// Surface where font for drawing is rendered to
      nom::SDL_Canvas font_buffer;
      /// Font file data, used by SDL_ttf extension
      TTF_Font *font;
      /// Color of the font
      nom::Color text_color;
      /// holds contents of text as a string buffer
      std::string text_buffer;
      /// X, Y, width & height drawing coordinates
      nom::Coords coords;
  };
}
#endif // NOMLIB_SDL_TTF_FONT_HEADERS defined
