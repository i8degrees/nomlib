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
#ifndef NOMLIB_GUI_UI_LAYOUT_HPP
#define NOMLIB_GUI_UI_LAYOUT_HPP

#include <memory>
#include <vector>
#include <typeinfo>

#include "nomlib/config.hpp"
#include "nomlib/gui/gui_config.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/gui/UIWidgetLayoutItem.hpp"
#include "nomlib/gui/UILayoutItem.hpp"

namespace nom {

// Forward declarations
class UIWidget;
// class UISpacerItem;

/// \brief Abstract class interface for Layout management
///
/// http://doc.qt.digia.com/4.6/qlayout.html#takeAt
class UILayout: public UILayoutItem
{
  public:
    typedef UILayout self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    enum SizeConstraint
    {
      SetDefaultConstraint = 0,
      SetFixedSize,
      SetMinimumSize,
      SetMaximumSize,
      SetMinAndMaxSize,
      SetNoConstraint
    };

    /// \brief Default constructor.
    UILayout( void );

    /// \brief Destructor.
    virtual ~UILayout( void );

    /// \brief Construct a new top-level layout, with a top-level (parent)
    /// widget.
    UILayout( UIWidget* parent );

    /// \remarks The default implementation returns Horizontal or Vertical.
    ///
    /// \note Re-implements UILayoutItem::directions.
    virtual Orientations directions( void ) const;

    /// \brief Return the number of items in the layout.
    ///
    /// \remarks This method must be implemented in the subclass.
    virtual int count( void ) const = 0;

    /// \brief Return the layout item at the specified position (index).
    ///
    /// \remarks This method must be implemented in the subclass.
    virtual UILayoutItem::raw_ptr at( int pos ) const = 0;

    /// \brief Destroy the layout item at the specified position (index).
    ///
    /// \remarks This method must be implemented in the subclass.
    virtual void erase( int pos ) = 0;

    virtual void add_item( UILayoutItem::raw_ptr item ) = 0;

    /// \brief Re-implements UILayoutItem::bounds.
    virtual IntRect bounds( void ) const;

    /// \brief Re-implements UILayoutItem::invalidate.
    virtual void invalidate( void );

    /// \brief Re-implements UILayoutItem::empty.
    ///
    /// \remarks This method uses the required concrete implementation of
    /// nom::UILayout::count.
    bool empty( void ) const;

    /// \remarks This function provides type-safe casting.
    virtual UILayout::raw_ptr layout( void );

    // \returns The parent widget of this layout on success, or NULL if this
    // layout is not installed in any widget.
    //
    // \todo If this layout is a sub-layout, the parent widget of the parent
    // layout is returned.
    UIWidget* parent( void ) const;

    /// \brief Re-implements UILayoutItem::maximum_size.
    ///
    /// \returns The maximum size of the layout. This is the maximum size that
    /// the layout can have while still respecting the specifications.
    ///
    /// \remarks The default implementation allows unlimited resizing.
    ///
    /// \todo The returned value does not include the space required by
    /// nom::UIWidget::set_contents_margins.
    virtual Size2i maximum_size( void ) const;

    /// \brief Re-implements UILayoutItem::minimum_size.
    ///
    /// \returns The minimum size of the layout. This is the smallest size that
    /// the layout can have while still respecting the specifications.
    ///
    /// \remarks The default implementation allows unlimited resizing.
    ///
    /// \todo The returned value does not include the space required by
    /// nom::UIWidget::set_contents_margins.
    virtual Size2i minimum_size( void ) const;

    /// \brief Re-implements UILayoutItem::set_bounds.
    virtual void set_bounds( const IntRect& rect );

    bool activate( void ) const;

    /// \brief The active state of the layout.
    bool enabled( void ) const;

    // bool top_layout( void ) const;

    /// \todo Implement.
    enum SizeConstraint size_constraint( void ) const;

    /// \brief The spacing in between widgets inside the layout.
    ///
    /// \todo If no value is explicitly set, the layout's spacing is inherited
    /// from the parent's layout, or from the style settings for the parent
    /// widget.
    ///
    /// \todo For nom::UIGridLayout and nom::UIFormLayout, it is possible to set
    /// different horizontal and vertical spacings using ::set_horiz_spacing and
    /// ::set_vert_spacing method calls. In this case, ::spacing returns
    /// negative one (-1).
    int spacing( void ) const;

    /// \brief Insert a widget to the layout.
    ///
    /// \remarks This method call uses nom::UILayout::add_item.
    ///
    /// \todo Consider changing return type to int; return position index of
    /// insertion.
    void add_widget( UIWidget* widget );

    /// \note QLayout::indexOf
    virtual int find_widget( const UIWidget* widget ) const;

    /// \brief Destroy a layout item from the layout.
    ///
    /// \returns The position (index) of the removed layout, or nom::npos if no
    /// layout was removed.
    int erase_item( UILayoutItem* layout );

    /// \brief Destroy a widget item from the layout.
    ///
    /// \returns The position (index) of the removed widget, or nom::npos if no
    /// widget was removed.
    int erase_widget( UIWidget* widget );

    void set_alignment( uint32 align );

    bool set_alignment( const UIWidget* widget, uint32 align );

    bool set_alignment( const UILayout::raw_ptr layout, uint32 align );

    void set_margins( const IntRect& margins );

    void set_enabled( bool state );

    void set_size_constraint( enum SizeConstraint size );

    /// \brief Set the spacing in between widgets.
    ///
    /// \remarks There is a known visual spacing glitch that occurs when using
    /// the FinalFantasy border theme style with a set spacing of two (2) or
    /// less.
    void set_spacing( int space );

    /// \brief Update the layout for the parent widget.
    void update( void );

  protected:
    void add_child_widget( UIWidget* widget );

  private:
    /// \brief Helper method for quickly determining if we are in an object
    // instance that is horizontal -- assumption is vertical otherwise.
    bool horiz( void ) const;

    IntRect bounds_;
    IntRect margins_;

    /// \brief State of layout usage.
    bool enabled_;
    int spacing_;
    bool invalid_;

    enum SizeConstraint contraints_;

    /// \brief The direction of this layout -- horizontal or vertical.
    Orientations directions_;

    /// \brief The parent widget that has the layout installed (managed).
    ///
    /// \remarks The pointer is not owned by us; do *not* free this.
    UIWidget* parent_;
};

} // namespace nom

#endif // include guard defined
