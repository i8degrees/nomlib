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
#ifndef NOMLIB_SPRITE_SHEET_HPP
#define NOMLIB_SPRITE_SHEET_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include "math.h"

#include "json_spirit_writer_template.h"
#include "json_spirit_reader_template.h"

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"

namespace nom {

#define NOM_DEBUG_SPRITE_SHEET

/// \brief SpriteSheet data container class
class SpriteSheet
{
  public:
    /// Default, do nothing much constructor.
    SpriteSheet ( void );

    /// Construct a sprite sheet from a given list of arguments. The total tile
    /// count is used if the num_sprites argument is not initialized, otherwise
    /// user-defined number of tiles, with a padding of plus one to the number
    /// given.
    ///
    /// Padding is applied on all four sides. Spacing is applied between each
    /// tile.
    ///
    /// \TODO Spacing and padding calculations
    SpriteSheet (
                  int32 sheet_width, int32 sheet_height,
                  int32 sprite_width, int32 sprite_height,
                  int32 spacing = 0, int32 padding = 0, int32 num_sprites = 0
                );

    /// Destructor.
    ~SpriteSheet ( void );

    /// Get the calculations made for a particular ID number.
    const Coords dimensions ( int32 index ) const;

    /// - Sprite Sheet JSON files:
    ///
    ///   * RECT coordinates
    ///   * spacing, padding, sheet_width & sheet_height, number of sprites,
    /// etc.
    ///   * timestamp
    ///   * perhaps scaling / rotation info?
    ///   * ...
    ///
    /// \TODO
    bool save ( const std::string& filename );
    bool load ( const std::string& filename );

    //bool rebuild ( void );
    //void update ( void );
    //void draw ( void );

  private:
    /// ...
    std::map<int32, Coords> sheet;
/*
    /// ...
    int32 num_sprites;

    /// ...
    int32 sprite_width;

    /// ...
    int32 sprite_height;

    /// ...
    int32 spacing;

    /// ...
    int32 padding;

    /// ...
    int32 sheet_width;

    /// ...
    int32 sheet_height;
*/
};


} // namespace nom

#endif // include guard defined

// Example of 4X4 Sheet
//   ________________
//  | 0 | 1 | 2 | 3 |
//  |===============|
//  | 0 | 1 | 2 | 3 |
//  |===============|
//  | 0 | 1 | 2 | 3 |
//  |===============|
//  | 0 | 1 | 2 | 3 |
//  |---------------|
