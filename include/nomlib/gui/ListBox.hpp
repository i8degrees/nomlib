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
#ifndef NOMLIB_GUI_LIST_BOX_HPP
#define NOMLIB_GUI_LIST_BOX_HPP

#include <vector>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/Text.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/gui/UIWidget.hpp"
#include "nomlib/gui/Drawables.hpp"
#include "nomlib/gui/UIItemContainer.hpp"

namespace nom {

/// \brief GUI list box widget.
class ListBox: public UIWidget
{
  public:
    typedef ListBox self_type;
    typedef UIWidget derived_class;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor.
    ListBox (
              UIWidget* parent,
              int64 id,
              const Point2i& pos,
              const Size2i& size,
              const UIItemContainer::raw_ptr store
            );

    /// \brief Destructor; resource clean up is automatic.
    virtual ~ListBox( void );

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
    virtual const Size2i size_hint( void ) const;

    /// \brief Query the validity of the object
    ///
    /// \remarks A valid object must have both the positioning & size bounding
    /// coordinates set to a non-null object value (used for rendering), in
    /// addition to a non-null object pointer to the internal item storage
    /// container.
    bool valid( void ) const;

    /// \brief Obtain a pointer to the internal storage object in use.
    ///
    /// \remarks An assert will be triggered upon detecting a null store
    /// object pointer.
    UIItemContainer::raw_ptr store( void ) const;

    /// \brief Obtian the color used for the rendering of highlighted text.
    const Color4i& selected_text_color( void ) const;

    /// \brief Implements the nom::UIWidget::draw method.
    void draw( RenderTarget& target ) const;

    /// \brief Set the internal storage container for the object to manage.
    void set_item_store( const UIItemContainer::raw_ptr store );

    /// \brief Set the text color used to highlight the current selection
    /// item.
    void set_selected_text_color( const Color4i& color );

    /// \brief Perform a bounds coordinates collision test.
    int hit_test( const Point2i& pt );

  protected:
    /// \brief Re-implements UIWidget::on_size_changed.
    virtual void on_size_changed( const UIWidgetEvent& ev );

    /// \brief Default event listener for handling key press events.
    ///
    /// \remarks This method handles the updating the current selection text
    /// label, which in turn, also updates the widget. The call to update
    /// allows -- amongst other things -- the ability to update the text color
    /// to the selection text.
    virtual void on_key_down( const UIWidgetEvent& ev );

    /// \brief Default event listener for handling mouse down events.
    ///
    /// \todo Explain the contrast in why this method call is vastly simplified
    /// in comparison to the method logic in on_key_down and on_mouse_down.
    virtual void on_mouse_down( const UIWidgetEvent& ev );

    virtual void on_mouse_enter( const UIWidgetEvent& ev );

    /// \brief Default event listener for handling mouse wheel events.
    ///
    /// \remarks This method handles the updating the current selection text
    /// label, which in turn, also updates the widget. The call to update
    /// allows -- amongst other things -- the ability to update the text color
    /// to the selection text.
    virtual void on_mouse_wheel( const UIWidgetEvent& ev );

    /// \brief Re-implements UIWidget::set_focused.
    void set_focused( bool state );

  private:
    /// \brief Implements the nom::UIWidget::update method.
    ///
    /// \todo Provide logic for customizing positioning and alignment of the
    /// text.
    ///
    /// \todo Automatic best fit calculations with consideration to the
    /// text dimensions (font size, text dimensions, ...), number of elements
    /// and so on.
    void update( void );

    /// \brief Internal storage container for holding items.
    UIItemContainer::unique_ptr store_;

    /// \brief Rendered text items.
    ///
    /// \remarks This container is generated from the internal storage
    /// container's items.
    ///
    /// \todo Use labels_.updated() && labels_.set_updated() ...?
    Drawables labels_;

    /// \brief The text color used to highlight the selection text label.
    Color4i selected_text_color_;
};

} // namespace nom

#endif // include guard defined
