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

//#define NOM_DEBUG_SDL_BITMAP_FONT

namespace nom {

class BitmapFont: public IFont
{
  public:
    /// Default constructor
    BitmapFont ( void );

    /// Default destructor
    ~BitmapFont ( void );

    /// Return a std::shared_ptr copy of this instance
    IFont::SharedPtr clone ( void ) const;

    /// Obtains set text string buffer; defaults to \0
    const std::string& getText ( void ) const;

    /// Set a new text string for drawing; defaults to \0
    void setText ( const std::string& text );

    /// Obtain the text width in pixels of the set text string.
    ///
    /// This calculation mimics the rendering calculations done and should be
    /// exact within one pixel accuracy.
    ///
    /// \todo Rename me to text_width
    /// \todo Support multi-line texts
    int32 getFontWidth ( void ) const;

    /// Obtain the text height in pixels of the set text string.
    ///
    /// This calculation mimics the rendering calculations done and should be
    /// exact within one pixel accuracy.
    ///
    /// \todo Rename me to text_height
    int32 getFontHeight ( void ) const;

    const Color& getColor ( void ) const;
    const Coords& getPosition ( void ) const;

    /// Obtain text character spacing width in pixels; this variable is affected
    /// by the total image width size.
    uint32 getSpacing ( void ) const;

    /// Set new text character spacing width (in pixels) -- this variable is
    /// used during the calculation of the text width (see getFontWidth method)
    /// in addition to the rendering process (see draw method) when there is a
    /// space character (' ') found in the provided text string.
    void setSpacing ( uint32 spaces );

    /// Obtain text character spacing height offsets in pixels; defaults to
    /// variable calculations made within Load method
    uint32 getNewline ( void ) const;

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

    int32 sheet_width;
    int32 sheet_height;

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

  namespace priv {

/// \todo FIXME; we need to figure out how to free this resource when we are
/// using it within the MessageBox class -- we are leaking kilobytes as-is.
void Free_BitmapFont ( BitmapFont* ptr );


  } // namespace priv
} // namespace nom

#endif // include guard defined
