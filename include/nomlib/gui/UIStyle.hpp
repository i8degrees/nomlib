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
#ifndef NOMLIB_GUI_UI_STYLE_HPP
#define NOMLIB_GUI_UI_STYLE_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"

namespace nom {

/// \brief Customizable widget properties
class UIStyle
{
  public:
    typedef UIStyle self_type;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor.
    UIStyle( void );

    /// \brief Destructor.
    ~UIStyle( void );

    int font_size( void ) const;

    const Color4i& font_color( void ) const;

    uint32 font_style( void ) const;

    void set_font_size( int point_size );
    void set_font_color( const Color4i& color );
    void set_font_style( uint32 style );

  private:
    /// \brief The font's height (font point size).
    int font_size_;

    /// \brief The color used for the font.
    Color4i font_color_;

    /// \brief The rendering style used for the font.
    uint32 font_style_;
};

} // namespace nom

#endif // include guard defined
