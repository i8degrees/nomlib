/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/graphics/fonts/IFont.hpp"
#include "nomlib/graphics/fonts/FontMetrics.hpp"
#include "nomlib/graphics/fonts/FontPage.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/Image.hpp"
#include "nomlib/graphics/RenderWindow.hpp"
#include "nomlib/system/SDL_helpers.hpp"

// Dump glyph bounding coordinates & save source, destination sheets as PNG
// files
//#define NOM_DEBUG_SDL2_TRUE_TYPE_FONT_GLYPHS

// Dump the individual glyph bitmaps as PNG files
//#define NOM_DEBUG_SDL2_TRUE_TYPE_FONT_GLYPHS_PNG

namespace nom {

/// \brief TrueType fonts renderer
class TrueTypeFont: public IFont
{
  public:
    typedef TrueTypeFont self_type;
    typedef self_type* RawPtr;
    typedef std::shared_ptr<self_type> SharedPtr;

    /// \brief Default constructor; initialize the object to sane defaults.
    ///
    /// \remarks The SDL2_TTF extension must first be initialized; see nom::init.
    TrueTypeFont ( void );

    /// \brief Destructor
    ///
    /// \remarks The SDL2_TTF extension should be shutdown using nom::quit
    ~TrueTypeFont ( void );

    /// \brief Copy constructor
    TrueTypeFont ( const TrueTypeFont& copy );

    /// \brief Construct a clone of the existing instance
    IFont::raw_ptr clone( void ) const;

    /// \brief Validity check
    bool valid ( void ) const;

    enum IFont::FontType type ( void ) const;

    TTF_Font* font ( void ) const;

    /// Obtain the texture atlas used internally for rendering glyphs from
    ///
    /// \remarks It is assumed that the requested character size is already
    /// loaded into memory (using TrueTypeFont::set_point_size).
    const Image& image ( uint32 character_size ) const;

    /// \brief Obtain text character spacing width in pixels
    ///
    /// \returns  The width applied when the space carriage is encountered when
    ///           rendered.
    sint spacing ( uint32 character_size ) const;

    int point_size( void ) const;

    /// \brief Obtain font's line spacing
    ///
    /// \param character_size Point size in pixels
    ///
    /// \returns  Height offset in pixels
    int newline( uint32 character_size ) const;

    int kerning( uint32 first_char, uint32 second_char, uint32 character_size ) const;

    /// \brief Get the font's hinting style.
    ///
    /// \returns The hinting type, one of: TTF_HINTING_NORMAL, TTF_HINTING_LIGHT,
    /// TTF_HINTING_MONO or TTF_HINTING_NONE.
    ///
    /// \remarks The default hinting is TTF_HINTING_NONE.
    int hinting( void ) const;

    /// \brief Obtain a glyph
    ///
    /// \param    codepoint        ASCII character to lookup
    /// \param    character_size   Reserved for future implementation
    ///
    /// \returns  nom::Glyph structure
    const Glyph& glyph ( uint32 codepoint, uint32 character_size ) const;

    /// \brief Obtain font's outline size
    ///
    /// \param character_size The point size to retrieve the outline size for
    ///
    /// \returns Outline size in pixels
    int outline ( /*uint32 character_size*/void ) /*const*/;

    /// \brief Set a new font point size
    ///
    /// \param size Point size in pixels
    ///
    /// \remarks This is a potentially expensive call; if the glyphs for the
    /// requested point size has not been stored (generated previously), the
    /// generation of the font's ASCII glyph symbols must be done.
    ///
    /// \note For performance critical sections where scaling of the font
    /// occurs (think: real-time), you may want to take advantage of the glyph
    /// caching feature by making a call to this method for every used point
    /// size of the scaling range.
    bool set_point_size( int point_size );

