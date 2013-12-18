/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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

******************************************************************************/
#ifndef NOMLIB_SDL2_TRUETYPE_FONT_HEADERS
#define NOMLIB_SDL2_TRUETYPE_FONT_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include <SDL_ttf.h>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/IFont.hpp"
#include "nomlib/graphics/Glyph.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/Window.hpp"
#include "nomlib/system/SDL_helpers.hpp"

namespace nom {

/// \brief TrueType fonts renderer
class TrueTypeFont:
                    public IFont
{
  public:
    /// Default constructor; we initialize the SDL_ttf extension here
    TrueTypeFont ( void );

    /// Default destructor; we shutdown the SDL_ttf extension here
    ///
    /// \FIXME Resetting the font pointer is a temporary workaround patch;
    /// SDL_App destructs long before TrueTypeFont does in TTcards, which
    /// doesn't allow us to free our font resources here properly.
    ~TrueTypeFont ( void );

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    enum IFont::FileType type ( void ) const;

    SDL_SURFACE::RawPtr image ( void ) const;

    /// Obtain text character spacing width in pixels; this variable is affected
    /// by the total image width size.
    uint spacing ( void ) const;

    sint font_size ( void ) const;

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

    /// Getter for obtaining the vector outline of the loaded font.
    //int32 getFontOutline ( void ) const;

    /// Set a new vector outline -- in pixels -- for the loaded font.
    //void setFontOutline ( int32 depth );

    const IntRect& glyph ( uint32 character );

    /// \brief Load a new font in from a file.
    ///
    /// Refer to the SDL_ttf documentation for file formats supported. As of
    /// this writing, TTF and FON file formats are known to be supported.
    bool load ( const std::string& filename, const Color4u& colorkey,
                bool use_cache = false
              );

  private:
    /// Trigger a rebuild of the font metrics from the current font; this
    /// recalculates character sizes, coordinate origins, spacing, etc.
    bool build ( void );

    /// Surface where font for drawing is rendered to
    Texture font_buffer_;

    /// Font file data, used by SDL_ttf extension
    std::shared_ptr<TTF_Font> font_;

    /// individual chars positioning offsets within bitmap_font
    ///
    /// \todo Implement
    Glyph::GlyphMap glyphs_;

    enum IFont::FileType type_;

    /// Font point (pixel) size; defaults to 12
    sint font_size_;

    /// Height (in pixels) to offset when newline carriage char is encountered
    uint newline_;

    /// Width in pixels to offset when a space carriage char is encountered.
    ///
    /// Note that you may need to reset this if you are using bitmap fonts with
    /// high resolution graphics. I recently went from 384x224 to 768x448 and
    /// this was enough to offset this variable by 18 pixels.
    uint spacing_;

    /// Store the file path so we can change font sizes on the fly
    std::string filename_;

    /// Whether or not to use caching features of nom::ObjectCache
    bool use_cache_;
};

namespace priv {

/// \todo FIXME; we need to figure out how to free this resource when we are
/// using it within the MessageBox class -- we are leaking kilobytes as-is.
void Free_TrueTypeFont ( TrueTypeFont* ptr );

} // namespace priv
} // namespace nom

#endif // include guard defined

/// \class nom::TrueTypeFont
/// \ingroup graphics
///
///   [TO BE WRITTEN]
///
/// \todo Re-write the class to render fonts from a cached nom::Texture source,
/// much like we do in nom::BitmapFont or nom::SpriteBatch.
