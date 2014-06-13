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
#ifndef NOMLIB_GUI_CONFIG_HPP
#define NOMLIB_GUI_CONFIG_HPP

#include <memory>
#include <vector>

#include "nomlib/config.hpp"

namespace nom {

/// \brief A integer indicating to the constructor of UIWidget to auto-generate
/// the internal identifier of the object.
static const int AUTO_ID = -1;

/// \brief The maximum size a widget may be.
///
/// \note The value is borrowed from the Qt v4.6 docs.
static const int WIDGET_MAX_SIZE = 16777215;

/// \note Borrowed from Qt 4.6
enum Orientations: uint32
{
  Horizontal = 0x1,
  Vertical = 0x2
};

/// \brief Alignment types.
///
/// \remarks See also, nom::Anchor enumeration.
///
/// \note Borrowed from [maelstrom's screenlib](https://hg.libsdl.org/Maelstrom/)
enum Alignment: uint32
{
  X_LEFT = 0x01,
  X_CENTER = 0x02,
  X_RIGHT = 0x4,

  /// \remarks Horizontal
  X_MASK = ( X_LEFT | X_CENTER | X_RIGHT ),

  Y_TOP = 0x10,
  Y_CENTER = 0x20,
  Y_BOTTOM = 0x40,

  /// \remarks Vertical
  Y_MASK = ( Y_TOP | Y_CENTER | Y_BOTTOM )
};

/// \note Borrowed from [maelstrom's screenlib](https://hg.libsdl.org/Maelstrom/)
enum Anchor: uint32
{
  TopLeft = Y_TOP | X_LEFT,   // Hex: 0x11, Dec: 17
  Top = Y_TOP | X_CENTER,
  TopRight = Y_TOP | X_RIGHT, // Hex: 0x14, Dec: 20

  Left = Y_CENTER | X_LEFT,
  Center = Y_CENTER | X_CENTER,
  Right = Y_CENTER | X_RIGHT,

  BottomLeft = Y_BOTTOM | X_LEFT,
  Bottom = Y_BOTTOM | X_CENTER,
  BottomRight = Y_BOTTOM | X_RIGHT
};

/// \note Borrowed from Qt 4.6
enum FocusPolicy: uint32
{
  /// Widget accepts focus by tabbing.
  TabFocus = 0x1,

  /// Widget accepts focus by clicking.
  ClickFocus = 0x2,

  /// Widget accepts focus by both tabbing and clicking.
  StrongFocus = TabFocus | ClickFocus | 0x8,

  /// Widget accepts focus by both tabbing and clicking (StrongFocus), in
  /// addition to accepting focus via mouse wheel.
  WheelFocus = StrongFocus | 0x4,

  /// Widget does not accept focus.
  NoFocus = 0
};

typedef std::vector<std::string> ItemStrings;

/// \brief Internal data container type of drawable objects.
// typedef std::vector<Text::unique_ptr> ItemLabels;

} // namespace nom

#endif // include guard defined
