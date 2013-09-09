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
#include "nomlib/graphics/SpriteSheet.hpp"

namespace nom {

SpriteSheet::SpriteSheet ( void ) {}

SpriteSheet::SpriteSheet  (
                            int32 sheet_width, int32 sheet_height,
                            int32 sprite_width, int32 sprite_height,
                            int32 spacing, int32 padding, int32 num_sprites
                          )
{
  float sheet_width_dimensions = floor ( sheet_width / sprite_width ); // 2082 / 64 = 32.5
  float sheet_height_dimensions = floor ( sheet_height / sprite_height ); // 262 / 64 = 4.0
  int32 total_frames = sheet_width_dimensions * sheet_height_dimensions;
  int32 x_offset = 0;
  int32 y_offset = 0;
  // spacing: ( id * sprite_width ) + ( spacing * id + 1 )
  // y_offset += padding

  // The number sprite going from left to right
  int32 rows = 0;

  // The number sprite going from top to bottom
  int32 cols = 0;

  // The row increment value, counted once every processed row.
  int32 row_increment = 0;

  // Assume whole sheet coordinates extraction if num_sprites argument passed
  // is zero.
  if ( num_sprites == 0 )
  {
    num_sprites = total_frames;
  }
  else
  {
    num_sprites = num_sprites + 1; // plus one padding (we count from zero)
  }

  // For while the id variable is less than number of sprites, commence..!
  for ( int32 id = 0; id < num_sprites; id++ )
  {
    // First sprite always starts at 0, 0 -- so let us initialize it.
    if ( id == 0 )
    {
      this->sheet[id].x = 0;
      this->sheet[id].y = 0;
      this->sheet[id].width = 0;
      this->sheet[id].height = 0;
    }

    // If we have reached the end of the row, go back to the front of the next row
    if ( rows > ( sheet_width_dimensions - 1 ) )
    {
      rows = 0;
      cols += 1;
      //y_offset += padding;
      //NOM_DUMP_VAR(padding);
    }

#if defined (NOM_DEBUG_SPRITE_SHEET)
  NOM_DUMP_VAR(cols);
#endif

    //x_offset += ( id * sprite_width ) + ( spacing * id + 1 );
    //NOM_DUMP_VAR(x_offset);

    this->sheet[id].x = sprite_width * rows;
    // plus one padding (we count from zero)
    this->sheet[id].y = sprite_height * ( cols + 1 );
    this->sheet[id].width = sprite_width;
    this->sheet[id].height = sprite_height;

#if defined (NOM_DEBUG_SPRITE_SHEET)
  NOM_DUMP_VAR ( this->sheet[id].x );
  NOM_DUMP_VAR ( this->sheet[id].y );
  NOM_DUMP_VAR ( this->sheet[id].width );
  NOM_DUMP_VAR ( this->sheet[id].height );
  NOM_DUMP_VAR ( row_increment );
#endif

    rows++;
    row_increment++;
    // ...check please!
    // ...The show must go on..

#if defined (NOM_DEBUG_SPRITE_SHEET)
  NOM_DUMP_VAR ( rows );
  NOM_DUMP_VAR ( row_increment );
  NOM_DUMP_VAR ( id );
#endif

  } // end for loop

  for ( int32 id = 0; id < this->sheet.size(); id++ )
  {
    if ( padding )
    {
      // TODO
    }
  }
}

SpriteSheet::~SpriteSheet ( void ) {}

const Coords SpriteSheet::dimensions  ( int32 index ) const
{
  auto itr = this->sheet.find ( index );

  if ( itr == this->sheet.end() )
  {
    return Coords::null;
  }

  return itr->second;
}

bool SpriteSheet::save ( const std::string& filename )
{
  return true;
}

bool SpriteSheet::load ( const std::string& filename )
{
  return true;
}

/*
bool SpriteSheet::rebuild ( void )
{
  return true;
}
*/


} // namespace nom
