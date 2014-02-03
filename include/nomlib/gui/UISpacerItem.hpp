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
#ifndef NOMLIB_GUI_UI_SPACER_ITEM_HPP
#define NOMLIB_GUI_UI_SPACER_ITEM_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/gui/gui_config.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/gui/UILayoutItem.hpp"
#include "nomlib/gui/UILayoutPolicy.hpp"

namespace nom {

/// \brief Spacer (blank space) layout element.
///
/// http://doc.qt.digia.com/4.6/qspaceritem.html
///
/// \note Implemented as per qlayoutitem.cpp, branch 4.6
class UISpacerItem: public UILayoutItem
{
  public:
    typedef UISpacerItem self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor.
    UISpacerItem( void );

    /// \brief Destructor.
    virtual ~UISpacerItem( void );

    /// \brief Construct a complete UI spacer item.
    UISpacerItem( const Size2i& size, uint32 horiz_policy, uint32 vert_policy );

    /// \brief Re-implements UILayoutItem::empty.
    virtual bool empty( void ) const;

    /// \brief Re-implements UILayoutItem::directions.
    virtual enum Orientations directions( void ) const;

    /// \brief Re-implements UILayoutItem::bounds.
    virtual IntRect bounds( void ) const;

    /// \brief Re-implements UILayoutItem::maximum_size.
    virtual Size2i maximum_size( void ) const;

    /// \brief Re-implements UILayoutItem::minimum_size.
    virtual Size2i minimum_size( void ) const;

    /// \brief Re-implements UILayoutItem::set_bounds.
    virtual void set_bounds( const IntRect& rect );

    /// \brief Re-implements UILayoutItem::size_hint.
    virtual Size2i size_hint( void ) const;

    /// \brief Re-implements UILayoutItem::spacer_item.
    virtual UISpacerItem::raw_ptr spacer_item( void );

    virtual Size2i size( void ) const;

    /// \note If you call this method after the spacer item has been added to
    /// a layout, it will be necessary to invalidate the layout in order for
    /// the spacer item's new size to take effect.
    void set_size( const Size2i& size, uint32 horiz_policy, uint32 vert_policy );

  private:
    Size2i size_;

    IntRect bounds_;

    UILayoutPolicy policy_;
};

} // namespace nom

#endif // include guard defined
