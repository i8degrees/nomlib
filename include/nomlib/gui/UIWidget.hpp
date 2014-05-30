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
#ifndef NOMLIB_GUI_UI_WIDGET_HPP
#define NOMLIB_GUI_UI_WIDGET_HPP

#include <vector>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/system/Event.hpp"
#include "nomlib/graphics/shapes/Rectangle.hpp" // tool tip
#include "nomlib/graphics/fonts/Font.hpp"
#include "nomlib/graphics/Text.hpp"
#include "nomlib/gui/UIEventHandler.hpp"
#include "nomlib/gui/IDecorator.hpp"
#include "nomlib/gui/Drawables.hpp"
#include "nomlib/gui/UILayout.hpp"
#include "nomlib/gui/UILayoutPolicy.hpp"
#include "nomlib/gui/UIStyle.hpp"

namespace nom {

// Forward declarations
// class UIStyle;

/// \brief GUI window object container
class UIWidget: public UIEventHandler
{
  public:
    typedef UIWidget self_type;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;
    typedef self_type* raw_ptr;

    /// \todo Move to gui_config.hpp as 'UIWidgetChildren'...?
    typedef std::vector<UIWidget::raw_ptr> Children;

    /// \brief Default constructor
    UIWidget( void );

    /// \brief Destructor.
    virtual ~UIWidget( void );

    /// \brief Copy constructor.
    ///
    /// \remarks This class is non-copyable.
    UIWidget( const self_type& copy ) = delete;

    /// \brief Copy assignment operator.
    self_type& operator =( const self_type& rhs );

    /// \brief Construct a widget.
    ///
    /// \param pos The positioning coordinates of the widget; must be relative
    /// to the positioning coordinates of the top-level window.
    ///
    /// \param size The size of the widget to construct.
    UIWidget  (
                UIWidget* parent,
                int64 id,
                const Point2i& pos,
                const Size2i& size
              );

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    /// \brief Query the validity of the object
    ///
    /// \remarks A valid object must have both the positioning & size bounding
    /// coordinates be set to a non-null object value. See nom::Point2i::null
    /// and nom::Size2i::null for their respective values.
    bool valid( void ) const;

    /// \brief Reserved for future use
    /*const IntRect&*/int padding( void ) const;

    const Font& font( void ) const;

    /// \brief Obtain the internal unique identifier for the object.
    uint32 id( void ) const;

    /// \brief Obtain the unique identifier string for the object.
    const std::string& name( void ) const;

    bool focused( void ) const;

    /// \brief Get the focus policy for the widget.
    ///
    /// \remarks See also, nom::FocusPolicy.
    uint32 focus_policy( void ) const;

    bool parent_has_focus( void ) const;

    UIWidget::raw_ptr parent( void ) const;

    const Children& children( void ) const;

    /// \returns Ownership of the object pointer is not transferred.
    IDecorator::raw_ptr decorator( void ) const;

    /// \brief Get the title of the top-level window.
    const std::string title( void ) const;

    const UILayoutPolicy& size_policy( void ) const;

    /// \brief Perform a bounds coordinates collision test.
    bool contains( const IDrawable::raw_ptr obj, const Point2i& pt );

    /// \brief Perform a bounds coordinates collision test.
    ///
    /// \remarks This is a specialization of the generic (nom::IDrawable)
    /// hit_test method, made to suit nom::Text objects. It may be wise to try
    /// staying away from using this method signature except when absolutely
    /// necessary. On the other hand, it is possible that this method call is
    /// faster than the generic signature, due to the type conversion
    /// differences?
    ///
    /// \note When using nom::Drawables -- generic storage container -- it is
    /// first necessary to up-cast the pointer to nom::Text before you can
    /// rely on the hit test results; the un-casted object will take the
    /// generic method signature otherwise, and much confusion will ensure!
    bool contains( const Text::raw_ptr obj, const Point2i& pt );

