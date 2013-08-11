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
class Cursor
{
  public:
    Cursor ( void );
    Cursor ( unsigned int x, unsigned int y, unsigned int width, unsigned int height );
    virtual ~Cursor ( void );

    int32 getX ( void );
    int32 getY ( void );
    void setPosition ( unsigned int x = 0, unsigned int y = 0 );
    void move ( unsigned int x = 0, unsigned int y = 0 );

    signed int getSheetID ( void );
    void setSheetID ( signed int sheet_id = -1 );
    void setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding );

    unsigned int getState ( void );
    void setState ( unsigned int state );

    //bool isCursorLocked ( void );
    //void lockCursor ( bool toggle );

    Coords getCursorPos ( unsigned int x, unsigned int y );
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();

    void load( std::string filename, Color colorkey, bool use_cache );
    void Update ( void );
    void Draw ( void* video_buffer );

  private:
    Sprite cursor; // interface cursor

    // cursor XY coords mapping
    std::vector<Coords> coords_map;

    //    coords_map schema:
    //
    //  [index-pos].[ x, y, width, height ]
    //
};


  } // namespace ui
} // namespace nom

#endif // NOMLIB_SDL_CURSOR_HEADERS defined
