/******************************************************************************

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
#include <sstream>

#include <SDL/SDL.h>

#include "sdl/utils.hpp"
#include "math/Color.hpp"
#include "math/Transformable.hpp"
#include "gfx/SDL_Canvas.hpp"
#include "gfx/SDL_Drawable.hpp"
#include "nomlib_config.hpp"

namespace nom
{
  /// Text effect styling
  enum Style
  {
    Regular = 0,
    Bold = 1,
    Italic = 2,
    Underlined = 3,
    Faded = 4
  };

  class SDL_BitmapFont:// public SDL_Drawable, //  "is a" inheritance
                        public nom::Transformable //  "has a" inheritance
  {
    public:
      /// Default constructor
      SDL_BitmapFont ( void );

      /// Default destructor
      ~SDL_BitmapFont ( void );

      /// Obtains set text string buffer; defaults to \0
      const std::string& getText ( void ) const;

      /// Set a new text string for drawing; defaults to \0
      void setText ( const std::string& text );

      /// Compute the width in pixels of the set text string; defaults to zero (0)
      /// \internal
      /// TODO: I don't think this is entirely accurate; this->spacing - 2 is
      /// fudged ...
      ///
      /// We probably ought to be calculating the width based off the same algorithm as
      /// is shown in SDL_BitmapFont::Load
      /// \endinternal
      int32_t getTextWidth ( void );
      /// Compute the height in pixels of the set text string; defaults to zero (0)
      int32_t getTextHeight ( void );

      /// Obtain text character spacing width in pixels; defaults to given
      /// sheet_width argument divided by two (2)
      uint32_t getSpacing ( void );

      /// Set new text character spacing width in pixels
      void setSpacing ( uint32_t spaces );

      /// Obtain text character spacing height offsets in pixels ;defaults to variable
      /// calculations made within Load method
      uint32_t getNewline ( void );

      /// Set new text character spacing height offsets in pixels
      void setNewline ( uint32_t newline );

      uint8_t getStyle ( void ) const;
      void setStyle ( uint8_t style, uint8_t options = 150 );

      const nom::Coords findGlyph ( const std::string& glyph );
      /// \brief Loads a new bitmap font from a file
      /// \internal
      /// TODO: add spacing / padding so that we can export with black guidelines
      /// \endinternal
      bool Load ( const std::string& filename, const nom::Color& colorkey, uint32_t sheet_width = 16,
                  uint32_t sheet_height = 16 );

      void Update ( void );
      /// Draw the set text string to the video surface
      void Draw ( void* video_buffer );

    private:
      /// pointer reference holding our bitmap font image sheet
      nom::SDL_Canvas bitmap_font;
      /// individual chars positioning offsets within bitmap_font
      nom::Coords chars[256];
      /// height (in pixels) to offset when newline carriage char is encountered
      uint32_t newline;
      /// width (in pixels) to offset when a space carriage char is encountered
      uint32_t spacing;
      /// holds contents of text as a string
      std::string text_buffer;
      /// Current text effect set
      uint8_t text_style;
  };
}
#endif // NOMLIB_SDL_BITMAP_FONT_HEADERS defined
