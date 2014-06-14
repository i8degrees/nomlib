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
#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/gui/UIWidget.hpp"
#include "nomlib/gui/UIHBoxLayout.hpp"
#include "nomlib/gui/UIVBoxLayout.hpp"
#include "nomlib/gui/UIWidgetEvent.hpp"

namespace nom {

namespace priv {

/// \brief Default callback method used in registering event listeners.
void on_click( const nom::UIWidgetEvent& ev );

/// \brief Internal helper method for verifying expected output across tests.
///
/// \remarks You should use expected_layout_spacer_output unless you have
/// specific needs, such as needing to override / ignore the size dimensions.
void expected_layout_spacer_pos( const UILayout* layout, int idx, const Point2i& pos );

/// \brief Internal helper method for verifying expected output across tests.
///
/// \remarks You should use expected_layout_spacer_output unless you have
/// specific needs, such as needing to override / ignore the position results.
void expected_layout_spacer_dims( const UILayout* layout, int idx, const Size2i& dims );

/// \brief Helper method for verifying expected output across tests.
void expected_layout_spacer_output( const UILayout* layout, int idx, const Point2i& pos, const Size2i& dims );

/// \brief Internal helper method for verifying expected output across tests.
///
/// \remarks You should use expected_layout_widget_output unless you have
/// specific needs, such as needing to override / ignore the size dimensions.
void expected_layout_widget_pos( const UILayout* layout, int idx, const Point2i& pos );

/// \brief Internal helper method for verifying expected output across tests.
///
/// \remarks You should use expected_layout_widget_output unless you have
/// specific needs, such as needing to override / ignore the position results.
void expected_layout_widget_dims( const UILayout* layout, int idx, const Size2i& dims );

/// \brief Helper method for verifying expected output across tests.
void expected_layout_widget_output( const UILayout* layout, int idx, const Point2i& pos, const Size2i& dims );

/// \brief Helper method for verifying expected output across tests.
///
/// \param pos A vector containing two Point2i objects that describe the
/// position of the first widget item in the layout and the last widget in the
/// layout (order matters).
///
/// \param dims A vector containing two Size2i objects that describe the
/// dimensions of the first widget item in the layout and the last widget in the
/// layout (order matters).
void expected_layout_output( const UILayout* layout, const std::vector<Point2i>& pos, const std::vector<Size2i>& dims );

/// \brief Helper method for creating a layout and verifying results across
/// tests in a consistent manner.
UIBoxLayout* create_layout( UIWidget* window, const std::vector<UIWidget*>& items, const std::vector<int>& spacers, const std::string& name, Orientations dir, int spacing = 1 );

void set_layout_alignment( UILayout* layout, uint32 align );
void set_widget_alignment( UILayout* layout, const UIWidget* window, uint32 align );

} // namespace priv

} // namespace nom
