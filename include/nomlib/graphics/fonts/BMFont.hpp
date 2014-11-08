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
#ifndef NOMLIB_GRAPHICS_FONTS_BMFONT_HPP
#define NOMLIB_GRAPHICS_FONTS_BMFONT_HPP

#include <fstream>
#include <sstream>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/graphics/fonts/IFont.hpp"
#include "nomlib/graphics/fonts/FontMetrics.hpp"
#include "nomlib/graphics/fonts/FontPage.hpp"

namespace nom {

/// \brief The kerning information is used to adjust the distance between
/// certain characters, e.g. some characters should be placed closer to
/// each other than others.
struct BMFontKerningPair
{
  /// \brief The first glyph identifier.
  ///
  /// \remarks This is mapped to the kernings tag's 'first' field.
  int first_char_id = 0;

  /// \brief The second glyph identifier.
  ///
  /// \remarks This is mapped to the kernings tag's 'second' field.
  int second_char_id = 0;

  /// \brief How much the x position should be adjusted when drawing the
  /// second character immediately following the first.
  int x_offset = 0;
};

std::ostream& operator <<(std::ostream& os, const BMFontKerningPair& k);

struct BMFontPage
{
  /// \brief The page identifier.
  ///
  /// \remarks This is the page tag's 'id' field.
  int id = 0;

  /// \brief The texture file name for the page.
  ///
  /// \remarks This is the page tag's 'filename' field.
  ///
  /// \note File paths with spaces are not supported by the parser.
  std::string filename;
};

class BMFont: public IFont
{
  public:
    typedef BMFont self_type;

    /// \brief Default constructor.
    BMFont();

    /// \brief Destructor.
    virtual ~BMFont();

    IFont* clone() const override;

    bool valid() const override;

    IFont::FontType type() const override;

    /// \param character_size Not implemented.
    const Image& image(uint32 character_size) const override;

    /// \param codepoint      The char identifier (an ASCII value) to lookup.
    /// \param character_size Not implemented.
    const Glyph& glyph(uint32 codepoint, uint32 character_size) const override;

    /// \param character_size Not implemented.
    int spacing(uint32 character_size) const override;

    /// \brief Obtain the kerning pair offsets between two glyphs.
    ///
    /// \param character_size Not implemented.
    ///
    /// \returns A kerning pair offset value on success, or nom::int_min on
    /// failure, such as if the font in use is invalid. If font kerning is
    /// disabled, a value of zero (0) is always returned.
    int kerning(uint32 first_char, uint32 second_char, uint32 character_size) const override;

    /// \remarks Not implemented.
    int hinting() const override;

    /// \remarks Not implemented.
    uint32 font_style() const override;

    /// \param character_size Not implemented.
    const Size2i& page_size(uint32 page) const;

    /// \param character_size Not implemented.
    int newline(uint32 character_size) const override;

    const FontMetrics& metrics() const override;

    int point_size() const;

    bool use_kerning() const;

    /// \remarks Not implemented.
    bool set_point_size(int) override;

    /// \remarks Not implemented.
    bool set_hinting(int) override;

    /// \remarks Not implemented.
    bool set_outline(int) override;

    /// \remarks Not implemented.
    void set_font_style(uint32) override;

    /// \brief Set the use of kerning for the font.
    ///
    /// \remarks Not implemented.
    void set_font_kerning(bool state);

    bool load(const std::string& filename) override;

    /// \brief Read an input source that conforms to the [BMFont](http://www.angelcode.com/products/bmfont/doc/file_format.html#tags)
    /// file spec.
    ///
    /// \note The file parser derives from the implementation provided by
    /// Promit of [GameDev.net Forums](http://www.gamedev.net/topic/330742-quick-tutorial-variable-width-bitmap-fonts/). Thanks!
    bool parse_ascii_file(std::istream& fp);

  private:
    /// \brief Create the texture atlas from the parsed font metrics.
    ///
    /// \param character_size Not implemented.
    bool build(uint32 character_size);

    /// \fixme Parsing string sequences with spaces
    void parse_string(const std::string& key, std::string& value);

    /// \brief The total dimensions -- width and height in integer pixels -- of
    /// the texture atlas used to hold the glyphs.
    ///
    /// \remarks This is mapped to the common tag's 'scaleW' and 'scaleH'
    /// fields.
    Size2i page_size_;

    /// \brief The glyph characteristics for the font in use.
    ///
    /// \remarks This structure maps the FontMetrics::lineHeight field to the
    /// common tag's 'lineHeight' field and the FontMetrics::base field to
    /// the common tag's 'base' field.
    FontMetrics metrics_;

    /// \remarks This structure maps the GlyphAtlas::GlyphPage::bounds fields
    /// to the char tag's 'x', 'y', 'width' and 'height' fields, the
    /// GlyphAtlas::GlyphPage::advance field to the char tag's 'advance' field
    /// and GlyphAtlas::GlyphPage::offset fields to the char tag's 'xoffset'
    /// and 'xoffset' fields.
    mutable GlyphPage pages_;

    /// \brief The point size of the font.
    ///
    /// \remarks This is mapped to the info tag's 'size' field.
    int point_size_;

    BMFontPage page_;

    /// \brief Kerning pairs.
    std::vector<BMFontKerningPair> kernings_;

    /// \brief Usage state of kerning pair offsets.
    bool use_kerning_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::BMFont
/// \ingroup graphics
///
/// \brief Bitmap font interface implementing most of the [BMFont](http://www.angelcode.com/products/bmfont/)
/// text/ascii file spec.
///
/// The following fields are not implemented from the info tag:
/// 'bold', 'italic', 'charset', 'unicode', 'stretchH', 'smooth', 'aa',
/// 'padding', 'spacing', 'outline'.
///
/// The following fields are not implemented from the common tag:
/// 'packed', 'alphaChnl', 'redChnl', 'greenChnl', 'blueChnl'.
///
/// The following fields are not implemented from the char tag:
/// 'page', 'chnl'.
///
/// Only one glyph page is supported. String sequences with spaces are not
/// supported.
///
/// Software this interface has been tested with includes:
///
/// [BMGlyph v2.0.4](http://www.bmglyph.com/)
/// [BMFont v1.14b](http://www.angelcode.com/products/bmfont/)
///
