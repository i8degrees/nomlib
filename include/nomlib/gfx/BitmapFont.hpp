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

#include "nomlib/config.hpp"
#include "nomlib/sys/Logger.hpp"
#include "nomlib/gfx/IFont.hpp"
#include "nomlib/sdl/utils.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/gfx/Canvas.hpp"
#include "nomlib/gfx/IDrawable.hpp"

namespace nom {

class BitmapFont: //  public IDrawable, //  "is a" inheritance
                          public IFont // "is-a" relationship
{
  public:
    /// Default constructor
    BitmapFont ( void );

    /// Default destructor
    ~BitmapFont ( void );

    /// Obtains set text string buffer; defaults to \0
    const std::string& getText ( void ) const;

    /// Set a new text string for drawing; defaults to \0
    void setText ( const std::string& text );

    /// Compute the width in pixels of the set text string; defaults to zero (0)
    /// \internal
    /// TODO: I don't think this is entirely accurate; this->spacing - 2 is
    /// fudged ...
    ///
    /// We probably ought to be calculating the width based off the same
    /// algorithm as is shown in ::Load
    /// \endinternal
    int32 getFontWidth ( void ) const;

    /// Compute the height in pixels of the set text string; defaults to zero (0)
    int32 getFontHeight ( void ) const;

    /// Obtain text character spacing width in pixels; defaults to given
    /// sheet_width argument divided by two (2)
    uint32 getSpacing ( void );

    /// Set new text character spacing width in pixels
    void setSpacing ( uint32_t spaces );

    /// Obtain text character spacing height offsets in pixels; defaults to
    /// variable calculations made within Load method
    uint32 getNewline ( void );

    /// Set new text character spacing height offsets in pixels
    void setNewline ( uint32_t newline );

    FontStyle getFontStyle ( void ) const;
    void setFontStyle ( uint8 style, uint8 options = 150 );

    void setFontSize ( int32 point_size );

    const Coords findGlyph ( const std::string& glyph );
    /// \brief Loads a new bitmap font from a file
    /// \internal
    /// TODO: add spacing / padding so that we can export with black guidelines
    /// \endinternal
    bool Load ( const std::string& filename, const Color& colorkey,
                int32 font_size = 12, bool use_cache = 0
              );

    void Update ( void );
    /// Draw the set text string to the video surface
    void Draw ( void* video_buffer ) /*const*/;

  private:
    /// pointer reference holding our bitmap font image sheet
    Canvas bitmap_font;
    /// individual chars positioning offsets within bitmap_font
    Coords chars[256];
    /// height (in pixels) to offset when newline carriage char is encountered
    uint32 newline;
    /// width (in pixels) to offset when a space carriage char is encountered
    uint32 spacing;
    /// holds contents of text as a string
    std::string text_buffer;
    /// Current text effect set
    enum FontStyle text_style;

    /// Not implemented (yet)
    int32 font_size;

    uint32 sheet_width;
    uint32 sheet_height;
};


} // namespace nom

#endif // include guard defined
