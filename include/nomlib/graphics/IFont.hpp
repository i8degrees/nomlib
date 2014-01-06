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
#ifndef NOMLIB_SDL2_IFONT_HPP
#define NOMLIB_SDL2_IFONT_HPP

#include <iostream>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/Glyph.hpp"
#include "nomlib/graphics/Texture.hpp"

namespace nom {

class IFont
{
  public:
    typedef IFont* RawPtr;
    typedef std::shared_ptr<IFont> SharedPtr;

    enum FontType
    {
      NotDefined = 0,
      BitmapFont,
      TrueTypeFont,
      UserDefined
    };

    IFont ( void )
    {
      //NOM_LOG_TRACE(NOM);
    }

    virtual ~IFont ( void )
    {
      //NOM_LOG_TRACE(NOM);
    }

    virtual IFont::SharedPtr clone ( void ) const = 0;

    virtual SDL_SURFACE::RawPtr image ( void ) const = 0;
    //virtual const Texture& texture ( uint32 = 0 ) /*const*/ = 0;
    virtual enum IFont::FontType type ( void ) const = 0;

    virtual const Glyph& glyph ( uint32, uint32 = 0 ) const = 0;
    virtual uint newline ( void ) const = 0;
    virtual uint spacing ( void ) const = 0;

    virtual void set_spacing ( uint ) = 0;
    virtual bool build ( uint32 = 0 ) = 0;
};

} // namespace nom

#endif // include guard defined
