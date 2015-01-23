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
#ifndef NOMLIB_GRAPHICS_DISPLAY_MODE_HPP
#define NOMLIB_GRAPHICS_DISPLAY_MODE_HPP

// #include <memory>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/math/Size2.hpp"

namespace nom {

/// \brief Container for describing a video display mode
struct DisplayMode
{
  /// \brief The pixel format of the display video mode.
  ///
  /// \remarks One of the SDL_PixelFormatEnum enumeration values.
  uint32 format = 0;

  /// \brief The horizontal and vertical dimensions, in pixel units, of the
  /// display video mode.
  Size2i bounds;

  /// \brief The display vertical refresh rate of the display video mode.
  ///
  /// \remarks The display's vertical refresh rate, in hertz. Zero (0) if
  /// unspecified.
  int refresh_rate = 0;
};

typedef std::vector<DisplayMode> DisplayModeList;

/// \brief Compare two video modes for equality.
bool operator ==(const DisplayMode& lhs, const DisplayMode& rhs);

/// \brief Compare two video modes for inequality.
bool operator !=(const DisplayMode& lhs, const DisplayMode& rhs);

/// \brief Compare two video modes for lesser than.
bool operator <(const DisplayMode& lhs, const DisplayMode& rhs);

/// \brief Compare two video modes for greater than.
bool operator >(const DisplayMode& lhs, const DisplayMode& rhs);

/// \brief Compare two video modes for lesser than or equal to.
bool operator <=(const DisplayMode& lhs, const DisplayMode& rhs);

/// \brief Compare two video modes for greater than or equal to.
bool operator >=(const DisplayMode& lhs, const DisplayMode& rhs);

} // namespace nom

#endif // include guard defined