    /// \brief Query if this is a top-level widget.
    ///
    /// \returns Boolean TRUE if this is a top-level widget (parent object
    /// pointer is NULL), or boolean FALSE if this is not a top-level widget
    /// (parent object is not NULL).
    bool is_window( void ) const;

    /// \brief Obtain the visibility state of the widget.
    ///
    /// \returns Boolean TRUE if the widget is shown, boolean FALSE if the
    /// widget is hidden.
    bool visible( void ) const;

    /// \brief Get the maximum size of the widget.
    const Size2i maximum_size( void ) const;

    /// \brief Get the minimum size of the widget.
    const Size2i minimum_size( void ) const;

    /// \brief Get the preferred size of the widget.
    ///
    /// \remarks If the preferred size is invalid (NULL), ...
    virtual const Size2i size_hint( void ) const;

    /// \brief Obtain the widget's local positioning coordinates.
    ///
    /// \returns These coordinates are relative to the global (screen)
    /// coordinates.
    // const IntRect local_bounds( void ) const;

    /// \brief Obtain the widget's global (screen, or rendering) coordinates.
    ///
    /// \todo Consider renaming the method?
    const IntRect global_bounds( void ) const;

    /// \brief Get the layout of the widget.
    ///
    /// \returns An object pointer to the UILayout if set, or NULL if a layout
    /// for the widget has not been set.
    UILayout* layout( void ) const;

    std::shared_ptr<UIStyle> style( void ) const;

    /// \brief Reserved for future use
    void set_padding( int pad );

    /// \brief Set the font resource of the widget.
    ///
    /// \remarks This method call will propagate the given font resource to its
    /// children, if any, and request an update of each widget, so that the
    /// resource takes effect immediately. This method call is potentially
    /// expensive.
    ///
    /// \note You'll want to call this method from a widget that is *not* a top-
    /// level window.
    ///
    /// \todo Provide some way of requesting that fonts do *NOT* propagate to
    /// children..?
    void set_font( const Font& font );

    /// \brief Set a font from a nom::Font object pointer.
    ///
    /// \note Used for ResourceCache::load_resource.
    void set_font( const Font* font );

    // The default parameter 'inherit' is intended only as a
    // temporary workaround:
    // void set_font( const IFont* font, bool inherit = true );

    /// \brief Set the unique identifying integer for the object instance.
    ///
    /// \remarks The ID should always be generated using an algorithm that
    /// uniqueness across the current program execution.
    ///
    /// \note See also, nom::UIWidget::generate_id.
    void set_id( uint32 id );

    /// \brief Set a unique string identifier for the object.
    void set_name( const std::string& name );

    /// \brief Set the focus of the widget.
    ///
    /// \remarks This method is intended to be overridden by the widget.
    virtual void set_focused( bool state );

    /// \brief Set the widget's focus policy.
    ///
    /// \param flags An enumeration of FocusPolicy::NoFocus flags -- bit-mask
    /// friendly.
    ///
    /// \remarks The default focus policy for a widget is FocusPolicy::NoFocus.
    void set_focus_policy( uint32 flags );

    void set_parent( const UIWidget::raw_ptr parent );

    /// \brief Attach a decorator to the widget.
    ///
    /// \remarks Passing an object with an invalid (null) position or size will
    /// result in the object's dimensions being set to the parent (this)
    /// dimensions. This is a provided for the convenience of not needing to
    /// explicitly initialize the object with the dimensions of the parent
    /// widget.
    void set_decorator( const IDecorator::raw_ptr object );

    /// \brief Set the window's caption (title) text.
    ///
    /// \remarks This method call will fail if a valid font has not been set.
    ///
    /// \note The window's title is not set by default.
    void set_title( const std::string& title );

    /// \brief Set the default layout behavior of the widget.
    ///
    /// \remarks If there is a UILayout that is managing children, the size
    /// policy specified by that layout is used. If there is no such layout,
    /// the result of this method call is used to set said layout.
    void set_size_policy( uint32 horiz, uint32 vert );

    void set_style( const std::shared_ptr<UIStyle> style );

    void insert_child( const UIWidget::raw_ptr child );

