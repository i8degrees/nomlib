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
  sheet_width_ ( 16 ),
  sheet_height_ ( 16 ),
  newline_ ( 0 ),
  spacing_ ( 0 ),
  type_ ( IFont::FontType::BitmapFont )
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
  //this->bitmap_font_ = copy.bitmap_font_;
  this->pages_ = copy.pages();
  this->newline_ = copy.newline();
  this->spacing_ = copy.spacing();
  this->type_ = copy.type();
}

IFont::SharedPtr BitmapFont::clone ( void ) const
{
  return BitmapFont::SharedPtr ( new BitmapFont ( *this ) );
}

bool BitmapFont::valid ( void ) const
{
  if ( this->pages_[0].texture->valid() == true ) return true;

  return false;
}

enum IFont::FontType BitmapFont::type ( void ) const
{
  return this->type_;
}

SDL_SURFACE::RawPtr BitmapFont::image ( void ) const
{
  return this->pages_[0].texture->image();
}

/*
const Texture& BitmapFont::texture ( uint32 character_size ) //const
{
  return this->pages_[character_size].texture;
}
*/

uint BitmapFont::spacing ( void ) const
{
  return this->spacing_;
}

const Glyph& BitmapFont::glyph ( uint32 codepoint, uint32 character_size ) const
{
  GlyphAtlas& glyphs = this->pages_[character_size].glyphs;

  return glyphs[codepoint];
}
/* TODO
  GlyphAtlas::const_iterator it = glyphs.find(codepoint);

  if ( it != glyphs.end() )
  {
    return it->second;
  }
  else
  {
    // FIXME: implement support for handling this condition
    return Glyph();
  }
*/

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
  if ( this->pages_[0].texture->load ( filename, SDL_PIXELFORMAT_RGBA8888 ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not load bitmap font image file: " + filename );
    return false;
  }

  this->pages_[0].texture->set_colorkey ( colorkey, true );

  // Attempt to build font metrics
  if ( this->build() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not build bitmap font metrics" );
    return false;
  }

  return true;
}

bool BitmapFont::build ( uint32 character_size )
{
  uint32 tile_width = 0;
  uint32 tile_height = 0;
  uint32 top = 0;
  uint32 base = 0; // baseline of character 'A'
  uint32 current_char = 0; // counter
  uint32 background_color = 0;

  // GlyphPage to be filled in (indexed by character_size)
  //struct FontPage page;

  NOM_ASSERT ( this->pages_[0].texture->valid() );

  if ( this->pages_[0].texture->lock() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not lock surface." );
    return false;
  }

  background_color = RGBA (
                            this->pages_[0].texture->colorkey(),
                            this->pages_[0].texture->pixel_format()
                          );

  tile_width = this->pages_[0].texture->width() / this->sheet_width();
  tile_height = this->pages_[0].texture->height() / this->sheet_height();
  top = tile_height;
  base = tile_height;

  for ( int32 rows = 0; rows < this->sheet_width(); rows++ )
  {
    for ( int32 cols = 0; cols < this->sheet_height(); cols++ )
    {
      // Set character offsets
      this->pages_[0].glyphs[current_char].bounds.x = tile_width * cols;
      this->pages_[0].glyphs[current_char].bounds.y = tile_height * rows;
      this->pages_[0].glyphs[current_char].bounds.w = tile_width;
      this->pages_[0].glyphs[current_char].bounds.h = tile_height;

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
          if( this->pages_[0].texture->pixel ( pX, pY ) != background_color )
          {
            //Set the x offset
            this->pages_[0].glyphs[current_char].bounds.x = pX;

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
          if ( this->pages_[0].texture->pixel ( pX, pY ) != background_color )
          {
            //Set the width
            this->pages_[0].glyphs[current_char].bounds.w = ( pX - this->pages_[0].glyphs[current_char].bounds.x ) + 1;

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
          if( this->pages_[0].texture->pixel ( pX, pY ) != background_color )
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
      if ( current_char == 'A' )
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
            if ( this->pages_[0].texture->pixel ( pX, pY ) != background_color )
            {
              // Bottom of a is found
              base = pRow;

              //Break the loops
              pCol = tile_width;
              pRow = -1;
            }
          } // end for pixel columns loop
        } // end for pixel rows loop
      } // end if current_char

      // Go to the next character
      current_char++;
    }
  }
  this->pages_[0].texture->unlock(); // Finished messing with pixels

  // Calculate space
  this->set_spacing ( tile_width / 2 );

  // Calculate new line
  this->set_newline ( base - top );

  // Loop off excess top pixels
  for ( uint32 top_pixels = 0; top_pixels < current_char; ++top_pixels )
  {
    this->pages_[0].glyphs[top_pixels].bounds.y += top;
    this->pages_[0].glyphs[top_pixels].bounds.h -= top;
  }

  // Dump table of calculated bitmap character positions
  #if defined (NOM_DEBUG_SDL2_BITMAP_FONT)
    NOM_DUMP_VAR(this->spacing());
    NOM_DUMP_VAR(this->newline());
    for ( uint32 glyph = 0; glyph < current_char; ++glyph )
    {
      NOM_DUMP_VAR ( glyph );
      NOM_DUMP_VAR ( this->pages_[0].glyphs[glyph].bounds.x );
      NOM_DUMP_VAR ( this->pages_[0].glyphs[glyph].bounds.y );
      NOM_DUMP_VAR ( this->pages_[0].glyphs[glyph].bounds.w );
      NOM_DUMP_VAR ( this->pages_[0].glyphs[glyph].bounds.h );
    }
  #endif

/*
  page.texture.initialize ( this->pages_[0].texture->width(),
                            this->pages_[0].texture->height(),
                            SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_STATIC
                          );
*/

  for ( uint32 glyph = 0; glyph < current_char; ++glyph )
  {
    //IntRect gbounds = IntRect ( page.glyphs[glyph].bounds );
    /*
    page.texture.initialize ( gbounds.w, gbounds.h, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC );
    page.texture.update ( this->pages_[0].texture->pixels(), this->pages_[0].texture->pitch(), gbounds );
    */
    //page.texture->set_bounds ( page.glyphs[glyph].bounds );
  }

  //page.texture = pages_[0].texture;
  //page.texture = std::shared_ptr<Image> ( this->pages_[0].texture->clone() );
  //this->pages_.insert( std::pair<uint32, FontPage> ( character_size, page ) ).first;

  //NOM_DUMP_VAR(this->pages_[0].glyphs[65].bounds.x);
  //NOM_DUMP_VAR(this->pages_[0].glyphs[65].bounds.y);
  //NOM_DUMP_VAR(this->pages_[0].glyphs[65].bounds.w);
  //NOM_DUMP_VAR(this->pages_[0].glyphs[65].bounds.h);

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

const GlyphPage& BitmapFont::pages ( void ) const
{
  return this->pages_;
}

} // namespace nom

