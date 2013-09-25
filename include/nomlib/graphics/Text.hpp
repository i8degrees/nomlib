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
#ifndef NOMLIB_TEXT_HPP
#define NOMLIB_TEXT_HPP

#include <iostream>
#include <string>

#include "nomlib/config.hpp"

#include "nomlib/graphics/Canvas.hpp"
#include "nomlib/graphics/IFont.hpp"
#include "nomlib/graphics/BitmapFont.hpp"
#include "nomlib/graphics/TrueTypeFont.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/system/File.hpp"

namespace nom {

enum FontType
{
  Unknown = 0,
  Bitmap = 1,
  TrueType = 2
};

/// \brief Unified text output interface; designed for wrapping Bitmap &
/// TrueType fonts together to co-exist under one API.
class Text: public IDrawable
{
  public:
    Text ( void );
    virtual ~Text ( void );

    /// Load a new font into memory from a file
    ///
    /// See nom::BitmapFont and nom::TrueTypeFont documentation for supported
    /// file formats.
    bool load ( const std::string& filename,
                const Color& colorkey = Color::Black,
                bool use_cache = false
              );

    FontType getFontType ( void );

    const std::string getText ( void ) const;
    int32 getFontWidth ( void ) const;
    int32 getFontHeight ( void ) const;
    FontStyle getFontStyle ( void ) const;

    const Color& getColor ( void ) const;
    const Coords getPosition ( void ) const;

    uint32 getNewline ( void ) const;
    uint32 getSpacing ( void ) const;

    void setText ( const std::string& text );
    void setColor ( const Color& color );
    void setPosition ( const Coords& coords );
    void setFontSize ( int32 size );
    void setFontStyle ( int32 style, uint8 options = 150 );

    void setSpacing ( uint32 spaces );

    virtual void Update ( void );
    virtual void Draw ( Surface* video_buffer ) const;

    /// Rescale the font with a chosen resizing algorithm
    ///
    /* virtual */ bool resize ( enum ResizeAlgorithm scaling_algorithm );

  private:
    /// Set the type of file we are loading; either a bitmap or a TrueType font
    void setFontType ( enum FontType type );

    std::shared_ptr<IFont> font;
    /// Holds type of font in use; see setFontType
    enum FontType file_type;
};


} // namespace nom

#endif // include guard defined
