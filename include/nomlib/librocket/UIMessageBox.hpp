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
#ifndef NOMLIB_LIBROCKET_UIMESSAGEBOX_HPP
#define NOMLIB_LIBROCKET_UIMESSAGEBOX_HPP

#include <iostream>
#include <string>
#include <memory>

#include <Rocket/Core/Context.h>
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/Element.h>
#include <Rocket/Core/ElementUtilities.h>

#include "nomlib/config.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/librocket/RocketUtilities.hpp"

namespace nom {

// Forward declarations
class UIEventListener;
class RenderWindow;

class UIWidget: public Transformable
{
  public:
    UIWidget( const Point2i& pos, const Size2i& dims );

    virtual ~UIWidget();

    /// \brief Re-implements Transformable::set_position.
    virtual void set_position( const Point2i& pos );

    /// \brief Re-implements Transformable::set_size.
    ///
    /// \remarks The size does not take precedence over the max-width and
    /// max-height style sheet properties when they have been set by the
    /// end-user.
    virtual void set_size( const Size2i& dims );

    rocket::Context* desktop() const;
    rocket::ElementDocument* document() const;

    bool valid() const;
    bool visible() const;

    const std::string& title_id() const;
    std::string title() const;

    bool set_desktop( rocket::Context* ctx );

    /// \todo Rename to load_document_file?
    bool set_document_file( const std::string& filename );

    void set_document_size();

    void show();
    void hide();
    void close();

    void set_title( const std::string& text );

    void set_title_id( const std::string& id );

    void set_font( rocket::Element* target, const std::string& font );
    void set_font_size( rocket::Element* target, int point_size );

    void set_alignment( rocket::Element* element, uint32 alignment );

    void register_event_listener( rocket::Element* element,
                                  const std::string& ev,
                                  UIEventListener* observer,
                                  bool interruptible = false );

  private:
    /// \remarks This pointer is **not** owned by us, and must not be freed.
    rocket::Context* desktop_;

    /// \remarks This pointer is **not** owned by us, and must not be freed.
    rocket::ElementDocument* document_;

    std::string title_id_;
};

/// \brief Simple UI interface for drawing a styled message box
///
/// \todo Check for valid element IDs
class UIMessageBox: public UIWidget
{
  public:
    typedef UIMessageBox self_type;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;

    /// \brief Default constructor; initialize object to an invalid state --
    /// its position and size are set to the value of Point2i::null and
    /// Size2i::null, respectively.
    ///
    /// \see UIWidget::set_desktop, UIWidget::set_document_file,
    /// UIMessageBox::initialize.
    UIMessageBox();

    UIMessageBox( const Point2i& pos, const Size2i& dims );

    virtual ~UIMessageBox();

    bool initialize();

    bool enabled() const;

    std::string title_text() const;

    /// \brief Obtain the rectangle bounds of the title string.
    ///
    /// \returns nom::IntRect object with the absolute X, Y coordinates -- as
    /// rendered -- and string's dimensions with respect to the current font in
    /// use.
    ///
    /// \note The string's height dimension is the newline height, **not** the
    /// maximal glyph height.
    ///
    /// \todo Rename to title_text_bounds
    IntRect title_bounds() const;

    /// \fixme
    uint32 title_alignment() const;

    std::string message_text() const;

    /// \brief Obtain the rectangle bounds of the message string.
    ///
    /// \returns nom::IntRect object with the absolute X, Y coordinates -- as
    /// rendered -- and string's dimensions with respect to the current font in
    /// use.
    ///
    /// \note The string's height dimension is the newline height, **not** the
    /// maximal glyph height.
    ///
    /// \todo Rename to message_text_bounds
    IntRect message_bounds() const;

    /// \fixme
    uint32 message_alignment() const;

    const std::string& message_id() const;

    void disable();
    void enable();

    /// \brief Set the title (caption) text of the message box.
    ///
    /// \remarks The title text's alignment is set to Anchor::TopLeft.
    void set_title( const std::string& text, const std::string& font, uint point_size );

    /// \brief Set the title (caption) text of the message box.
    void set_title_text( const std::string& text );

    /// \brief Set the title (caption) text's font.
    void set_title_font( const std::string& font );

    /// \brief Set the title (caption) text's font size.
    void set_title_font_size( int point_size );

    /// \brief Set the title (caption) text's alignment.
    void set_title_alignment( uint32 align );

    /// \brief Set the message text label.
    ///
    /// \remarks The message label text's alignment is set to
    /// Anchor::MiddleCenter.
    void set_message( const std::string& text, const std::string& font, uint point_size );

    /// \brief Set the message text label.
    void set_message_text( const std::string& text );

    /// \brief Set the message text's font.
    void set_message_font( const std::string& font );

    /// \brief Set the message text's font size.
    void set_message_font_size( int point_size );

    /// \brief Set the message text's alignment.
    void set_message_alignment( uint32 align );

    void set_message_id( const std::string& id );

    /// \brief Implements UIWidget::draw method.
    void draw( RenderTarget& target ) const;

  protected:
    /// \brief Implements UIWidget::update method.
    virtual void update();

  private:
    std::string message_id_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::UIMessageBox
/// \ingroup librocket
///
