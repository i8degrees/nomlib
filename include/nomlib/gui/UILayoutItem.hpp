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
#ifndef NOMLIB_GUI_UI_LAYOUT_ITEM_HPP
#define NOMLIB_GUI_UI_LAYOUT_ITEM_HPP

#include <memory>
#include <typeinfo>

#include "nomlib/config.hpp"
#include "nomlib/gui/gui_config.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Rect.hpp"

namespace nom {

// Forward declarations
class UISpacerItem;
class UILayout;
class UIWidget;

/// \brief Abstract class for UI widget layout
///
/// http://doc.qt.digia.com/4.6/qlayoutitem.html#layout
///
/// \note Implemented as per qlayoutitem.cpp, branch 4.6
class UILayoutItem
{
  public:
    typedef UILayoutItem self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Maximum layout size.
    ///
    /// \note QLAYOUTSIZE_MAX; exact value taken from Qt Designer.
    static const Size2i layout_max_;

    /// \brief Default constructor.
    UILayoutItem( void );

    /// \brief Destructor.
    virtual ~UILayoutItem( void );

    /// \brief Construct an object with an alignment.
    UILayoutItem( uint32 align );

    /// \remarks Implements UILayoutItem::directions.
    ///
    /// \note QLayoutItem::expandingDirections.
    virtual Orientations directions( void ) const = 0;

    /// \note QLayoutItem::bounds.
    virtual IntRect bounds( void ) const = 0;

    /// \note QLayoutItem::isEmpty.
    virtual bool empty( void ) const = 0;

    /// \note QLayoutItem::maximumSize.
    virtual Size2i maximum_size( void ) const = 0;

    /// \note QLayoutItem::minimumSize.
    virtual Size2i minimum_size( void ) const = 0;

    /// \note QLayoutItem::setGeometry.
    virtual void set_bounds( const IntRect& rect ) = 0;

    /// \note QLayoutItem::sizeHint.
    virtual Size2i size_hint( void ) const = 0;

    /// \brief Invalidate any cached information in this layout item.
    ///
    /// \note QLayoutItem::invalidate.
    virtual void invalidate( void );

    /// \note QLayoutItem::alignment.
    uint32 alignment( void ) const;

    /// \note QLayoutItem::setAlignment.
    void set_alignment( uint32 align );

    /// \remarks This function provides type-safe casting.
    ///
    /// \note QLayoutItem::spacerItem.
    virtual UISpacerItem* spacer_item( void );

    /// \returns If this item is a UILayout, it is returned as a UILayout,
    /// otherwise NULL is returned.
    ///
    /// \remarks This function provides type-safe casting.
    ///
    /// \note QLayoutItem::layout.
    virtual UILayout* layout( void );

    /// \remarks This function provides type-safe casting.
    ///
    /// \note QLayoutItem::widget.
    virtual UIWidget* widget( void ) const;

    // virtual bool hasHeightForWidth() const;
    // virtual int heightForWidth(int) const;
    // virtual int minimumHeightForWidth(int) const;
    // QSizePolicy::ControlTypes controlTypes() const;

  private:
    uint32 alignment_;
};

} // namespace nom

#endif // include guard defined