    /// \remarks Pointer ownership is not transferred; you must not free the
    /// returned object pointer.
    // UIWidget::raw_ptr find_window( int64 id );

    /// \remarks Pointer ownership is not transferred; you must not free the
    /// returned object pointer.
    // UIWidget::raw_ptr find_window( const std::string& name );

    /// \remarks Pointer ownership is not transferred; you must not free the
    /// returned object pointer.
    // UIWidget::raw_ptr find_window( const UIWidget::raw_ptr widget ) const;

    /// \remarks Pointer ownership is not transferred; you must not free the
    /// returned object pointer.
    UIWidget::raw_ptr find_child( int64 id );

    /// \remarks Pointer ownership is not transferred; you must not free the
    /// returned object pointer.
    UIWidget::raw_ptr find_child( const std::string& name );

    /// \remarks Pointer ownership is not transferred; you must not free the
    /// returned object pointer.
    UIWidget::raw_ptr find_child( const UIWidget::raw_ptr widget ) const;

    /// \remarks Pointer ownership is not transferred; you must not free the
    /// returned object pointer.
    UIWidget::Children find_children( const UIWidget::raw_ptr widget ) const;

    /// \brief Destroy a widget (child) object.
    ///
    /// \param id Unique identifier of the nom::UIWidget object to be erased.
    bool erase_child( int64 id );

    /// \brief Destroy a widget (child) object.
    ///
    /// \param id Unique identifier of the nom::UIWidget object to be erased.
    bool erase_child( const std::string& name );

    /// \brief Destroy one or more matching widget (children) objects.
    ///
    /// \param id Unique identifier of the nom::UIWidget object(s) to be erased.
    void erase_children( int64 id );

    /// \brief Destroy one or more matching widget (children) objects.
    ///
    /// \param id Unique identifier of the nom::UIWidget objects to be erased.
    void erase_children( const std::string& name );

    /// \brief Destroy all stored widgets (children) objects.
    void clear_children( void );

    /// \brief Implements IDrawable::update.
    virtual void update( void );

    /// \brief Implements IDrawable::draw.
    virtual void draw( RenderTarget& target ) const;

    /// \brief Re-implements EventHandler::process_event.
    ///
    /// \note Emitted UIWidgetEvent objects reflects the state of the widget
    /// data *before* the callback(s) are executed.
    virtual bool process_event( const nom::Event& ev );

    /// \brief Getter for internal updated status.
    bool updated( void ) const;

    /// \brief Set the internal updated status.
    void set_updated( bool state );

    /// \brief Set the visibility state (shown or hidden) of the widget.
    void set_visibility( bool state );

    /// \brief Set the maximum size of the widget.
    void set_maximum_size( const Size2i& size );

    /// \brief Set the minimum size of the widget.
    void set_minimum_size( const Size2i& size );

    /// \brief Set the preferred size of the widget.
    void set_size_hint( const Size2i& size );

    /// \brief Set the global (screen) coordinates of the widget.
    // void set_global_bounds( const Point2i& pos, const Size2i& size );

    /// \brief Set the global (screen) coordinates of the widget.
    // void set_global_bounds( const IntRect& bounds );

    /// \brief Set the layout of the widget.
    void set_layout( UILayout* layout );

  protected:
    virtual void on_size_changed( const UIWidgetEvent& ev );

    virtual void on_mouse_down( const UIWidgetEvent& ev );
    virtual void on_mouse_up( const UIWidgetEvent& ev );

    virtual void on_mouse_enter( const UIWidgetEvent& ev );
    virtual void on_mouse_leave( const UIWidgetEvent& ev );

    virtual void on_mouse_wheel( const UIWidgetEvent& ev );

    virtual void on_key_down( const UIWidgetEvent& ev );
    virtual void on_key_up( const UIWidgetEvent& ev );

