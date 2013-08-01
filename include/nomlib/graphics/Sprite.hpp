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
#ifndef NOMLIB_SPRITE_HEADERS
#define NOMLIB_SPRITE_HEADERS

#include <iostream>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/system/Logger.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/Canvas.hpp"
#include "nomlib/math/Transformable.hpp"

namespace nom {

class Sprite: public IDrawable,     //  "is a" inheritance
              public Transformable  //  "has a" inheritance
{
  public:
    Sprite ( void );
    Sprite ( unsigned int width, unsigned int height );
    virtual ~Sprite ( void );

    unsigned int getState ( void );
    void setState ( unsigned int state );

    signed int getSheetID ( void );
    void setSheetID ( signed int id );
    void setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding );

    bool Load ( std::string filename, Color colorkey, bool use_cache = 0,
                unsigned int flags = SDL_SRCCOLORKEY | SDL_RLEACCEL
              );

    void Update ( void );
    void Draw ( void* video_buffer ) const;

  private:
    Canvas sprite_buffer;
    Coords offsets;

    unsigned int state; /// alive, dying, dead, ...

    struct {
      signed int id; /// maps a specific sprite within sheet
      unsigned int sprite_width; /// width of sprite in sheet
      unsigned int sprite_height; /// height of sprite in sheet
      unsigned int width; /// width of sprite sheet
      unsigned int height; /// height of sprite sheet
      unsigned int spacing; /// applied between each sheet tile
      unsigned int padding; /// applied on all four sides of sheet tile
    } sheet;
};


} // namespace nom

#endif // NOMLIB_SPRITE_HEADERS defined
