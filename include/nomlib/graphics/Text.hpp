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
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/fonts/Font.hpp"

namespace nom {

/// \todo Implement back-buffer for texture rendering
class Text: public Transformable
{
  public:
    typedef Text self_type;
    typedef Transformable derived_class;

    /// \brief Font face style; multiple styles can be combined from bitwise
    /// masks.
    ///
    /// \remarks Styles are only available with TrueType fonts. No action will
    /// result from using these styles with a bitmap font.
    ///
    /// \fixme Implement styles for BitmapFont objects.
    enum Style: uint32
    {
      Normal = 1,         /// Default
      Bold = 2,
      Italic = 4,
      Underline = 8,
      Strikethrough = 16
    };

    /// Default constructor
    Text( void );

    /// Destructor
    ~Text( void );

    /// \brief Construct a Text, initializing it with a text string, a nom::Font
    /// object reference and character size.
    Text (
            const std::string& text,
            const Font& font,
            uint character_size = nom::DEFAULT_FONT_SIZE,
            const Color4i& text_color = Color4i::White
          );

    /// \brief Construct a Text, initializing it with a text string, a nom::Font
    /// object pointer and character size.
    Text (
            const std::string& text,
            Font* font,
            uint character_size = nom::DEFAULT_FONT_SIZE,
            const Color4i& text_color = Color4i::White
          );

    /// \brief Implements the required IDrawable::clone method.
    IDrawable::raw_ptr clone( void ) const;

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    const Font& font() const;

    const Texture& texture ( void ) const;

    /// Obtain validity of the Text object
    bool valid ( void ) const;

    // enum IFont::FontType font_type( void ) const;

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

    /// \brief Obtain the text height in pixels of the set text
    ///
    /// \remarks  This calculation should mimic the rendering calculations
    ///           precisely.
    ///
    /// \returns  Non-negative integer value of the object's text string height,
    ///           in pixels, on success. Zero (0) integer value on failure;
    ///           a possible combination of: no font, bad font, no text string
    ///           etc.
    sint text_height ( const std::string& text_string ) const;

    /// Get text string
    const std::string& text ( void ) const;

    /// Get text color
    const Color4i& color( void ) const;

    /// Get text style
    uint32 style( void ) const;

    /// Get text character size (in pixels?)
    uint text_size ( void ) const;

    /// \brief Set the font to use in rendering text.
    ///
    /// \remarks You must ensure that you are passing a valid nom::Font object
    /// that has been loaded into memory with its appropriate ::load call.
    void set_font(const Font& font);

    /// \brief Set the font to use in rendering text.
    ///
    /// \remarks This method exists for the convenience of direct interfacing
    /// with the nom::IFont interface. You must ensure that you are passing a
    /// valid nom::IFont-derived object that has been loaded into memory with
    /// its appropriate ::load call.
    ///
    /// \note This method is used by nom::FontCache.
    void set_font(Font* font);

    /// \brief Set the text string to be rendered.
    void set_text( const std::string& text );

    /// \brief Set the text's font point size.
    ///
    /// \remarks This method rebuilds the texture atlas when the point size
    /// differs from the current set point size, and may be an expensive
    /// operation.
    ///
    /// \note The text's font must be set for this method call to succeed.
    void set_text_size ( uint character_size );

    /// \brief Set text's font color.
    void set_color( const Color4i& text_color );

    /// \brief Set the text's rendering style for the font.
    ///
    /// \see The nom::Text::Style enumeration.
    void set_style( uint32 style );

    /// Render text to a target
    ///
    /// \todo Test horizontal tabbing '\t'
    void draw ( RenderTarget& target ) const;

  private:
    /// \brief Apply requested transformations, styles, etc
    ///
    /// \remarks This internal method takes care of updating the properties of
    /// the font in the correct order, so that the end-user (developer) does not
    /// need to worry about using the API in a particular order.
    ///
    /// \note Implements nom::IDrawable::update.
    void update();

    /// \brief Get the current text width.
    int width() const;

    /// \brief Get the current text height.
    int height() const;

    Font font_;

    /// \fixme
    mutable Texture texture_;

    /// Holds contents of text as a string buffer
    std::string text_;
    uint text_size_;
    Color4i color_;

    /// Current text effect set
    uint32 style_;

    /// \see nom::Text::update
    bool dirty_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::Text
/// \ingroup graphics
///
///     [TODO: DESCRIPTION]
///
/// \code
///
/// #include <iostream>
/// #include <nomlib/graphics.hpp>
///
/// // Declarations
/// std::string file_path = "absolute_path_to_truetype_font.ttf";
/// bool running = true;
/// nom::RenderWindow window;
/// nom::Font font;
/// nom::Text label;
///
/// // Initialization
/// if( font.load( file_path ) == false )
/// {
///   // Handle Err
/// }
///
/// label.set_font( font );
/// label.set_position( nom::Point2i( 25, 25 );
/// label.set_text( "Hello, world!" );
/// label.set_color( nom::Color4i::Orange );
/// label.set_text_size( 24 );
///
/// while( running == true )
/// {
///   label.draw( window );
/// }
///
/// \endcode
///
