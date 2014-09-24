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
#ifndef NOMLIB_LIBROCKET_UIWIDGET_HPP
#define NOMLIB_LIBROCKET_UIWIDGET_HPP

#include <string>

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
    typedef UIWidget self_type;
    typedef self_type* raw_ptr;

    enum FocusPolicy
    {
      NONE = rocket::ElementDocument::FocusFlags::NONE,
      FOCUS = rocket::ElementDocument::FocusFlags::FOCUS,
      MODAL = rocket::ElementDocument::FocusFlags::MODAL
    };

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
    bool valid( rocket::Element* target ) const;
    bool visible() const;

    const std::string& title_id() const;
    std::string title() const;

    /// \brief Get an element's text alignment.
    ///
    /// \returns One of the nom::Anchor enumeration values.
    uint32 alignment( rocket::Element* target ) const;

    bool set_desktop( rocket::Context* ctx );

    /// \todo Rename to load_document_file?
    bool set_document_file( const std::string& filename );

    void set_document_size();

    void show();
    void hide();
    void close();

    /// \brief Set the focus policy.
    ///
    /// \see UIWidget::FocusPolicy enumeration.
    void set_focus( uint32 focus = FocusPolicy::FOCUS );

    /// \brief Perform a bounds coordinates collision test.
    bool hit_test( rocket::Element* target, const Point2i& pt );

    void set_title( const std::string& text );

    void set_title_id( const std::string& id );

    void set_font( rocket::Element* target, const std::string& font );
    void set_font_size( rocket::Element* target, int point_size );

    /// \param alignment    One of the following Anchor enumerations:
    /// Anchor::Left, Anchor::Center, Anchor::Right.
    ///
    /// \todo Change return type to boolean
    void set_alignment( rocket::Element* element, uint32 alignment );

    void register_event_listener( rocket::Element* element,
                                  const std::string& ev,
                                  UIEventListener* observer,
                                  bool interruptible = false );

    /// \brief Implements UIWidget::draw method.
    virtual void draw( RenderTarget& target ) const;

    /// \brief Implements UIWidget::update method.
    virtual void update();

  private:
    /// \remarks This pointer is **not** owned by us, and must not be freed.
    rocket::Context* desktop_;

    /// \remarks This pointer is **not** owned by us, and must not be freed.
    rocket::ElementDocument* document_;

    std::string title_id_;
};

} // namespace nom

#endif // include guard defined
