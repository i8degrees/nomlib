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
#ifndef NOMLIB_GUI_UI_BOX_LAYOUT_HPP
#define NOMLIB_GUI_UI_BOX_LAYOUT_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/gui/gui_config.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/gui/UILayout.hpp"
#include "nomlib/gui/UISpacerItem.hpp"
#include "nomlib/gui/UIWidget.hpp"

namespace nom {

// Forward declaration
class IDecorator;

/// \brief Abstract class interface for Layout management
///
/// \note http://doc.qt.digia.com/4.6/layout.html
/// \note http://doc.qt.digia.com/4.6/qboxlayout.html
/// \note http://doc.qt.digia.com/4.6/layouts-borderlayout.html
class UIBoxLayout: public UILayout
{
  public:
    typedef UIBoxLayout self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    enum Direction
    {
      LeftRight = 0,    /// Default; (American) horizontal layout
      RightLeft,        /// Not implemented; reserved for future use.
      TopBottom,        /// (American) Vertical layout
      BottomTop         /// Not implemented; reserved for future use.
    };

    /// \brief Default constructor; initialize spacing to one (1).
    UIBoxLayout( void );

    /// \brief Destructor.
    virtual ~UIBoxLayout( void );

    /// \brief Construct a new top-level layout, with a parent widget.
    ///
    /// \param parent The parent widget (top-level window typically) to be set
    /// with this layout as its manager. This may be initialized as NULL.
    UIBoxLayout( enum Direction dir, UIWidget* parent );

    /// \remarks Re-implements UILayoutItem::directions.
    virtual Orientations directions( void ) const;

    /// \brief The layout item's preferred size.
    ///
    /// \note Re-implements UILayoutItem::size_hint.
    ///
    /// \note The layout object's preferred size will be the largest element's
    /// preferred size dimensions.
    virtual Size2i size_hint( void ) const;

    /// \brief The accumulate total of every layout item's preferred size.
    Size2i total_size_hint( void ) const;

    /// \brief The layout item's minimum size requirements.
    ///
    /// \brief Re-implements UILayout::minimum_size.
    ///
    /// \note The layout object's preferred size will be the largest element's
    /// minimum size dimensions.
    virtual Size2i minimum_size( void ) const;

    /// \brief The accumulate total of every layout item's minimum size.
    Size2i total_minimum_size( void ) const;

    /// \brief Implements UILayout::count.
    virtual int count( void ) const;

    /// \brief Implements UILayout::at.
    virtual UILayoutItem* at( int pos ) const;

    enum Direction direction( void ) const;

    /// \brief Re-implements UILayout::set_bounds.
    virtual void set_bounds( const IntRect& rect );

    /// \brief Implements UILayout::add_item.
    ///
    /// \todo Rename to append_item or append_layout?
    virtual void add_item( UILayoutItem* item );

    /// \brief Implements UILayout::erase.
    virtual void erase( int pos );

    /// \todo Rename to insert_layout?
    void insert_item( int pos, UILayoutItem* item );

    /// \todo Rename to append_layout?
    void append_item( UILayoutItem* item );

    void insert_widget( int pos, UIWidget* widget );

    /// \todo Finish implementation of this method; the alignment code in
    /// UIWidgetLayoutItem::set_bounds should do nicely.
    void insert_widget( int pos, UIWidget* widget, uint32 align );

    void append_widget( UIWidget* widget );

    void append_widget( UIWidget* widget, uint32 align );

    /// \brief Insert a spacer layout item into the layout.
    void insert_spacer( int pos, UISpacerItem* item );

    /// \brief Insert a spacer layout item into the layout.
    void insert_spacer( int pos, int height );

    /// \brief Insert a spacer layout item into the layout.
    void append_spacer( UISpacerItem* item );

    /// \brief Insert a spacer layout item into the layout.
    void append_spacer( int height );

    void set_direction( enum Direction dir );

  private:
    /// \brief Helper method for quickly determining if we are in an object
    // instance that is horizontal -- assumption is vertical otherwise.
    bool horiz( void ) const;

    /// \brief Storage container for each of our layout items.
    ///
    /// \remarks This necessarily needs to be a raw pointer, due to ownership
    /// semantics (we are borrowing memory pointers from nom::UIWidget, the
    /// rightful owner).
    std::vector<UILayoutItem*> items_;

    enum Direction direction_;
};

} // namespace nom

#endif // include guard defined
