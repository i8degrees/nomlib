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
#ifndef NOMLIB_GUI_BITMAP_BUTTON_HPP
#define NOMLIB_GUI_BITMAP_BUTTON_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/gui/Button.hpp"

namespace nom {

// Forward declarations
class UIWidgetEvent;

/// \brief GUI button with a custom bitmap background container widget.
class BitmapButton: public Button
{
  public:
    typedef BitmapButton self_type;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor.
    ///
    /// \param image A bitmap image that is initialized -- pixel data loaded
    /// from file or memory.
    ///
    /// \deprecated
    BitmapButton  (
                    UIWidget* parent,
                    int64 id,
                    const Point2i& pos,
                    const Size2i& size,
                    const Texture& image
                  );

    /// \brief Construct a nom::BitmapButton from a parent widget.
    ///
    /// \remarks This object instance inherits the properties of the parent,
    // /such as (but not limited to) position, size, name, id and font.
    // BitmapButton( UIWidget* parent, const Texture& image );

    /// \brief Destructor.
    virtual ~BitmapButton( void );

    /// \brief Copy constructor.
    ///
    /// \remarks This class is non-copyable.
    BitmapButton( const self_type& copy ) = delete;

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    /// \brief Get the minimum size of the widget.
    ///
    /// \note Re-implements UIWidget::minimum_size.
    virtual const Size2i minimum_size( void ) const;

    /// \brief Get the preferred size of the widget.
    ///
    /// \note Re-implements UIWidget::size_hint.
    ///
    /// \remarks This value is used when managed by a layout, when the size
    /// policy is set to SizePolicy::Preferred.
    virtual const Size2i size_hint( void ) const;

    /// \brief Implements IDrawable::draw
    virtual void draw( RenderTarget& target ) const;

    /// \brief Get the size dimensions of the default bitmap image.
    ///
    /// \returns The size dimensions of the bitmap image on success. On err,
    /// such as If the default bitmap image is not valid (not initialized),
    /// Size2i::null is returned.
    Size2i bitmap_size( void ) const;

    /// \brief Get the default bitmap image.
    ///
    /// \returns A valid (pixels loaded from file or memory) nom::Texture on
    /// success. An invalid (missing pixel data) on failure, such as when the
    /// image has not been set.
    const Texture& default_bitmap( void ) const;

    /// \brief Get the pressed bitmap image.
    ///
    /// \returns A valid (pixels loaded from file or memory) nom::Texture on
    /// success. An invalid (missing pixel data) on failure, such as when the
    /// image has not been set.
    const Texture& pressed_bitmap( void ) const;

    /// \brief Get the focused bitmap image.
    ///
    /// \returns A valid (pixels loaded from file or memory) nom::Texture on
    /// success. An invalid (missing pixel data) on failure, such as when the
    /// image has not been set.
    const Texture& focused_bitmap( void ) const;

    /// \brief Get the disabled bitmap image.
    ///
    /// \returns A valid (pixels loaded from file or memory) nom::Texture on
    /// success. An invalid (missing pixel data) on failure, such as when the
    /// image has not been set.
    const Texture& disabled_bitmap( void ) const;

    /// \brief Set the default bitmap image to be rendered.
    ///
    /// \param image A nom::Texture object reference that has been initialized
    /// (valid pixel data initialized from a file or memory).
    ///
    /// \see Button::State::Default.
    void set_default_bitmap( const Texture& image );

    /// \brief Set the pressed bitmap image to be rendered.
    ///
    /// \param image A nom::Texture object reference that has been initialized
    /// (valid pixel data initialized from a file or memory).
    ///
    /// \see Button::State::Pressed.
    void set_pressed_bitmap( const Texture& image );

    /// \brief Set the focused bitmap image to be rendered.
    ///
    /// \param image A nom::Texture object reference that has been initialized
    /// (valid pixel data initialized from a file or memory).
    ///
    /// \see Button::State::Focused.
    void set_focused_bitmap( const Texture& image );

    /// \brief Set the disabled bitmap image to be rendered.
    ///
    /// \param image A nom::Texture object reference that has been initialized
    /// (valid pixel data initialized from a file or memory).
    ///
    /// \see Button::State::Disabled.
    void set_disabled_bitmap( const Texture& image );

  protected:
    virtual void update_bounds( void );

    /// \brief Re-implements UIWidget::on_size_changed.
    virtual void on_size_changed( const UIWidgetEvent& ev );

    // virtual void on_mouse_down( const UIWidgetEvent& ev );
    // virtual void on_mouse_up( const UIWidgetEvent& ev );
    // virtual void on_mouse_enter( const UIWidgetEvent& ev );
    // virtual void on_mouse_leave( const UIWidgetEvent& ev );

  private:
    /// \brief Implements IDrawable::update.
    void update( void );

    /// \brief The image data for Button::State::Default (0)
    Texture default_bitmap_;

    /// \brief The image data for Button::State::Pressed (1)
    Texture pressed_bitmap_;

    /// \brief The image data for Button::State::Focused (2)
    Texture focused_bitmap_;

    /// \brief The image data for Button::State::Disabled (3)
    Texture disabled_bitmap_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::BitmapButton
/// \ingroup gui
///
/// The size dimensions of the pressed, focused and disabled bitmaps *must*
/// match the size dimensions of the default bitmap image.
///
