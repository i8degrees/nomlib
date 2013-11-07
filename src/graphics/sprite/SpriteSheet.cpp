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

const int NOM_SPRITE_SHEET_MAJOR_VERSION = 0;
const int NOM_SPRITE_SHEET_MINOR_VERSION = 2;
const int NOM_SPRITE_SHEET_PATCH_VERSION = 0;

SpriteSheet::SpriteSheet ( void ) :
    sheet_filename_ ( "\0" ), sheet_sprites ( 0 ),
    sheet_spacing ( 0 ), sheet_padding ( 0 ),
    sheet_width ( 0 ), sheet_height ( 0 )
{}

SpriteSheet::~SpriteSheet ( void ) {}

SpriteSheet::SpriteSheet ( const std::string& filename ):
    sheet_filename_ ( filename ), sheet_sprites ( 0 ),
    sheet_spacing ( 0 ), sheet_padding ( 0 ),
    sheet_width ( 0 ), sheet_height ( 0 )
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
                          ):
  sheet_filename_ ( filename ), sheet_sprites ( num_sprites ),
  sheet_spacing ( spacing ), sheet_padding ( padding ),
  sheet_width ( sheet_width ), sheet_height ( sheet_height )
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

  this->sheet_filename_ = filename;
NOM_ASSERT ( this->sheet.size() == num_sprites );
  this->sheet_sprites = this->sheet.size();
  this->sheet_spacing = spacing;
  this->sheet_padding = padding;
NOM_ASSERT ( sprite_width * num_sprites == sheet_width );
  this->sheet_width = sheet_width;
NOM_ASSERT ( sprite_height * num_sprites == sheet_height );
  this->sheet_height = sheet_height;
}

const Coords& SpriteSheet::dimensions  ( int32 index ) const
{
  return this->sheet.at ( index );
}

int32 SpriteSheet::frames ( void ) const
{
  return this->sheet.size();
}

const std::string& SpriteSheet::sheet_filename ( void ) const
{
  return this->sheet_filename_;
}

SpriteSheet::SharedPtr SpriteSheet::clone ( void ) const
{
  return SpriteSheet::SharedPtr ( new SpriteSheet ( *this ) );
}

bool SpriteSheet::save ( const std::string& filename )
{
  Json::Value node; // Starting JSON node
  nom::JSON::FileWriter writer;

  std::string sprite_sheet_version = std::to_string ( NOM_SPRITE_SHEET_MAJOR_VERSION ) + "." + std::to_string ( NOM_SPRITE_SHEET_MINOR_VERSION ) + "." + std::to_string ( NOM_SPRITE_SHEET_PATCH_VERSION );

  if ( this->sheet.empty() ) return false;

  auto id = 0;
  for ( id = 0; id < this->sheet.size(); ++id )
  {
    node[id]["id"] = id;
    node[id]["x"] = this->sheet[id].x;
    node[id]["y"] = this->sheet[id].y;
    node[id]["width"] = this->sheet[id].width;
    node[id]["height"] = this->sheet[id].height;
  }

  // Push out our file meta-data last
  node[id]["sheet_filename"] = this->sheet_filename_;
  node[id]["sheet_sprites"] = this->sheet_sprites;
  node[id]["sheet_spacing"] = this->sheet_spacing;
  node[id]["sheet_padding"] = this->sheet_padding;
  node[id]["sheet_width"] = this->sheet_width;
  node[id]["sheet_height"] = this->sheet_height;
  node[id]["sheet_version"] = sprite_sheet_version;
  node[id]["sheet_modified"] = getCurrentTime();

  if ( writer.save ( filename, node ) == false )
  {
NOM_LOG_ERR ( NOM, "Unable to save file: " + filename );
    return false;
  }

  return true;
}

