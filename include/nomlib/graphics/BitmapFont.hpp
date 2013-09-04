/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Portions Copyright (c) 2004-2013 Lazy Foo' Productions [1]
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

1. http://lazyfoo.net/SDL_tutorials/lesson30/index.php

******************************************************************************/
#ifndef NOMLIB_SDL_BITMAP_FONT_HEADERS
#define NOMLIB_SDL_BITMAP_FONT_HEADERS

#include <iostream>
#include <string>
#include <sstream>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IFont.hpp"
#include "nomlib/sdl/utils.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/graphics/Canvas.hpp"

namespace nom {

class BitmapFont: public IFont
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
    /// algorithm as is shown in the load method
    /// \endinternal
    int32 getFontWidth ( void ) const;

    /// Compute the height in pixels of the set text string; defaults to zero (0)
    int32 getFontHeight ( void ) const;

    const Color& getColor ( void ) const;
    const Coords& getPosition ( void ) const;

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

    void setColor ( const Color& color );
    void setPosition ( const Coords& coords );

    const Coords findGlyph ( const std::string& glyph );
    /// \brief Loads a new bitmap font from a file
    /// \internal
    /// TODO: add spacing / padding so that we can export with black guidelines
    /// \endinternal
    bool load ( const std::string& filename, const Color& colorkey,
                bool use_cache = false
              );

    void Update ( void );
    /// Draw the set text string to the video surface
    void Draw ( void* video_buffer ) const;

    /// Rescale the font with a chosen resizing algorithm
    ///
    /// Re-implements IFont::resize
    bool resize ( enum ResizeAlgorithm scaling_algorithm );

  private:
    /// Trigger a rebuild of the font characteristics gleaned from the image file;
    /// recalculate the character sizes, coordinate origins, spacing, etc.
    bool rebuild ( void );

    const int32 sheet_width;
    const int32 sheet_height;

    /// pointer reference holding our bitmap font image sheet
    mutable Canvas bitmap_font;

    /// Background color used in computation of individual character boundaries.
    Color colorkey;

    /// individual chars positioning offsets within bitmap_font
    Coords chars[256];

    /// Positioning coordinates
    Coords coords;

    /// height (in pixels) to offset when newline carriage char is encountered
    uint32 newline;

    /// Width in pixels to offset when a space carriage char is encountered.
    ///
    /// Note that you may need to reset this if you are using bitmap fonts with
    /// high resolution graphics. I recently went from 384x224 to 768x448 and
    /// this was enough to offset this variable by 18 pixels.
    uint32 spacing;

    /// holds contents of text as a string
    std::string text_buffer;

    /// Current text effect set
    enum FontStyle text_style;

    /// Not implemented (yet)
    Color color;
};


} // namespace nom

#endif // include guard defined