    /// \brief Find a new widget to give keyboard focus to.
    ///
    /// \returns Boolean TRUE if a child is available to pass focus onto,
    /// and boolean FALSE if a child is not available to pass focus onto.
    ///
    /// \remarks The default key binding for this event action is Shift+TAB.
    ///
    /// \todo Implement focus logic; we ideally want to have access to every
    /// top-level window object that is active, so that we are able to iterate
    /// through the complete widget tree.
    bool focus_previous_child( void );

    /// \brief Find a new widget to give keyboard focus to.
    ///
    /// \returns Boolean TRUE if a child is available to pass focus onto,
    /// and boolean FALSE if a child is not available to pass focus onto.
    ///
    /// \remarks The default key binding for this event action is TAB.
    ///
    /// \todo Implement focus logic; we ideally want to have access to every
    /// top-level window object that is active, so that we are able to iterate
    /// through the complete widget tree.
    bool focus_next_child( void );

  private:
    /// \brief The unique identifier for a nom::UIWidget object instance.
    static int64 next_window_id_;

    /// \brief Ensure a unique identifier is created for the object instance.
    int64 generate_id( void ) const;

    /// \brief Ensure that the dimensions of the object are in a valid state.
    ///
    /// \param object IDrawable object that inherits from nom::Transformable.
    ///
    /// \remarks If the object is in an invalid (null) state, the dimensions are
    /// set to the parent (this) object's dimensions.
    /// void validate_dimensions( const IDrawable::raw_ptr object );

    /// \brief Unique integer identifier for the object instance; this is used
    /// to keep internal track of nom::UIWidget instances. This variable is
    /// managed internally by this class.
    ///
    /// \remarks This variable can also be used by the end-user (developer)
    /// to perform object lookups. See also: nom::UIWidget::id.
    int64 id_;

    /// \brief Unique string identifier (human-friendly) for the object
    /// instance.
    ///
    /// \note See also, nom::UIWidget::name, nom::UIWidget::set_name.
    std::string name_;

    /// \brief Renderable objects.
    // Drawables drawables_;

    /// Reserved for future use
    /*IntRect*/int padding_;

    /// \brief The font used by children of the widget object.
    Font font_;

    /// \brief The owning object member.
    UIWidget::unique_ptr parent_;

    /// \remarks The object pointers are not owned by us.
    Children children_;

    /// \brief The focused state of the widget.
    bool focused_;

NOM_IGNORED_VARS();
    /// \brief The functionality state of the object.
    ///
    /// \remarks Not implemented; reserved for future use.
    bool enabled_;
NOM_IGNORED_ENDL();

    /// \brief The visibility state of the object (shown or hidden).
    bool visible_;

    /// \brief Internal object state; we re-create drawable objects every time
    /// the internal state changes in order to save as many CPU cycles.
    bool updated_;

    /// \brief The decorator object for the widget
    ///
    /// \remarks Control over the border and background of the widget.
    IDecorator::unique_ptr decorator_;

    /// \brief The title of the top-level window.
    Text::unique_ptr title_;

    /// \brief Maximum size of widget.
    Size2i max_size_;

    /// \brief Minimum size of widget.
    Size2i min_size_;

    /// \brief Preferred size of widget.
    Size2i size_hint_;

    /// \brief The layout of the widget.
    std::unique_ptr<UILayout> layout_;

NOM_IGNORED_VARS();
    Drawables tooltip_;
    uint32 l_time;
    int l_x = 0;
    int l_y = 0;
    bool tip_;
NOM_IGNORED_ENDL();

    /// \brief Widget's focus policy setting.
    ///
    /// \remarks See also, nom::FocusPolicy.
    uint32 focus_policy_;

    UILayoutPolicy policy_;

    std::shared_ptr<UIStyle> style_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::UIWidget
/// \ingroup gui
///
/// Every widget should, at the minimum, set or implement the following:
/// a) setting the minimum size at construction; typically it is OK to use the
/// the explicit values passed by the end-user (developer) as the minimum
/// size requirements of a widget; b) possibly a size policy; c) event listener
/// for ON_WINDOW_SIZE_CHANGED (think: window resize/move event); d) possibly
/// a focus policy; e) a widget name.
///
