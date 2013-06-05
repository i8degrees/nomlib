/******************************************************************************
      Font.h

    Base font class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_FONT_HEADERS
#define NOMLIB_FONT_HEADERS

#include <iostream>
#include <string>

#include "gfx.h"

class Font
{
  public:

    Font ( void );
    ~Font ( void );

    virtual signed int getX ( void );
    virtual signed int getY ( void );
    virtual nom::Coords getXY ( void );
    virtual void setX ( signed int x_ );
    virtual void setY ( signed int y_ );
    virtual void setXY ( signed int x_, signed int y_ );

    virtual std::string getText ( void );
    virtual void setText ( std::string text );

    virtual nom::Color getTextColor ( void );
    virtual void setTextColor ( const nom::Color &color );

  protected:
    nom::Color text_color; // the color of the font
    std::string text_buffer; // holds contents of text as a string
    nom::Coords coords; // X, Y, width & height blitting coordinates
};

#endif // NOMLIB_FONT_HEADERS defined
