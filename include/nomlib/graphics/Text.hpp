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
      Italic = 4,         /// Verify working functionality
      Underlined = 8,     /// Verify working functionality
      Strikethrough = 16  /// Verify working functionality
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

    /// \brief Construct a Text, initializing it with a text string, a nom::Font
    /// object reference, character size and text alignment.
    Text (
            const std::string& text,
            const font_type& font,
            uint character_size = nom::DEFAULT_FONT_SIZE,
            uint32 align = Anchor::TopLeft,
            const Color4i& text_color = Color4i::White
          );

    /// \brief Construct a Text, initializing it with a text string, a nom::Font
    /// object pointer, character size and text alignment.
    Text (
            const std::string& text,
            const font_type::raw_ptr font,
            uint character_size = nom::DEFAULT_FONT_SIZE,
            uint32 align = Anchor::TopLeft,
            const Color4i& text_color = Color4i::White
          );

    /// \see nom::DataViewTextItem
    Text( const std::string& text );

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
    const Color4i& color( void ) const;

    /// Get text style
    uint32 style( void ) const;

    //const Point2i& local_bounds ( void ) const;

    /// \brief Obtain the rendered coordinate bounds of the text object.
    ///
    /// \remarks This is equivalent to a call made to the methods ::position(),
    /// ::width and ::height.
    IntRect global_bounds( void ) const;

    /// \brief Get the text alignment.
    uint32 alignment( void ) const;

    /// Get text character size (in pixels?)
    uint text_size ( void ) const;

    /// \brief Obtain the set rendering flags.
    ///
    /// \remarks See also: nom::Text::ExtraRenderingFeatures enumeration.
    uint32 features( void ) const;

    /// \brief Set the overall dimension bounds (width & height) of the text
    /// object.
    ///
    /// \remarks This method ensures that the text's alignment is recalculated.
    ///
    /// \note Re-implements Transformable::set_size.
    void set_size( const Size2i& size );

    /// \brief Set the font from a nom::Font object.
    void set_font( const Text::font_type& font );

    /// \brief Set a font from a nom::Font object pointer.
    ///
    /// \note Used for ResourceCache::load_resource.
    void set_font( Text::font_type* font );

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

    /// \brief Set the text's alignment.
    ///
    /// \see The nom::Anchor enumeration.
    ///
    /// \remarks This method depends on the size dimensions and the text's font
    /// being in a valid state.
    ///
    /// \note Every call to this method modifies the destination positions used
    /// in the rendered text.
    void set_alignment( uint32 align );

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
    /// \remarks This internal method takes care of updating the properties of
    /// the font in the correct order, so that the end-user (developer) does not
    /// need to worry about using the API in a particular order.
    ///
    /// \note Implements nom::IDrawable::update.
    void update( void );

    mutable font_type font_; // FIXME?
    mutable Texture texture_; // FIXME

    /// Holds contents of text as a string buffer
    std::string text_;
    uint text_size_;
    Color4i color_;

    /// Current text effect set
    uint32 style_;
    uint32 alignment_;

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
