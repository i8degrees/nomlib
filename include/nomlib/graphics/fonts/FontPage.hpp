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
#ifndef NOMLIB_GRAPHICS_FONT_PAGE_HPP
#define NOMLIB_GRAPHICS_FONT_PAGE_HPP

#include <map>

#include "nomlib/config.hpp"
#include "nomlib/graphics/Image.hpp"
#include "nomlib/graphics/fonts/Glyph.hpp"
#include "nomlib/graphics/fonts/FontRow.hpp"

namespace nom {

/// \brief Container structure for font data
struct FontPage
{
  /// Default constructor
  FontPage ( void ) :
    texture { new Image() },
    next_row ( 0 )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Destructor
  ~FontPage ( void )
  {
    //NOM_LOG_TRACE(NOM);
  }

  GlyphAtlas glyphs;

  /// Container for the glyph's pixel buffer
  Image::SharedPtr texture;

  /// Y position of the next new row in the texture
  uint next_row;

  /// Positioning of all the existing rows
  PageRow rows;
};

/// Table mapping glyph data with its corresponding texture
typedef std::map<uint32, FontPage> GlyphPage;

} // namespace nom

#endif // include guard defined
