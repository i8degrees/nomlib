/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/Glyph.hpp"
#include "nomlib/graphics/fonts/FontRow.hpp"

namespace nom {

/// \brief Container structure for font data
struct FontPage
{
  /// Default constructor
  FontPage ( void ) :
    texture { new Image() }
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
  std::shared_ptr<Image> texture;

  /// Positioning of all the existing rows
  std::vector<FontRow> rows;
};

/// Table mapping glyph data with its corresponding texture
typedef std::map<uint32, FontPage> GlyphPage;

} // namespace nom

#endif // include guard defined
