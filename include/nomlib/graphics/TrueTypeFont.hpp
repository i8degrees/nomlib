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
#include "nomlib/graphics/IFont.hpp"
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
    ~TrueTypeFont ( void );

    /// Return a std::shared_ptr copy of this instance
    IFont::SharedPtr clone ( void ) const;

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    /// Obtains set text string buffer; defaults to \0
    const std::string& getText ( void ) const;

    /// Compute the width in pixels of the set text string; defaults to zero (0)
    int32 getFontWidth ( void ) const;

    /// Compute the height in pixels of the set text string; defaults to zero (0)
    int32 getFontHeight ( void ) const;

    IFont::FontStyle getFontStyle ( void ) const;

    const Color4u& getColor ( void ) const;
    const Coords& getPosition ( void ) const;

    /// Not implemented
    uint32 getNewline ( void ) const;

    /// Not implemented
    uint32 getSpacing ( void ) const;

    IFont::TextAlignment getTextJustification ( void ) const;

    void setFontStyle ( int32 style, uint8 options = 150 );

    /// Set a new text point size
    void setFontSize ( int32 point_size );

    /// \brief Set a new text string for drawing; defaults to \0
    ///
    /// NOTE: We render the font drawing surface here
    void setText ( const std::string& text );

    void setColor ( const Color4u& color );
    void setPosition ( const Coords& coords );

    /// Not implemented
    void setSpacing ( uint32 spaces );

    /// Not implemented.
    void setTextJustification ( IFont::TextAlignment alignment );

    /// Getter for obtaining the vector outline of the loaded font.
    int32 getFontOutline ( void ) const;

    /// Set a new vector outline -- in pixels -- for the loaded font.
    void setFontOutline ( int32 depth );

    /// Getter for obtaining the current font rendering style in use
    IFont::RenderStyle getRenderingStyle ( void ) const;

    /// Set a new font rendering style.
    void setRenderingStyle ( IFont::RenderStyle );

    /// \brief Load a new font in from a file.
    ///
    /// Refer to the SDL_ttf documentation for file formats supported. As of
    /// this writing, TTF and FON file formats are known to be supported.
    bool load ( const std::string& filename, const Color4u& colorkey,
                bool use_cache = false
              );

    void update ( void );

    /// Draw the set text string to the video surface
    void draw ( RenderTarget target ) const;

  private:
    /// Trigger a rebuild of the font metrics from the current font; this
    /// recalculates character sizes, coordinate origins, spacing, etc.
    bool rebuild ( void );

    /// Surface where font for drawing is rendered to
    Texture font_buffer;

    /// Font file data, used by SDL_ttf extension
    std::shared_ptr<TTF_Font> font;

    /// Positioning coordinates (including width and height)
    Coords coords;

    /// Text color
    Color4u color;

    /// holds contents of text as a string buffer
    std::string text_buffer;

    /// Current text effect set
    FontStyle text_style;
    uint8 style_options;

    /// Store the file path so we can change font sizes on the fly
    std::string filename;

    /// Font point (pixel) size; defaults to 12
    int32 font_size;

    /// Whether or not to use caching features of nom::ObjectCache
    bool use_cache;

    enum IFont::TextAlignment text_alignment;

    enum IFont::RenderStyle rendering;
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
