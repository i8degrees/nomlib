/******************************************************************************

    Abstract Font Interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_IFONT_HPP
#define NOMLIB_IFONT_HPP

#include <iostream>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/sys/Logger.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Transformable.hpp"

namespace nom {

/// Text effect styling
enum FontStyle
{
  Regular = 0,
  Bold = 1,
  Italic = 2,
  Underlined = 3,
  Faded = 4
};

class IFont: public Transformable // "is-a" relationship
{
  public:
    IFont ( void ) {}

    virtual ~IFont ( void ) {}

    virtual bool Load ( const std::string& filename, const Color& colorkey,
                        int32 font_size = 12, bool use_cache = 0
                      ) = 0;

    virtual const std::string& getText ( void ) const = 0;
    virtual int32 getFontWidth ( void ) const = 0;
    virtual int32 getFontHeight ( void ) const = 0;
    virtual FontStyle getFontStyle ( void ) const = 0;

    virtual void setText ( const std::string& text ) = 0;
    virtual void setFontSize ( int32 point_size ) = 0;
    virtual void setFontStyle ( uint8 style, uint8 options = 150 ) = 0;

    virtual void Update ( void ) = 0;
    virtual void Draw ( void* video_buffer ) = 0;
};


} // namespace nom

#endif // include guard defined
