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
#ifndef NOMLIB_SDL2_MESSAGEBOX_HEADERS
#define NOMLIB_SDL2_MESSAGEBOX_HEADERS

#include <iostream>
#include <string>
#include <memory>
#include <array>

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/Label.hpp"
#include "nomlib/graphics/Gradient.hpp"
#include "nomlib/gui/GrayFrame.hpp"

namespace nom {

/// \brief Simple UI interface for drawing a styled message box
class MessageBox: public IDrawable
{
  public:
    enum Style
    {
      None = 0,
      Gray = 1
    };

    MessageBox ( void );

    virtual ~MessageBox ( void );

    MessageBox  (
                  int32 x, int32 y, int32 width, int32 height,
                  enum MessageBox::Style style, const Gradient& background
                );

    MessageBox  (
                  int32 x, int32 y, int32 width, int32 height,
                  GrayFrame::SharedPtr style = nullptr,
                  Gradient::SharedPtr background = nullptr
                );

    MessageBox& operator = ( const MessageBox& other );

    bool enabled ( void ) const;

    const std::string title ( void );
    const std::string text ( void );

    void disable ( void );
    void enable ( void );

    const Point2i size ( void ) const;
    const Point2i position ( void ) const;

    void set_title ( const Label& title );
    void set_text ( const Label& text );

    void draw ( RenderTarget target ) const;

  protected:
    void update ( void );

  private:
    IDrawable::SharedDrawables drawable;

    /// Array holding our up to two labels (title and text, respectively)
    std::array<Label, 2> labels;

    Coords coords;
    bool enabled_;

    bool updated;
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
/// nom::Label label;
/// nom::Gradient linear;
/// nom::ui::MessageBox info_box;

/// // Initialize the background to use in our info_box object as a gradient filled
/// // background
/// linear.set_start_color ( nom::Color4u ( 67, 67, 67, 255 ) );
/// linear.set_end_color ( nom::Color4u ( 99, 99, 99, 255 ) );
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
/// info_box.set_label_text_alignment ( nom::Label::TextAlignment::MiddleCenter );

/// info_box.update();

/// // Render our info_box from a previously initialized nom::Window object
/// info_box.draw ( window );
///
/// \endcode
