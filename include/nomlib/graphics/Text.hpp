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
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/graphics/Texture.hpp"

#include "nomlib/graphics/fonts/IFont.hpp"
#include "nomlib/graphics/fonts/Font.hpp"

namespace nom {

/// \todo Implement back-buffer for texture rendering
class Text: public Transformable
{
  public:
    typedef Text self_type;
    typedef Transformable derived_class;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;

    typedef Font font_type;

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
    ///
    /// \deprecated Text::Alignment is probably going to be replaced by
    /// nom::Anchor in the near future.
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

    /// \brief Additional rendering features; these are all optional, and are
    /// turned off by default.
    ///
    /// \remarks Bit-mask friendly to pass.
    enum ExtraRenderingFeatures: uint32
    {
      // Prevent rendering of text that goes beyond a user-defined size; the
      // formula for detecting "out of bounds" is ::position().x + ::size().w.
      CropText = 1
    };

    /// Default constructor
    Text( void );

    /// Destructor
    ~Text( void );

    /// Copy constructor
    Text ( const self_type& copy );

    /// Copy assignment overload
    self_type& operator = ( const self_type& other );

    /// Construct a Text, initializing it with a text string, a Font
    /// object, character size and text alignment.
    Text (
            const std::string& text,
            const font_type& font,
            uint character_size = 14,
            enum Text::Alignment align = Text::Alignment::TopLeft,
            const Color4i& text_color = Color4i::White
          );

    // Construct a Text, initializing it with a text string, an IFont derived
    // object, character size and text alignment.
    // Text (
    //         const std::string& text,
    //         const IFont& font,
    //         uint character_size = 14,
    //         enum Text::Alignment align = Text::Alignment::TopLeft
    //       );

    /// Construct a minimal Text object, initializing it with a text string.
    ///
    /// \remarks A text font must be set before any rendering, width/height
    /// calculations, etc. is used; see also -- ::set_font.
    Text ( const std::string& text );

    // Construct a Text, initializing it with a text string, an IFont
    // raw pointer, character size, text color and text alignment.
    // Text (
    //         const std::string& text,
    //         const IFont::raw_ptr font,
    //         uint character_size = 14,
    //         enum Text::Alignment align = Text::Alignment::TopLeft,
    //         const Color4i& text_color = Color4i::White
    //       );

    /// \brief Implements the required IDrawable::clone method.
    IDrawable::raw_ptr clone( void ) const;

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    font_type& font( void ) const;

    Text::raw_ptr get( void );

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

    /// \brief Obtain the rendered coordinate bounds of the text object.
    ///
    /// \remarks This is equivalent to a call made to the methods ::position(),
    /// ::width and ::height.
    IntRect global_bounds( void ) const;

    /// Get text alignment
    enum Text::Alignment alignment ( void ) const;

    /// Get text character size (in pixels?)
    uint text_size ( void ) const;

    /// \brief Obtain the set rendering flags.
    ///
    /// \remarks See also: nom::Text::ExtraRenderingFeatures enumeration.
    uint32 features( void ) const;

    /// \brief Set the font from a nom::Font object.
    void set_font( const Text::font_type& font );

    /// \brief Set a font from a nom::Font object pointer.
    ///
    /// \note Used by ResourceCache::load_resource.
    void set_font( Text::font_type* font );

    // \brief Set a new font, deriving from an nom::IFont derived object.
    // void set_font( const IFont& font );

    // \brief Set a new font from an existing pointer deriving from IFont.
    //
    // void set_font( const IFont::raw_ptr font );
    // void set_font( const IFont::shared_ptr& font );

    /// Set new text
    void set_text ( const std::string& text );

    /// Set new text character size.
    ///
    /// \remarks This method forces a rebuild of the texture atlas with the
    /// given size, and may be an expensive operation.
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
    void draw ( RenderTarget& target ) const;

    /// Rescale the font with a chosen resizing algorithm
    ///
    /// \todo SDL2 port
    bool resize ( enum Texture::ResizeAlgorithm scaling_algorithm );

    /// \brief Set additional rendering flags.
    ///
    /// \remarks See also: nom::Text::ExtraRenderingFeatures enumeration.
    ///
    /// \note Bit-mask friendly.
    void set_features( uint32 flags );

  private:
    /// \brief Apply requested transformations, styles, etc
    ///
    /// \remarks Implements nom::IDrawable::update
    ///
    /// \todo Logic for this feature is incomplete!
    void update ( void );

    mutable font_type font_; // FIXME?
    mutable Texture texture_; // FIXME

    /// Holds contents of text as a string buffer
    std::string text_;
    uint text_size_;
    Color4i color_;

    /// Current text effect set
    enum Style style_;
    enum Alignment alignment_;

    /// \brief Set additional rendering flags.
    ///
    /// \remarks See also: nom::Text::ExtraRenderingFeatures enumeration.
    uint32 features_;

/*
    Point2i local_bounds;
    Point2i global_bounds;
*/
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
