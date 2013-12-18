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
#ifndef NOMLIB_SDL2_BITMAP_FONT_HEADERS
#define NOMLIB_SDL2_BITMAP_FONT_HEADERS

#include <iostream>
#include <string>
#include <sstream>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IFont.hpp"
#include "nomlib/graphics/Glyph.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/system/SDL_helpers.hpp"

//#define NOM_DEBUG_SDL2_BITMAP_FONT

namespace nom {

class BitmapFont: public IFont
{
  public:
    /// Default constructor
    BitmapFont ( void );

    /// Default destructor
    ~BitmapFont ( void );

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    enum IFont::FileType type ( void ) const;

    SDL_SURFACE::RawPtr image ( void ) const;

    /// Obtain text character spacing width in pixels; this variable is affected
    /// by the total image width size.
    uint spacing ( void ) const;

    /// Set new text character spacing width (in pixels) -- this variable is
    /// used during the calculation of the text width; see
    /// nom::BitmapFont::width method.
    /// in addition to the rendering process (see draw method) when there is a
    /// space character (' ') found in the provided text string.
    void set_spacing ( uint spaces );

    /// Obtain text character spacing height offsets in pixels; defaults to
    /// variable calculations made within Load method
    uint newline ( void ) const;

    /// Set new text character spacing height offsets in pixels
    void set_newline ( uint newline );

    const IntRect& glyph ( uint32 character );

    /// Loads a new bitmap font from a file
    ///
    /// \todo Add spacing / padding so that we can export with black guidelines
    bool load (
                const std::string& filename, const Color4u& colorkey,
                bool use_cache = false
              );

  private:
    /// Trigger a build of the font characteristics gleaned from the image file;
    /// recalculate the character sizes, coordinate origins, spacing, etc.
    bool build ( void );

    /// Width -- in pixels -- of overall texture atlas sheet
    ///
    /// \todo Change type to int
    sint sheet_width;

    /// Height -- in pixels -- of overall texture atlas sheet
    ///
    /// \todo Change type to int
    sint sheet_height;

    /// Our bitmap font's bitmap atlas
    Image bitmap_font_;

    /// individual chars positioning offsets within bitmap_font
    Glyph::GlyphMap glyphs;

    /// Height (in pixels) to offset when newline carriage char is encountered
    uint newline_;

    /// Width in pixels to offset when a space carriage char is encountered.
    ///
    /// Note that you may need to reset this if you are using bitmap fonts with
    /// high resolution graphics. I recently went from 384x224 to 768x448 and
    /// this was enough to offset this variable by 18 pixels.
    uint spacing_;

    enum IFont::FileType type_;
};

namespace priv {

/// \todo FIXME; we need to figure out how to free this resource when we are
/// using it within the MessageBox class -- we are leaking kilobytes as-is.
void Free_BitmapFont ( BitmapFont* ptr );

} // namespace priv
} // namespace nom

#endif // include guard defined