    /// \brief Set the requested font hinting style.
    ///
    /// \remarks The hinting type, one of: TTF_HINTING_NORMAL, TTF_HINTING_LIGHT,
    /// TTF_HINTING_MONO or TTF_HINTING_NONE.
    ///
    /// \note This method call forces a rebuild of the glyph cache if the input
    /// type does not match the last known hinting.
    bool set_hinting( int type );

    /// \brief Set font's outline size
    ///
    /// \param outline The outline size in pixels
    ///
    /// \remarks This is an expensive method call; every glyph must be re-built!
    bool set_outline( int outline );

    /// \brief Load a new TrueType font from a file.
    ///
    /// \remarks Refer to the SDL_ttf documentation for file formats supported.
    /// TTF and FON file formats are known to be supported as of this writing.
    bool load( const std::string& filename );

    /// \brief Obtain information about the loaded font
    ///
    /// \remarks Implements IFont::metrics.
    const FontMetrics& metrics( void ) const;

  private:
    /// \brief Trigger a rebuild of the font metrics from the current font; this
    /// recalculates character sizes, coordinate origins, spacing, etc.
    ///
    /// \param character_size Font's point size, in pixels, to build glyphs for.
    bool build ( uint32 character_size );

    const GlyphPage& pages ( void ) const;

    sint sheet_width ( void ) const;
    sint sheet_height ( void ) const;

    /// \brief Find a suitable rectangle within the texture for a glyph
    ///
    /// \param page   Page of glyphs to search in
    /// \param width  Width of the rectangle
    /// \param height Height of the rectangle
    ///
    /// \return Found rectangle within the texture
    ///
    /// \remark This method makes it possible to store bitmap sheets efficiently
    ///
    /// \note Derived from SFML's sf::Font class -- thanks!
    const IntRect glyph_rect ( FontPage& page, int width, int height ) const;

    /// The type of font we are
    const enum IFont::FontType type_;

    /// Width -- in pixels -- of overall texture atlas sheet
    sint sheet_width_;

    /// Height -- in pixels -- of overall texture atlas sheet
    sint sheet_height_;

    /// Font file data, used by SDL_ttf extension
    std::shared_ptr<TTF_Font> font_;

    /// Table mapping a character size to its page -- a texture atlas combined
    /// with corresponding glyphs data.
    mutable GlyphPage pages_;

    /// General font metric data, such as the proper value for newline spacing
    FontMetrics metrics_;

    /// Store the file path so we can change font sizes on the fly
    ///
    /// \internal
    /// \note Replace with FT_Face face (using FreeType2 library)
    /// \endinternal
    std::string filename_;

    /// Font point size (in pixels)
    ///
    /// \internal
    /// \note   This instance variable is obsolete with FreeType2;
    ///         see FT_Face->size->metrics.x_ppem
    /// \endinternal
    sint point_size_;

    /// \brief The font's applied hinting style.
    ///
    /// \remarks The default is TTF_HINTING_NONE.
    int hinting_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::TrueTypeFont
/// \ingroup graphics
///
///   [TO BE WRITTEN]
///
/// \code
///
/// const int MAX_POINT_SIZE = 41;
///
/// nom::TrueTypeFont font;
///
/// // We must first load the font into memory from a file.
///
/// // Cache the glyphs of the font's point size range that we plan on using;
/// // offloads the cost of re-generating the glyph cache when the end-user
/// // requests an increase or decrease. (An increase in load-time for a
/// // decrease in latency upon rescaling, which is especially noticeable on
/// // older platforms and mobile devices).
/// for( auto idx = 0; idx != MAX_FONT_POINT_SIZE; ++idx )
/// {
///   font.set_point_size( idx + 1 );
/// }
///
/// \endcode
///
/// \see http://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_toc.html#SEC_Contents
/// \see http://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
/// \see http://chanae.walon.org/pub/ttf/ttf_glyphs.htm
/// \see http://cboard.cprogramming.com/game-programming/148396-ttf_font-gl-display-list-kerning-issue.html
/// \see SFML's sf::Font class
///
