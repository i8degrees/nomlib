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
#ifndef NOMLIB_GRAPHICS_GLYPH_HPP
#define NOMLIB_GRAPHICS_GLYPH_HPP

#include <map>

#include "nomlib/config.hpp"
#include "nomlib/math/Rect.hpp"

namespace nom {

/// Delimiter character to use with << operator
const std::string GLYPH_DELIMITER = ", ";

/// \brief Container structure for font data
struct Glyph
{
  Glyph ( void );

  ~Glyph ( void );

  /// \brief Bounding rectangle coordinates of the glyph as per the texture
  /// source.
  IntRect bounds;

  /// Offset to move horizontally to the next character (spacing)
  int advance;

  /// \brief The additional X, Y rendering offsets to apply when copying the
  /// glyph from the texture source to the output.
  ///
  /// \remarks This member field is implemented to support the BMFont file
  /// spec -- char tag's 'xoffset' and 'yoffset' fields.
  ///
  /// \see nom::BMFont, nom::Text.
  Point2i offset = Point2i::zero;
};

/// Table mapping glyph data with its corresponding texture
typedef std::map<uint32, Glyph> GlyphAtlas;

/// Pretty print the glyph
inline std::ostream& operator << ( std::ostream& os, const Glyph& mode );

/// Compare two glyphs for equality
inline bool operator == ( const Glyph& lhs, const Glyph& rhs );

/// Compare two glyphs for less-than equality
inline bool operator < ( const Glyph& lhs, const Glyph& rhs );

/// Compare two glyphs for greater-than equality
inline bool operator > ( const Glyph& lhs, const Glyph& rhs );

} // namespace nom

#endif // include guard defined
