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
#include "nomlib/graphics/sprite/SpriteSheet.hpp"

namespace nom {

SpriteSheet::SpriteSheet ( void ) :
  sheet_filename ( "\0" ), sheet_sprites ( 0 ),
  sheet_spacing ( 0 ), sheet_padding ( 0 ),
  sheet_width ( 0 ), sheet_height ( 0 ) {}

SpriteSheet::SpriteSheet ( const std::string& filename )
{
  if ( this->load ( filename ) == false )
  {
NOM_LOG_ERR ( NOM, "Could not load sprite sheet file at: " + filename );
  }
}

SpriteSheet::SpriteSheet  (
                            const std::string& filename,
                            int32 sheet_width, int32 sheet_height,
                            int32 sprite_width, int32 sprite_height,
                            int32 spacing, int32 padding, int32 num_sprites
                          )
{
  // Total number of sprites going from left to right
  int32 sheet_width_dimensions = ceil ( sheet_width / sprite_width );

  // Total number of sprites going from top to bottom
  int32 sheet_height_dimensions = ceil ( sheet_height / sprite_height );

  // Total number of sprites on the sheet
  int32 total_frames = sheet_width_dimensions * sheet_height_dimensions;

  int32 y_offset = 0; // This is used with padding calculation

  int32 rows = 0; // counter used in calculations (left to right)
  int32 cols = 0; // counter used in calculations (top to bottom)

  // Assume whole sheet coordinates extraction if num_sprites argument passed
  // is zero.
  if ( num_sprites == 0 )
  {
    num_sprites = total_frames;
  }

  this->sheet.resize ( num_sprites ); // Make room for storage in our vector

  // Initializing the first sprite frame
  this->sheet[0].x = spacing;
  this->sheet[0].y = padding;
  this->sheet[0].width = sprite_width;
  this->sheet[0].height = sprite_height;

  y_offset = padding;

  // For while the id variable is less than number of sprites, commence..!
  for ( int32 id = 0; id < num_sprites; id++ )
  {
    // If we have reached the end of the row, go back to the front of the next row
    if ( rows+1 > ( sheet_width_dimensions ) )
    {
      rows = 0;
      cols += 1;

      // Apply proper padding offset
      if ( padding )
      {
        y_offset = padding * cols + 1;
      }
      else
      {
        y_offset = padding;
      }
    }

#if defined (NOM_DEBUG_SPRITE_SHEET)
  NOM_DUMP_VAR(cols);
#endif

    // Apply proper spacing calculations
    if ( spacing )
    {
      this->sheet[id].x = ( spacing + rows ) + sprite_width * rows;
    }
    else
    {
      this->sheet[id].x = sprite_width * rows;
    }

    // plus one padding (we count from zero)
    this->sheet[id].y = sprite_height * ( cols ) + y_offset;
    this->sheet[id].width = sprite_width;
    this->sheet[id].height = sprite_height;

#if defined (NOM_DEBUG_SPRITE_SHEET)
  NOM_DUMP_VAR ( id );
  NOM_DUMP_VAR ( this->sheet[id].x );
  NOM_DUMP_VAR ( this->sheet[id].y );
  NOM_DUMP_VAR ( this->sheet[id].width );
  NOM_DUMP_VAR ( this->sheet[id].height );
#endif

    rows++;
    // ...check please!
    // ...The show must go on..
  } // end for loop

  this->sheet_filename = filename;
  this->sheet_sprites = num_sprites;
  this->sheet_spacing = spacing;
  this->sheet_padding = padding;
  this->sheet_width = sheet_width;
  this->sheet_height = sheet_height;
}

SpriteSheet::~SpriteSheet ( void ) {}

const Coords SpriteSheet::dimensions  ( int32 index ) const
{
  return this->sheet.at ( index );
}

int32 SpriteSheet::frames ( void ) const
{
  return this->sheet.size();
}

SpriteSheet::SharedPtr SpriteSheet::clone ( void ) const
{
  return SpriteSheet::SharedPtr ( new SpriteSheet ( *this ) );
}

bool SpriteSheet::save ( const std::string& filename )
{
#if ! defined ( NOM_PLATFORM_WINDOWS )
  nom::JSON::FileWriter fp; // json_spirit wrapper for file output
  json_spirit::Array sheet_data; // Overall container; this is the parent
  json_spirit::Object node; // JSON object record; the child
  std::string nomlib_version = std::to_string ( NOMLIB_VERSION_MAJOR ) + "." + std::to_string ( NOMLIB_VERSION_MINOR ) + "." + std::to_string ( NOMLIB_VERSION_PATCH );

  if ( this->sheet.empty() ) return false;

  for ( int32 id = 0; id < this->sheet.size(); id++ )
  {
    node.push_back ( json_spirit::Pair ( "ID", id ) );
    node.push_back ( json_spirit::Pair ( "x", this->sheet[id].x ) );
    node.push_back ( json_spirit::Pair ( "y", this->sheet[id].y ) );
    node.push_back ( json_spirit::Pair ( "width", this->sheet[id].width ) );
    node.push_back ( json_spirit::Pair ( "height", this->sheet[id].height ) );

    // Mark our current node "complete"; push it for writing!
    sheet_data.push_back ( node );

    node.clear(); // ...ready for the next one!
  }

  // Push out our file meta-data
  node.push_back ( json_spirit::Pair ( "sheet_filename", this->sheet_filename ) );
  node.push_back ( json_spirit::Pair ( "sheet_sprites", this->sheet_sprites ) );
  node.push_back ( json_spirit::Pair ( "sheet_spacing", this->sheet_spacing ) );
  node.push_back ( json_spirit::Pair ( "sheet_padding", this->sheet_padding ) );
  node.push_back ( json_spirit::Pair ( "sheet_width", this->sheet_width ) );
  node.push_back ( json_spirit::Pair ( "sheet_height", this->sheet_height ) );
  node.push_back ( json_spirit::Pair ( "sheet_version", nomlib_version ) );
  node.push_back ( json_spirit::Pair ( "sheet_modified", getCurrentTime() ) );

  sheet_data.push_back ( node );
  node.clear(); // ...ready for the next one!

  if ( fp.save ( filename, sheet_data, nom::JSON::CompactArrays ) == false )
  {
NOM_LOG_ERR ( NOM, "Unable to save the sprite sheet as a JSON file: " + filename );
    return false;
  }

  return true;
#endif // end if NOT Windows platform

#if defined ( NOM_PLATFORM_WINDOWS )
  return false;
#endif
}

bool SpriteSheet::load ( const std::string& filename )
{
#if ! defined ( NOM_PLATFORM_WINDOWS )
  std::ifstream fp; // input file handle
  json_spirit::Object node;
  json_spirit::Value value;
  json_spirit::Array sheet_data;

  // Iterators
  json_spirit::Array::size_type i;
  json_spirit::Object::size_type o;

  // temp buffers
  Coords buffer;

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
    if ( json_spirit::read_stream ( fp, value ) == false )
    {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
      fp.close();
      return false;
    }
    fp.close();
  }
  else
  {
    fp.close();
    return false;
  }

