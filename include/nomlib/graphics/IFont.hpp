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
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/IDrawable.hpp"

namespace nom {

class IFont:
              public IDrawable
{
  public:
    /// Text effect styling
    enum FontStyle
    {
      Regular = 0,
      Bold = 1,
      Italic = 2,
      Underlined = 3,
      Faded = 4
    };

    enum TextAlignment
    {
      TopLeft = 0,
      TopCenter,
      TopRight,
      MiddleLeft,
      MiddleCenter,
      MiddleRight,
      BottomLeft,
      BottomCenter,
      BottomRight
    };

    /// Text rendering qualities
    enum RenderStyle
    {
      NotImplemented = 0,
      Solid,
      Shaded,
      Blended
    };

    typedef IFont* RawPtr;
    typedef std::unique_ptr<IFont> UniquePtr;
    typedef std::shared_ptr<IFont> SharedPtr;

    IFont ( void ) {}
    virtual ~IFont ( void ) {}

    virtual IFont::SharedPtr clone ( void ) const = 0;

    virtual bool load (
                        const std::string& filename, const Color4u& colorkey,
                        bool use_cache = false
                      ) = 0;

    virtual const std::string& getText ( void ) const = 0;
    virtual int32 getFontWidth ( void ) const = 0;
    virtual int32 getFontHeight ( void ) const = 0;
    virtual IFont::FontStyle getFontStyle ( void ) const = 0;
    virtual const Coords& getPosition ( void ) const = 0;
    virtual const Color4u& getColor ( void ) const = 0;
    virtual uint32 getNewline ( void ) const = 0;
    virtual uint32 getSpacing ( void ) const = 0;
    virtual IFont::TextAlignment getTextJustification ( void ) const = 0;


    virtual void setText ( const std::string& text ) = 0;

    /// Set a new font point (pixel) size.
    ///
    /// Optional interface
    virtual void setFontSize ( int32 point_size )
    {
NOM_LOG_ERR ( NOM, "Method not implemented." );
    }

    virtual void setFontStyle ( int32 style, uint8 options = 150 ) = 0;
    virtual void setColor ( const Color4u& color ) = 0;
    virtual void setPosition ( const Coords& coords ) = 0;
    virtual void setSpacing ( uint32 spaces ) = 0;
    virtual void setTextJustification ( IFont::TextAlignment alignment ) = 0;

    virtual IFont::RenderStyle getRenderingStyle ( void ) const
    {
NOM_LOG_ERR ( NOM, "Method not implemented." );
      return IFont::RenderStyle::Solid;
    }

    virtual void setRenderingStyle ( IFont::RenderStyle )
    {
NOM_LOG_ERR ( NOM, "Method not implemented." );
    }

    /// Rescale the font with a chosen resizing algorithm
    ///
    /// Optional interface with a return of false when the deriving class has
    /// chosen not to re-implement this method.
    /// \todo SDL2 port
    virtual bool resize ( enum ResizeAlgorithm scaling_algorithm )
    {
NOM_LOG_ERR ( NOM, "Method not implemented." );
      return false;
    }
};

} // namespace nom

#endif // include guard defined
