/******************************************************************************

    Unified text output interface; designed for wrapping Bitmap & TrueType
  font objects to co-exist under one roof.

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_TEXT_HPP
#define NOMLIB_TEXT_HPP

#include <iostream>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/sys/Logger.hpp"
#include "nomlib/sys/OSXFS.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/gfx/Canvas.hpp"
#include "nomlib/gfx/IFont.hpp"
#include "nomlib/gfx/BitmapFont.hpp"
#include "nomlib/gfx/TrueTypeFont.hpp"
#include "nomlib/gfx/IDrawable.hpp"

namespace nom {

enum FontType
{
  Unknown = 0,
  Bitmap = 1,
  TrueType = 2
};

class Text/*: public IDrawable*/
{
  public:
    Text ( void );
    virtual ~Text ( void );

    bool Load ( const std::string& filename, int32 font_size = 12 );

    FontType getFontType ( void );

    const std::string& getText ( void ) const;
    int32 getFontWidth ( void ) const;
    int32 getFontHeight ( void ) const;
    FontStyle getFontStyle ( void ) const;

    const Coords getPosition ( void ) const;
    const Color getColor ( void ) const;

    void setText ( const std::string& text );
    void setColor ( const Color& color );
    void setPosition ( const Coords& coords );
    void setFontSize ( int32 size );
    void setFontStyle ( uint8 style, uint8 options = 150 );

    virtual void Update ( void );
    virtual void Draw ( void* video_buffer ) /*const*/;

  private:
    /// Set the type of file we are loading; either a bitmap or a TrueType font
    void setFontType ( enum FontType type );

    std::shared_ptr<IFont> font;
    /// Holds type of font in use; see setFontType
    enum FontType file_type;
};


} // namespace nom

#endif // include guard defined
