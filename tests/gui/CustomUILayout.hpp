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
#ifndef NOMLIB_TESTS_GUI_CUSTOM_UI_LAYOUT_HPP
#define NOMLIB_TESTS_GUI_CUSTOM_UI_LAYOUT_HPP

#include "nomlib/config.hpp"
#include "nomlib/gui/gui_config.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/gui/UILayout.hpp"
#include "nomlib/gui/UIWidget.hpp"

namespace nom {

// Forward declarations
// class UIWidget;

/// \brief Abstract class interface for Layout management
///
/// \note http://doc.qt.digia.com/4.6/layout.html
class CustomUILayout: public UILayout
{
  public:
    typedef CustomUILayout self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor.
    CustomUILayout( void );

    /// \brief Destructor.
    virtual ~CustomUILayout( void );

    /// \brief Construct a new top-level layout, with a parent widget.
    CustomUILayout( UIWidget* parent );

    /// \brief Re-implements UILayoutItem::size_hint.
    Size2i size_hint( void ) const;

    /// \brief Re-implements UILayout::minimum_size.
    Size2i minimum_size( void ) const;

    /// \brief Re-implements UILayout::set_bounds.
    void set_bounds( const IntRect& rect );

    /// \brief Implements UILayout::count.
    int count( void ) const;

    /// \brief Implements UILayout::at.
    UILayoutItem* at( int pos ) const;

    /// \brief Implements UILayout::erase.
    void erase( int pos );

    /// \brief Implements UILayout::add_item.
    void add_item( UILayoutItem::raw_ptr item );

  private:
    std::vector<UILayoutItem*> items_;
};

} // namespace nom

#endif // include guard defined
