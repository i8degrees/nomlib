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

namespace nom {

class IFont
{
  public:
    typedef IFont* RawPtr;
    typedef std::shared_ptr<IFont> SharedPtr;

    enum FileType
    {
      NotDefined = 0,
      BitmapFont = 1,
      TrueTypeFont = 2,
      UserDefined = 3,
    };

    IFont ( void ) {}
    virtual ~IFont ( void ) {}
    virtual IFont::SharedPtr clone ( void ) const = 0;
/*
    virtual bool load (
                        const std::string& filename, const Color4u& colorkey,
                        bool use_cache = false
                      ) = 0;
*/
    // REFACTOR:
    //virtual int32 getFontWidth ( void ) const = 0;
    // REFACTOR:
    //virtual int32 getFontHeight ( void ) const = 0;

    //virtual IFont::FontStyle getFontStyle ( void ) const = 0;
    //virtual const Coords& getPosition ( void ) const = 0;
    //virtual const Color4u& getColor ( void ) const = 0;
    virtual uint newline ( void ) const = 0;
    virtual uint spacing ( void ) const = 0;
    //virtual IFont::TextAlignment getTextJustification ( void ) const = 0;

    /// Set a new font point (pixel) size.
    ///
    /// Optional interface
/*
    virtual void setFontSize ( int32 point_size )
    {
NOM_LOG_ERR ( NOM, "Method not implemented." );
    }
*/

    //virtual void setFontStyle ( int32 style, uint8 options = 150 ) = 0;
    //virtual void setColor ( const Color4u& color ) = 0;
    //virtual void setPosition ( const Coords& coords ) = 0;
    virtual void set_spacing ( uint spaces ) = 0;
    //virtual void setTextJustification ( IFont::TextAlignment alignment ) = 0;
/*
    virtual IFont::RenderStyle getRenderingStyle ( void ) const
    {
NOM_LOG_ERR ( NOM, "Method not implemented." );
      return IFont::RenderStyle::Solid;
    }
*/
/*
    virtual void setRenderingStyle ( IFont::RenderStyle )
    {
NOM_LOG_ERR ( NOM, "Method not implemented." );
    }
*/
    /// Rescale the font with a chosen resizing algorithm
    ///
    /// Optional interface with a return of false when the deriving class has
    /// chosen not to re-implement this method.
    /// \todo SDL2 port
/*
    virtual bool resize ( enum Texture::ResizeAlgorithm scaling_algorithm )
    {
NOM_LOG_ERR ( NOM, "Method not implemented." );
      return false;
    }
*/
    virtual const IntRect& glyph ( uint32 character ) = 0;
    virtual SDL_SURFACE::RawPtr image ( void ) const = 0;
    virtual enum IFont::FileType type ( void ) const = 0;
    /// \todo This method should be part of the *required* implementation
    virtual bool build ( void ) { return false; }/*= 0*/;
};

} // namespace nom

#endif // include guard defined
