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
#ifndef NOMLIB_GUI_MESSAGEBOX_HPP
#define NOMLIB_GUI_MESSAGEBOX_HPP

#include <iostream>
#include <string>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/graphics/Text.hpp"
#include "nomlib/gui/UIWidget.hpp"

namespace nom {

// Forward declarations
class UIEvent;

/// \brief Simple UI interface for drawing a styled message box
class MessageBox: public UIWidget
{
  public:
    typedef MessageBox self_type;
    typedef UIWidget derived_class;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    MessageBox  (
                  UIWidget* parent,
                  int64 id,
                  const Point2i& pos,
                  const Size2i& size
                );

    virtual ~MessageBox( void );

    /// \brief Copy constructor.
    ///
    /// \remarks This class is non-copyable.
    MessageBox( const self_type& rhs ) = delete;

    bool enabled( void ) const;

    const std::string& title_text( void ) const;
    const std::string& message_text( void ) const;

    /// \brief Obtain the rectangle bounds of the set title text
    ///
    /// \returns IntRect with its x, y, width & height fields filled in
    ///
    /// \remarks The bounds are the final rendered coordinates (in pixels),
    /// which may include alignments done to the Text object after being set
    /// in this class.
    const IntRect title_bounds( void ) const;

    /// \brief Obtain the rectangle bounds of the set text
    ///
    /// \returns IntRect with its x, y, width & height fields filled in
    ///
    /// \remarks The bounds are the final rendered coordinates (in pixels),
    /// which may include alignments done to the Text object after being set
    /// in this class.
    const IntRect message_bounds( void ) const;

    Text::Alignment title_alignment( void ) const;
    Text::Alignment message_alignment( void ) const;

    void disable( void );
    void enable( void );

    /// \brief Set the title (caption) text of the message box.
    ///
    /// \remarks The title text's alignment is set to Text::Alignment::TopLeft.
    void set_title( const std::string& text, const Font& font, uint point_size );

    /// \brief Set the title (caption) text of the message box.
    void set_title_text( const std::string& text );

    /// \brief Set the title (caption) text's font.
    void set_title_font( const Font& font );

    /// \brief Set the title (caption) text's font size.
    void set_title_font_size( uint point_size );

    /// \brief Set the title (caption) text's alignment.
    void set_title_alignment( Text::Alignment align );

    /// \brief Set the message text label.
    ///
    /// \remarks The message label text's alignment is set to
    /// Text::Alignment::MiddleCenter.
    void set_message( const std::string& text, const Font& font, uint point_size );

    /// \brief Set the message text label.
    void set_message_text( const std::string& text );

    /// \brief Set the message text's font.
    void set_message_font( const Font& font );

    /// \brief Set the message text's font size.
    void set_message_font_size( uint point_size );

    /// \brief Set the message text's alignment.
    void set_message_alignment( Text::Alignment align );

    /// \brief Implements UIWidget::draw method.
    void draw( RenderTarget& target ) const;

  protected:
    /// \brief Implements UIWidget::update method.
    ///
    /// \todo Move to private scope.
    void update( void );

    // TODO:
    // void update_bounds( void );

    /// \brief Re-implements UIWidget::on_size_changed.
    virtual void on_size_changed( UIEvent* ev );

    virtual void on_mouse_down( UIEvent* ev );
    virtual void on_mouse_up( UIEvent* ev );

  private:
    /// \brief Title (caption) text label.
    Text title_;

    /// \brief Message contents text label.
    Text message_;

    bool enabled_;

    bool updated_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::MessageBox
/// \ingroup gui
///
///         [DESCRIPTION STUB]
///
/// Usage example:
/// \code
///
/// #include <nomlib/graphics.hpp>

/// // Position & size declarations
/// const nom::int WINDOW_WIDTH = 768;
/// const nom::int WINDOW_HEIGHT = 448;
/// const nom::int INFO_BOX_WIDTH = 352;
/// const nom::int INFO_BOX_HEIGHT = 48;
/// const nom::int INFO_BOX_ORIGIN_X = ( WINDOW_WIDTH - INFO_BOX_WIDTH ) / 2;
/// const nom::int INFO_BOX_ORIGIN_Y = ( WINDOW_HEIGHT - INFO_BOX_HEIGHT ) / 2;

/// // Object declarations
/// nom::Text label;
/// nom::Gradient linear;
/// nom::ui::MessageBox info_box;

/// // Initialize the background to use in our info_box object as a gradient filled
/// // background
/// linear.set_start_color ( nom::Color4i ( 67, 67, 67, 255 ) );
/// linear.set_end_color ( nom::Color4i ( 99, 99, 99, 255 ) );
/// linear.set_fill_direction ( nom::Gradient::FillDirection::Left );

/// // Initialize our info_box object
/// info_box = nom::ui::MessageBox  (
///                                   INFO_BOX_ORIGIN_X,
///                                   INFO_BOX_ORIGIN_Y,
///                                   INFO_BOX_WIDTH,
///                                   INFO_BOX_HEIGHT,
///                                   // Use the built-in "gray" frame style
///                                   nom::ui::FrameStyle::Gray,
///                                   // Use a custom background style object
///                                   linear
///                                 );

/// info_box.set_window_title_font ( &bitmap_font );
/// info_box.set_label_font ( &bitmap_font );
/// info_box.set_label ( "Hello, world!" );
/// info_box.set_window_title ( "INFO" );
/// info_box.set_label_text_alignment ( nom::Text::Alignment::MiddleCenter );

/// info_box.update();

/// // Render our info_box from a previously initialized nom::RenderWindow object
/// info_box.draw ( window );
///
/// \endcode
