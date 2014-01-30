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
#ifndef NOMLIB_GRAPHICS_TEXT_HPP
#define NOMLIB_GRAPHICS_TEXT_HPP

#include <iostream>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/graphics/fonts/BitmapFont.hpp"
#include "nomlib/graphics/fonts/IFont.hpp"
#include "nomlib/graphics/fonts/TrueTypeFont.hpp"

namespace nom {

/// \todo Implement back-buffer for texture rendering
class Text: public Transformable
{
  public:
    typedef Text* RawPtr;
    typedef std::shared_ptr<Text> SharedPtr;

    /// Font face style; multiple styles can be combined from bitwise masks
    ///
    /// \remarks Bold is only available with TrueType fonts. No action will
    // result from using this style with a bitmap font.
    enum Style: uint32
    {
      Regular = 1,        /// Default
      Bold = 2,           /// Not implemented
      Italic = 4,
      Underlined = 8,     /// Not implemented
      Strikethrough = 16  /// Not implemented
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
    enum Alignment: uint32
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
    Text ( void );

    /// Destructor
    virtual ~Text ( void );

    /// Copy constructor
    Text ( const Text& copy );

    /// Copy assignment overload
    Text& operator = ( const Text& other );

    /// Construct a Text, initializing it with a text string, an IFont
    /// reference, character size and text alignment.
    Text (  const std::string& text,
            const IFont& font,
            uint character_size = 14,
            enum Text::Alignment align = Text::Alignment::TopLeft
          );

    /// Construct a Text, initializing it with a text string, an IFont
    /// pointer, character size and text alignment.
    Text (  const std::string& text,
            const IFont::SharedPtr& font,
            uint character_size = 14,
            enum Text::Alignment align = Text::Alignment::TopLeft
          );

    Text::RawPtr get ( void );

    IFont::SharedPtr font ( void ) const;

    const Texture& texture ( void ) const;

    /// Obtain validity of the Text object
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
    sint text_width ( const std::string& text_string ) const;
    sint width ( void ) const;

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
    /// computation -- see nom::FontMetrics.
    sint text_height ( const std::string& text_string ) const;
    sint height ( void ) const;

    /// Get text string
    const std::string& text ( void ) const;

    /// Get text color
    const Color4i& color ( void ) const;

    /// Get text style
    enum Text::Style style ( void ) const;

    //const Point2i& local_bounds ( void ) const;
    //const Point2i& global_bounds ( void ) const;

    /// Get text alignment
    enum Text::Alignment alignment ( void ) const;

    /// Get text character size (in pixels?)
    uint text_size ( void ) const;

    /// Set a new font (deriving from IFont).
    void set_font ( const IFont& font );

    /// Set new text
    void set_text ( const std::string& text );

    /// Set new text character size
    void set_text_size ( uint character_size );

    /// Set new text color
    void set_color ( const Color4i& text_color );

    /// Set new text style
    void set_style ( enum Text::Style style );

    /// Set new text alignment
    ///
    /// \remarks This method modifies the destination positions used in
    /// rendering text.
    void set_alignment ( enum Text::Alignment align );

    /// Render text to a target
    ///
    /// \todo Test horizontal tabbing '\t'
    void draw ( RenderTarget target ) const;

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
    Color4i color_;
    /// Current text effect set
    enum Style style_;
    enum Alignment alignment_;
/*
    Point2i local_bounds;
    Point2i global_bounds;
*/
};

} // namespace nom

#endif // include guard defined
