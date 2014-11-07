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
#ifndef NOMLIB_GRAPHICS_FONTS_FONT_METRICS_HPP
#define NOMLIB_GRAPHICS_FONTS_FONT_METRICS_HPP

#include <string>

#include "nomlib/config.hpp"

namespace nom {

/// \brief Container structure for global font metric data
struct FontMetrics
{
  /// Default constructor
  FontMetrics ( void )  :
    height ( 0 ),
    newline ( 0 ),
    ascent ( 0 ),
    descent ( 0 )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Destructor
  ~FontMetrics ( void )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Maximum height of a font
  sint height;

  /// \brief Get the line spacing
  ///
  /// \remarks Line spacing is the vertical offset to apply between two
  /// consecutive lines of text.
  int newline;

  /// \brief The height (in pixels) from the absolute top of the glyph to the
  /// base of the glyph.
  int ascent;

  /// \brief Height (in pixels) from the baseline to the bottom of the glyph.
  int descent;

  /// Font family
  std::string family;

  /// \brief The font face style name
  std::string name;
};

} // namespace nom

#endif // include guard defined