  if ( value.type() != json_spirit::array_type )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
    return false;
  }

  sheet_data = value.get_array();

  for ( i = 0; i != sheet_data.size(); i++ )
  {
    if ( sheet_data[i].type() != json_spirit::obj_type )
    {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
      return false;
    }

    node = sheet_data[i].get_obj();

    for ( o = 0; o != node.size(); o++ )
    {
      const json_spirit::Pair& pair = node[o];
      const std::string& path = pair.name_;
      const json_spirit::Value& value = pair.value_;

      if ( path == "x" && value.type() == json_spirit::int_type )
      {
        buffer.x = value.get_int();
      }
      else if ( path == "y" && value.type() == json_spirit::int_type )
      {
        buffer.y = value.get_int();
      }
      else if ( path == "width" && value.type() == json_spirit::int_type )
      {
        buffer.width = value.get_int();
      }
      else if ( path == "height" && value.type() == json_spirit::int_type )
      {
        buffer.height = value.get_int();
        this->sheet.push_back ( Coords ( buffer.x, buffer.y, buffer.width, buffer.height ) );
      }
    } // end current node loop
  } // end current array node

  return true;
#endif // end if NOT Windows platform

#if defined ( NOM_PLATFORM_WINDOWS )
  return false;
#endif
}


} // namespace nom
