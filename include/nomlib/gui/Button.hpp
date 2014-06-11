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
#ifndef NOMLIB_GUI_BUTTON_HPP
#define NOMLIB_GUI_BUTTON_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/Text.hpp"
#include "nomlib/gui/UIWidget.hpp"

namespace nom {

/// \brief Basic GUI button container widget.
class Button: public UIWidget
{
  public:
    typedef Button self_type;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    enum State
    {
      Default = 0,
      Pressed,
      Focused,
      Disabled    // Reserved for future implementation
    };

    /// \brief Default constructor.
    ///
    /// \deprecated
    Button  (
              UIWidget* parent,
              int64 id,
              const Point2i& pos,
              const Size2i& size
            );

    /// \brief Construct a nom::Button from a parent widget.
    ///
    /// \remarks This object instance inherits the properties of the parent,
    // /such as (but not limited to) position, size, name, id and font.
    // Button( UIWidget* parent );

    /// \brief Destructor.
    virtual ~Button( void );

    /// \brief Copy constructor.
    ///
    /// \remarks This class is non-copyable.
    Button( const self_type& copy ) = delete;

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    /// \brief Get the preferred size of the widget.
    ///
    /// \note Re-implements UIWidget::size_hint.
    ///
    /// \remarks This value is used when managed by a layout, when the size
    /// policy is set to SizePolicy::Preferred.
    ///
    /// \todo Add the decorator's frame (if any) to the size calculations?
    virtual const Size2i size_hint( void ) const;

    /// \brief Re-implements IDrawable::draw.
    virtual void draw( RenderTarget& target ) const;

    /// \brief Get the state of the button.
    ///
    /// \returns One of the enumeration types.
    Button::State button_state( void ) const;

    const std::string& label_text( void ) const;

    void set_label( const std::string& text );

    /// \see The Button::State enumeration.
    void set_button_state( Button::State state );

  protected:
    virtual void update_bounds( void );

    /// \brief Re-implements UIWidget::on_size_changed.
    virtual void on_size_changed( const UIWidgetEvent& ev );

    virtual void on_update( const UIWidgetEvent& ev );

    virtual void on_mouse_down( const UIWidgetEvent& ev );
    virtual void on_mouse_up( const UIWidgetEvent& ev );

    virtual void on_mouse_enter( const UIWidgetEvent& ev );
    virtual void on_mouse_leave( const UIWidgetEvent& ev );

  private:
    /// \brief Implements IDrawable::update.
    void update( void );

    std::string text_;
    Text label_;

    Button::State button_state_;
};

} // namespace nom

#endif // include guard defined
