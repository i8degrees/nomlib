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
#ifndef NOMLIB_SDL_CURSOR_HEADERS
#define NOMLIB_SDL_CURSOR_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/graphics/Sprite.hpp"

namespace nom {
  namespace ui {

/// \brief Helper class for loading, positioning and keeping track of a cursor
/// controlled by a mouse and / or keyboard.
///
/// \TODO inherit from Sprite
class Cursor
{
  public:
    Cursor ( void );
    Cursor ( int32 x, int32 y, int32 width, int32 height );
    virtual ~Cursor ( void );

    int32 getX ( void ) const;
    int32 getY ( void ) const;

    /// Set the size of the cursor bitmap.
    void setSize ( int32 width, int32 height );

    /// Set the X, Y coordinates of the cursor bitmap.
    void setPosition ( int32 x, int32 y );

    void move ( int32 x, int32 y );

    int32 getSheetID ( void );

    void setSheetID ( int32 sheet_id = -1 );
    void setSheetDimensions ( int32 sheet_width, int32 sheet_height, int32 spacing, int32 padding );

    int32 getState ( void );
    void setState ( int32 state );

    //bool isCursorLocked ( void );
    //void lockCursor ( bool toggle );

    /// Move the cursor up.
    ///
    /// Returns the Y coordinate position of the cursor after it has been moved.
    ///
    /// \todo Rename me to up?
    virtual int32 moveCursorUp ( void );

    /// Move the cursor down.
    ///
    /// Returns the Y coordinate position of the cursor after it has been moved.
    ///
    /// \todo Rename me to down?
    virtual int32 moveCursorDown ( void );

    /// Move the cursor to the left.
    ///
    /// Returns the X coordinate position of the cursor after it has been moved.
    ///
    /// \todo Rename me to left?
    virtual int32 moveCursorLeft ( void );

    /// Move the cursor to the right.
    ///
    /// Returns the X coordinate position of the cursor after it has been moved.
    ///
    /// \todo Rename me to right?
    virtual int32 moveCursorRight ( void );

    bool load( const std::string& filename, const Color& colorkey, bool use_cache );

    void update ( void );
    void draw ( void* video_buffer );

    /// Rescale the font with a chosen resizing algorithm
    bool resize ( enum ResizeAlgorithm scaling_algorithm );

  protected:
    void initialize ( int32 x, int32 y, int32 width, int32 height );
    /// Bitmap for our interface cursor
    Sprite cursor;
};


  } // namespace ui
} // namespace nom

#endif // NOMLIB_SDL_CURSOR_HEADERS defined
