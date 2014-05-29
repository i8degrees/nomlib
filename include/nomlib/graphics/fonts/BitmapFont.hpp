/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Portions Copyright (c) 2004-2013 Lazy Foo' Productions [1]
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

1. http://lazyfoo.net/SDL_tutorials/lesson30/index.php

******************************************************************************/
#ifndef NOMLIB_SDL2_BITMAP_FONT_HEADERS
#define NOMLIB_SDL2_BITMAP_FONT_HEADERS

#include <iostream>
#include <string>
#include <sstream>

#include "nomlib/config.hpp"
#include "nomlib/graphics/fonts/IFont.hpp"
#include "nomlib/graphics/fonts/FontMetrics.hpp"
#include "nomlib/graphics/fonts/FontPage.hpp"

// Dump glyph bounding coordinates
//#define NOM_DEBUG_SDL2_BITMAP_FONT

namespace nom {

// Forward declarations
class Image;

class BitmapFont: public IFont
{
  public:
    typedef BitmapFont self_type;
    typedef self_type* RawPtr;
    typedef std::shared_ptr<self_type> SharedPtr;

    /// \brief Default constructor
    BitmapFont ( void );

    /// \brief Destructor
    ~BitmapFont ( void );

    /// \brief Copy constructor
    BitmapFont ( const BitmapFont& copy );

    /// \brief Construct a clone of the existing instance
    IFont::raw_ptr clone( void ) const;

    /// \brief Validity check
    bool valid ( void ) const;

    enum IFont::FontType type ( void ) const;

    const Image& image ( uint32 character_size ) const;

    /// \brief Obtain text character spacing width in pixels
    ///
    /// \returns  The width applied when the space carriage is encountered when
    ///           rendered.
    sint spacing ( uint32 character_size ) const;

    /// \brief Obtain font's line spacing
    ///
    /// \param character_size Point size in pixels
    ///
    /// \returns  Height offset in pixels
    int newline( uint32 character_size ) const;

    int kerning( uint32 first_char, uint32 second_char, uint32 character_size ) const;

    /// \brief Get the font's hinting style.
    ///
    /// \remarks This method is not implemented.
    ///
    /// \returns Zero.
    int hinting( void ) const;

    /// \brief Obtain a glyph
    ///
    /// \param    codepoint        ASCII character to lookup
    /// \param    character_size   Reserved for future implementation
    ///
    /// \returns  nom::Glyph structure
    const Glyph& glyph ( uint32 codepoint, uint32 character_size ) const;

    /// \brief Set a new font point size
    ///
    /// \param size Point size in pixels
    ///
    /// \remarks Not implemented
    bool set_point_size ( sint size );

    /// \brief Set the requested font hinting style.
    ///
    /// \remarks This method is not implemented.
    bool set_hinting( int type );

    /// \remarks This method is not implemented.
    bool set_outline( int );

    /// \brief Loads a new bitmap font from a file
    bool load( const std::string& filename );

    /// \brief Obtain information about the loaded font
    ///
    /// \remarks Implements IFont::metrics.
    const FontMetrics& metrics( void ) const;

  private:
    /// Trigger a build of the font characteristics gleaned from the image file;
    /// recalculate the character sizes, coordinate origins, spacing, etc.
    ///
    /// \param character_size   Reserved for future implementation.
    bool build ( uint32 character_size );

    const GlyphPage& pages ( void ) const;

    sint sheet_width ( void ) const;
    sint sheet_height ( void ) const;

    /// The type of font we are
    const enum IFont::FontType type_;

    /// Width -- in pixels -- of overall texture atlas sheet
    sint sheet_width_;

    /// Height -- in pixels -- of overall texture atlas sheet
    sint sheet_height_;

    /// Table mapping a character size to its page -- a texture atlas combined
    /// with corresponding glyphs data.
    mutable GlyphPage pages_;

    /// General font metric data, such as the proper value for newline spacing
    FontMetrics metrics_;
};

} // namespace nom

#endif // include guard defined
