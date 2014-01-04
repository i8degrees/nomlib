/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Portions Copyright (c) 2004-2013 Lazy Foo' Productions [1]
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

1. http://lazyfoo.net/SDL_tutorials/lesson30/index.php

******************************************************************************/
#include "nomlib/graphics/BitmapFont.hpp"

namespace nom {

BitmapFont::BitmapFont ( void ) :
  sheet_width_ { 16 },
  sheet_height_ { 16 },
  newline_ { 0 },
  spacing_ { 0 },
  type_ { IFont::FileType::BitmapFont }
{
  NOM_LOG_TRACE ( NOM );
}

BitmapFont::~BitmapFont ( void )
{
  NOM_LOG_TRACE ( NOM );
}

BitmapFont::BitmapFont ( const BitmapFont& copy )
{
  this->sheet_width_ = copy.sheet_width();
  this->sheet_height_ = copy.sheet_height();
  this->bitmap_font_ = copy.bitmap_font_;
  this->glyphs_ = copy.glyphs();
  this->newline_ = copy.newline_;
  this->spacing_ = copy.spacing_;
  this->type_ = copy.type_;
}

IFont::SharedPtr BitmapFont::clone ( void ) const
{
  return BitmapFont::SharedPtr ( new BitmapFont ( *this ) );
}

bool BitmapFont::valid ( void ) const
{
  if ( this->bitmap_font_.valid() == true ) return true;

  return false;
}

enum IFont::FileType BitmapFont::type ( void ) const
{
  return this->type_;
}

SDL_SURFACE::RawPtr BitmapFont::image ( void ) const
{
  return this->bitmap_font_.image();
}

uint BitmapFont::spacing ( void ) const
{
  return this->spacing_;
}

const Glyph::GlyphMap& BitmapFont::glyphs ( void ) const
{
  return this->glyphs_;
}

const IntRect& BitmapFont::glyph ( uint32 character )
{
  //uint8 ascii = 0;
  //std::istringstream i ( glyph );
  //i >> ascii;
  return this->glyphs_[ character ].bounds;
}

void BitmapFont::set_spacing ( uint spaces )
{
  this->spacing_ = spaces;
}

uint BitmapFont::newline ( void ) const
{
  return this->newline_;
}

void BitmapFont::set_newline ( uint newline )
{
  this->newline_ = newline;
}

bool BitmapFont::load ( const std::string& filename, const Color4u& colorkey,
                        bool use_cache
                      )
{
  // I don't understand why RGBA8888 is necessary here, but it is the only pixel
  // format that I have found that works when we are initializing a
  // nom::Texture (SDL_Texture) from a nom::Image (SDL_Surface).
  if ( this->bitmap_font_.load ( filename, SDL_PIXELFORMAT_RGBA8888 ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not load bitmap font image file: " + filename );
    return false;
  }

  this->bitmap_font_.set_colorkey ( colorkey, true );

  // Attempt to build font metrics
  if ( this->build() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not build bitmap font metrics" );
    return false;
  }

  return true;
}

bool BitmapFont::build ( void )
{
  uint32 tile_width = 0;
  uint32 tile_height = 0;
  uint32 top = 0;
  uint32 baseA = 0;
  uint32 currentChar = 0;
  uint32 background_color = 0;

NOM_ASSERT ( this->bitmap_font_.valid() );

  if ( this->bitmap_font_.lock() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not lock surface." );
    return false;
  }

  background_color = RGBA (
                            this->bitmap_font_.colorkey(),
                            this->bitmap_font_.pixel_format()
                          );

  tile_width = this->bitmap_font_.width() / this->sheet_width();
  tile_height = this->bitmap_font_.height() / this->sheet_height();
  top = tile_height;
  baseA = tile_height;

  for ( int32 rows = 0; rows < this->sheet_width(); rows++ )
  {
    for ( int32 cols = 0; cols < this->sheet_height(); cols++ )
    {
      // Set character offsets
      this->glyphs_[ currentChar ].bounds.x = tile_width * cols;
      this->glyphs_[ currentChar ].bounds.y = tile_height * rows;
      this->glyphs_[ currentChar ].bounds.w = tile_width;
      this->glyphs_[ currentChar ].bounds.h = tile_height;


      //Find Left Side; go through pixel columns
      for ( uint32 pCol = 0; pCol < tile_width; pCol++ )
      {
        //Go through pixel rows
        for ( uint32 pRow = 0; pRow < tile_height; pRow++ )
        {
          //Get the pixel offsets
          uint32 pX = ( tile_width * cols ) + pCol;
          uint32 pY = ( tile_height * rows ) + pRow;

          //If a non colorkey pixel is found
          if( this->bitmap_font_.pixel ( pX, pY ) != background_color )
          {
            //Set the x offset
            this->glyphs_[ currentChar ].bounds.x = pX;

            //Break the loops
            pCol = tile_width;
            pRow = tile_height;
          }
        }
      }

      //Find Right Side; go through pixel columns
      for ( int32 pCol_w = tile_width - 1; pCol_w >= 0; pCol_w-- )
      {
        //Go through pixel rows
        for ( uint32 pRow_w = 0; pRow_w < tile_height; pRow_w++ )
        {
          //Get the pixel offsets
          uint32 pX = ( tile_width * cols ) + pCol_w;
          uint32 pY = ( tile_height * rows ) + pRow_w;

          //If a non colorkey pixel is found
          if ( this->bitmap_font_.pixel ( pX, pY ) != background_color )
          {
            //Set the width
            this->glyphs_[ currentChar ].bounds.w = ( pX - this->glyphs_[ currentChar ].bounds.x ) + 1;

            //Break the loops
            pCol_w = -1;
            pRow_w = tile_height;
          }
        }
      }

      //Find Top; go through pixel rows
      for ( uint32 pRow = 0; pRow < tile_height; pRow++ )
      {
        //Go through pixel columns
        for ( uint32 pCol = 0; pCol < tile_width; pCol++ )
        {
          //Get the pixel offsets
          uint32 pX = ( tile_width * cols ) + pCol;
          uint32 pY = ( tile_height * rows ) + pRow;

          // If a non colorkey pixel is found
          if( this->bitmap_font_.pixel ( pX, pY ) != background_color )
          {
            // If new top is found
            if ( pRow < top )
            {
              top = pRow;
            }

            // Break the loops
            pCol = tile_width;
            pRow = tile_height;
          }
        }
      }

      // Calculate the baseline of 'A' so that we can use this in our
      // newline offset calculation
      if ( currentChar == 'A' )
      {
        // Go through pixel rows
        for ( int32 pRow = tile_height - 1; pRow >= 0; pRow-- )
        {
          // Go through pixel columns
          for ( uint32 pCol = 0; pCol < tile_width; pCol++ )
          {
            // Get the pixel offsets
            uint32 pX = ( tile_width * cols ) + pCol;
            uint32 pY = ( tile_height * rows ) + pRow;

            // If a non colorkey pixel is found
            if ( this->bitmap_font_.pixel ( pX, pY ) != background_color )
            {
              // Bottom of a is found
              baseA = pRow;

              //Break the loops
              pCol = tile_width;
              pRow = -1;
            }
          } // end for pixel columns loop
        } // end for pixel rows loop
      } // end if currentChar

      // Go to the next character
      currentChar++;
    }
  }
  this->bitmap_font_.unlock(); // Finished messing with pixels

  // Calculate space
  this->set_spacing ( tile_width / 2 );

  // Calculate new line
  this->set_newline ( baseA - top );

  // Loop off excess top pixels
  for ( uint32 p = 0; p < 256; p++ )
  {
    this->glyphs_[ p ].bounds.y += top;
    this->glyphs_[ p ].bounds.h -= top;
  }

  // Dump table of calculated bitmap character positions
  #if defined (NOM_DEBUG_SDL2_BITMAP_FONT)
    NOM_DUMP_VAR(this->spacing());
    NOM_DUMP_VAR(this->newline());
    for ( uint32 glyph = 0; glyph < 256; ++glyph )
    {
      NOM_DUMP_VAR ( glyph );
      NOM_DUMP_VAR ( this->glyphs_[ glyph ].bounds.x );
      NOM_DUMP_VAR ( this->glyphs_[ glyph ].bounds.y );
      NOM_DUMP_VAR ( this->glyphs_[ glyph ].bounds.w );
      NOM_DUMP_VAR ( this->glyphs_[ glyph ].bounds.h );
    }
  #endif

  //this->render_font.initialize ( this->bitmap_font_.image() );

  return true;
}

sint BitmapFont::sheet_width ( void ) const
{
  return this->sheet_width_;
}

sint BitmapFont::sheet_height ( void ) const
{
  return this->sheet_height_;
}

} // namespace nom

