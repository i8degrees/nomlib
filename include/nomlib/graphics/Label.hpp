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
#ifndef NOMLIB_LABEL_HPP
#define NOMLIB_LABEL_HPP

#include <iostream>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/graphics/fonts/BitmapFont.hpp"
#include "nomlib/graphics/fonts/IFont.hpp"
#include "nomlib/graphics/fonts/TrueTypeFont.hpp"

/// Dump each character's glyph data used in width & height calculations
//#define NOM_DEBUG_LABEL

namespace nom {

class Label: public Transformable
{
  public:
    typedef Label* RawPtr;
    typedef std::shared_ptr<Label> SharedPtr;

    /// Font face style; multiple styles can be combined from bitwise masks
    ///
    /// \remarks Bold is only available with TrueType fonts. No action will
    // result from using this style with a bitmap font.
    enum FontStyle: uint32
    {
      Regular = 1, // Default
      Bold = 2,
      Italic = 4,
      Underlined = 8
    };

    /// Text alignment choices (pick one).
    ///
    /// \internal
    ///
    /// \note Bitwise masks were intentionally not chosen here, in order to
    /// allow my preferred naming scheme. I think the scheme chosen feels
    /// most natural, and thus easier to remember and type out.
    ///
    /// \endinternal
    enum TextAlignment: uint32
    {
      TopLeft = 0,  // Default
      TopCenter,    // 1
      TopRight,     // 2
      MiddleLeft,   // 3
      MiddleCenter, // 4
      MiddleRight,  // 5
      BottomLeft,   // 6
      BottomCenter, // 7
      BottomRight   // 8
    };

    /// Default constructor
    Label ( void );

    /// Destructor
    ~Label ( void );

    /// Copy constructor
    Label ( const Label& copy );

    /// Copy assignment overload
    Label& operator = ( const Label& other );

    /// Construct a label, initializing it with a text string, an IFont
    /// reference, character size and text alignment.
    Label ( const std::string& text,
            const IFont& font,
            uint character_size,
            enum TextAlignment align = Label::TopLeft
          );

    /// Construct a label, initializing it with a text string, an IFont
    /// pointer, character size and text alignment.
    Label ( const std::string& text,
            const IFont::SharedPtr& font,
            uint character_size,
            enum TextAlignment align = Label::TopLeft
          );

    Label::RawPtr get ( void );

    IFont::SharedPtr font ( void ) const;

    const Texture& texture ( void ) const;

    /// Obtain validity of the label object
    bool valid ( void ) const;

    enum IFont::FontType type ( void ) const;

    /// \brief Obtain the text width (in pixels) of the set text
    ///
    /// \remarks  This calculation should mimic the rendering calculations
    ///           precisely.
    ///
    /// \returns  Non-negative integer value of the object's text string width,
    ///           in pixels, on success. Zero (0) integer value on failure;
    ///           a possible combination of: no font, bad font, no text string
    ///           etc.
    ///
    /// \todo Support multi-line texts (newline character handling)
    uint width ( void ) const;

    /// \brief Obtain the text height in pixels of the set text
    ///
    /// \remarks  This calculation should mimic the rendering calculations
    ///           precisely.
    ///
    /// \returns  Non-negative integer value of the object's text string height,
    ///           in pixels, on success. Zero (0) integer value on failure;
    ///           a possible combination of: no font, bad font, no text string
    ///           etc.
    ///
    /// \todo     We *may* have a better height value to use in our height
    // computation -- see nom::FontMetrics.
    uint height ( void ) const;

    /// Get label's text string
    const std::string& text ( void ) const;

    /// Get label's text color
    const Color4u& color ( void ) const;

    /// Get label's text style
    enum Label::FontStyle style ( void ) const;

    //const Point2i& local_bounds ( void ) const;
    //const Point2i& global_bounds ( void ) const;

    /// Get label's text alignment
    enum Label::TextAlignment alignment ( void ) const;

    /// Get label's text character size (in pixels?)
    uint text_size ( void ) const;

    /// Set a new font (deriving from IFont).
    void set_font ( const IFont& font );

    /// Set new label text
    void set_text ( const std::string& text );

    /// Set new label text character size
    void set_text_size ( uint character_size );

    /// Set new label text color
    void set_color ( const Color4u& text_color );

    /// Set new label text style
    void set_style ( enum Label::FontStyle style );

    /// Set new label text alignment
    ///
    /// \remarks This method modifies the destination positions used in
    /// rendering text.
    void set_alignment ( enum Label::TextAlignment align );

    /// Render label to a target
    ///
    /// \todo Test horizontal tabbing '\t'
    void draw ( RenderTarget target ) const;

    /// Rescale the font with a chosen resizing algorithm
    ///
    /// \todo SDL2 port
    bool resize ( enum Texture::ResizeAlgorithm scaling_algorithm );

  private:
    /// \brief Apply requested transformations, styles, etc
    ///
    /// \remarks Implements nom::IDrawable::update
    ///
    /// \fixme Logic for this feature is incomplete!
    void update ( void );

    IFont::SharedPtr font_;
    mutable Texture texture_; /// \FIXME
    /// Holds contents of text as a string buffer
    std::string text_;
    uint text_size_;
    Color4u color_;
    /// Current text effect set
    enum FontStyle style_;
    enum TextAlignment alignment_;
/*
    Point2i local_bounds;
    Point2i global_bounds;
*/
};

} // namespace nom

#endif // include guard defined
