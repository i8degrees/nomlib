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
#ifndef NOMLIB_SDL2_IFONT_HPP
#define NOMLIB_SDL2_IFONT_HPP

#include <iostream>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/fonts/Glyph.hpp"
#include "nomlib/graphics/fonts/FontMetrics.hpp"

namespace nom {

// Forward declarations
struct FontMetrics;

/// \brief Pure abstract interface for a font resource
class IFont
{
  public:
    typedef IFont self_type;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;

    enum FontType
    {
      NotDefined = 0,
      BitmapFont,
      TrueTypeFont
    };

    IFont( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    virtual ~IFont( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    virtual IFont::raw_ptr clone( void ) const = 0;
    virtual bool valid ( void ) const = 0;

    virtual const Image& image ( uint32 ) const = 0;
    virtual enum IFont::FontType type ( void ) const = 0;

    virtual const Glyph& glyph ( uint32, uint32 ) const = 0;
    virtual int newline( uint32 ) const = 0;
    virtual sint spacing ( uint32 ) const = 0;
    virtual int kerning( uint32, uint32, uint32 ) const = 0;
    virtual int hinting( void ) const = 0;
    virtual const FontMetrics& metrics( void ) const = 0;

    virtual bool set_point_size( int ) = 0;
    virtual bool set_hinting( int ) = 0;
    virtual bool set_outline( int ) = 0;

    virtual bool load( const std::string& filename ) = 0;
};

} // namespace nom

#endif // include guard defined

/// \class nom::IFont
/// \ingroup graphics/fonts
///
/// nomlib's built-in font classes all derive from this pure abstract class.
/// nom::Text focuses solely on text rendering, utilizing this common interface
/// defined here.
///
/// In short: a font resource class is responsible for computing available
/// glyphs (including their bounds, sizes, etc.) and most importantly, a
/// complete cache of usable glyph bitmaps laid out in a texture atlas (in no
/// particular packing order). nom::Text uses the font's texture atlas in
/// addition to the font's glyph metrics (the bounds being most critical for
/// rendering from this atlas to the screen).
///
/// See the source files of nom::BitmapFont for a complete example of how to
/// write a custom font resource class that is suitable for nom::Text to
/// render from. Supporting data structures include: nom::FontMetrics,
/// nom::FontPage, nom::FontRow, nom::Glyph and so on.
///
/// \code
///
///       [USAGE EXAMPLE STUB]
///
/// \endcode