bool SpriteSheet::load ( const std::string& filename )
{
  nom::JSON::FileReader parser;
  Json::Value root; // JSON root node

  // Temporary holding buffers to hold data until we are ready to commit
  Coords buffer;
  std::vector<Coords> buffer_sheet;

  if ( parser.load ( filename, root ) == false )
  {
NOM_LOG_ERR ( NOM, "Unable to open JSON file at: " + filename );
    return false;
  }

  // Layout sanity check
  if ( root.type() != Json::ValueType::arrayValue )
  {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (does not begin as a JSON array)." );
    return false;
  }

  // Populate our sheet vector while skipping the last JSON object;
  // this should always be file mete-data!
  for ( auto idx = 0; idx < root.size() - 1; ++idx )
  {
    // Layout sanity check
    if ( root[idx].type() != Json::ValueType::objectValue )
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (does not contain a JSON object)." );
      return false;
    }

    if ( root[idx]["x"].type() == Json::ValueType::intValue )
    {
      buffer.x = root[idx]["x"].asInt();
    }
    else
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (expected type: signed integer)." );
      return false;
    }

    if ( root[idx]["y"].type() == Json::ValueType::intValue )
    {
      buffer.y = root[idx]["y"].asInt();
    }
    else
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (expected type: signed integer)." );
      return false;
    }

    if ( root[idx]["width"].type() == Json::ValueType::intValue )
    {
      buffer.w = root[idx]["width"].asInt();
    }
    else
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (expected type: signed integer)." );
      return false;
    }

    if ( root[idx]["height"].type() == Json::ValueType::intValue )
    {
      buffer.h = root[idx]["height"].asInt();
    }
    else
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (expected type: signed integer)." );
      return false;
    }

    // Commit contents to our buffer if all goes well
    buffer_sheet.push_back ( buffer );
  }

  // If we have gotten this far, we assume all is well, so let's commit the data
  // as valid sprite sheet data!
  this->sheet.clear();
  this->sheet = buffer_sheet;

  // Populate our other instance variables with the last JSON object;
  // this should always be file mete-data!
  for ( auto idx = root.size() - 1; idx != root.size(); ++idx )
  {
    if ( root[idx]["sheet_filename"].type() == Json::ValueType::stringValue )
    {
      this->sheet_filename_ = root[idx]["sheet_filename"].asString();
    }
    else
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (expected type: string value)." );
      return false;
    }

    if ( root[idx]["sheet_sprites"].type() == Json::ValueType::intValue )
    {
NOM_ASSERT ( this->sheet.size() == root[idx]["sheet_sprites"].asInt() );
      this->sheet_sprites = root[idx]["sheet_sprites"].asInt();
    }
    else
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (expected type: integer value)." );
      return false;
    }

    if ( root[idx]["sheet_spacing"].type() == Json::ValueType::intValue )
    {
      this->sheet_spacing = root[idx]["sheet_spacing"].asInt();
    }
    else
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (expected type: integer value)." );
      return false;
    }

    if ( root[idx]["sheet_padding"].type() == Json::ValueType::intValue )
    {
      this->sheet_padding = root[idx]["sheet_padding"].asInt();
    }
    else
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (expected type: integer value)." );
      return false;
    }

    if ( root[idx]["sheet_width"].type() == Json::ValueType::intValue )
    {
NOM_ASSERT ( this->sheet[0].width * this->sheet.size() == root[idx]["sheet_width"].asInt() );
      this->sheet_width = root[idx]["sheet_width"].asInt();
    }
    else
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (expected type: integer value)." );
      return false;
    }

    if ( root[idx]["sheet_height"].type() == Json::ValueType::intValue )
    {
NOM_ASSERT ( this->sheet[0].height * this->sheet.size() == root[idx]["sheet_height"].asInt() );
      this->sheet_height = root[idx]["sheet_height"].asInt();
    }
    else
    {
NOM_LOG_ERR ( NOM, "Unable to parse JSON (expected type: integer value)." );
      return false;
    }
  }

#if defined ( NOM_DEBUG_SPRITE_SHEET_JSON_LOAD )
  this->dump();
#endif

  return true;
}

void SpriteSheet::dump ( void ) const
{
  // Sheet vector state
  for ( auto itr = this->sheet.begin(); itr != this->sheet.end(); ++itr )
  {
NOM_DUMP_VAR(itr->x);
NOM_DUMP_VAR(itr->y);
NOM_DUMP_VAR(itr->width);
NOM_DUMP_VAR(itr->height);
  }

  // Sheet's meta-data
NOM_DUMP_VAR(this->sheet_filename_);
NOM_DUMP_VAR(this->sheet_sprites);
NOM_DUMP_VAR(this->sheet_spacing);
NOM_DUMP_VAR(this->sheet_padding);
NOM_DUMP_VAR(this->sheet_width);
NOM_DUMP_VAR(this->sheet_height);
}


} // namespace nom
