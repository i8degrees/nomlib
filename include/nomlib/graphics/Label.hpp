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
#include "nomlib/graphics/IFont.hpp"
#include "nomlib/graphics/BitmapFont.hpp"
#include "nomlib/graphics/TrueTypeFont.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Point2.hpp"

//#define NOM_DEBUG_LABEL

namespace nom {

class Label: public Transformable
{
  public:
    typedef Label* RawPtr;
    typedef std::shared_ptr<Label> SharedPtr;

    /// Text effect styling
    enum FontStyle
    {
      Regular = 0,
      Bold = 1,
      Italic = 2,
      Underlined = 3,
      Faded = 4
    };

    enum TextAlignment
    {
      TopLeft = 0,
      TopCenter,
      TopRight,
      MiddleLeft,
      MiddleCenter,
      MiddleRight,
      BottomLeft,
      BottomCenter,
      BottomRight
    };

    /// Default constructor
    Label ( void );

    /// Destructor
    ~Label ( void );

    /// Construct an object deriving from IFont
    Label ( const IFont& font );

    bool initialize ( const IFont& font );

    Label::RawPtr get ( void );

    IFont::RawPtr font ( void ) const;

    /// Obtain validity of the label object
    bool valid ( void ) const;

    enum IFont::FileType type ( void ) const;

    /// Obtain the text width in pixels of the set text string.
    ///
    /// This calculation mimics the rendering calculations done and should be
    /// exact within one pixel accuracy.
    ///
    /// \todo Support multi-line texts
    int width ( void ) const;

    /// Obtain the text height in pixels of the set text string.
    ///
    /// This calculation mimics the rendering calculations done and should be
    /// exact within one pixel accuracy.
    int height ( void ) const;

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

    /// Set a new font, deriving from IFont.
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
    void update ( void );
    IFont::SharedPtr font_;
    mutable Texture render_font_; /// \FIXME
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
