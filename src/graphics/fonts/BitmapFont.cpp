/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/graphics/fonts/BitmapFont.hpp"

namespace nom {

BitmapFont::BitmapFont ( void ) :
  type_ ( IFont::FontType::BitmapFont ),
  sheet_width_ ( 16 ),
  sheet_height_ ( 16 )
{
  #if ! defined( NOM_DISABLE_GFX_LOG_TRACE )
    NOM_LOG_TRACE( NOM );
  #endif
}

BitmapFont::~BitmapFont ( void )
{
  #if ! defined( NOM_DISABLE_GFX_LOG_TRACE )
    NOM_LOG_TRACE( NOM );
  #endif
}

BitmapFont::BitmapFont ( const BitmapFont& copy ) :
  type_ { copy.type() },
  sheet_width_ { copy.sheet_width() },
  sheet_height_ { copy.sheet_height() },
  pages_ { copy.pages() },
  metrics_ { copy.metrics() }
{
  NOM_LOG_TRACE( NOM );
}

IFont::raw_ptr BitmapFont::clone( void ) const
{
  return new BitmapFont( *this );
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

const Image& BitmapFont::image ( uint32 character_size ) const
{
  return *this->pages_[0].texture.get();
}

sint BitmapFont::spacing ( uint32 character_size ) const
{
  return this->pages_[0].glyphs[65].advance; // 'A' ASCII character
}

int BitmapFont::newline( uint32 character_size ) const
{
  return this->metrics_.newline;
}

int BitmapFont::kerning( uint32 first_char, uint32 second_char, uint32 character_size ) const
{
  return 0; // TODO
}

int BitmapFont::hinting( void ) const
{
  return 0; // Not implemented
}

const Glyph& BitmapFont::glyph ( uint32 codepoint, uint32 character_size ) const
{
  GlyphAtlas& glyphs = this->pages_[0].glyphs;

  GlyphAtlas::const_iterator it = glyphs.find(codepoint);
  if ( it != glyphs.end() )
  {
    return it->second; // Found a match
  }
  else
  {
    return glyphs[codepoint]; // Err (probably going to receive an empty glyph)
  }
}

bool BitmapFont::set_point_size( int point_size )
{
  return false; // TODO
}

bool BitmapFont::set_hinting( int type )
{
  return false; // Not implemented
}

bool BitmapFont::set_outline( int )
{
  return false; // Not implemented
}

bool BitmapFont::load( const std::string& filename )
{
  // Set the font's face name as the filename of the bitmap font.
  this->metrics_.name = filename;

  // I don't understand why RGBA8888 is necessary here, but it is the only pixel
  // format that I have found that works when we are initializing a
  // nom::Texture (SDL_Texture) from a nom::Image (SDL_Surface).
  if ( this->pages_[0].texture->load ( filename, SDL_PIXELFORMAT_RGBA8888 ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not load bitmap font image file: " + filename );
    return false;
  }

  if ( this->pages_[0].texture->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "Internal bitmap texture is invalid" );
    return false;
  }

  // Set pixel at coordinates 0, 0 to be color keyed (transparent)
  uint32 key = this->pages_[0].texture->pixel( 0, 0 );
  Color4i colorkey = nom::pixel ( key, this->pages_[0].texture->pixel_format() );

  if ( this->pages_[0].texture->set_colorkey ( colorkey, true ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not set color key" );
    return false;
  }

  // Attempt to build font metrics
  if ( this->build(0) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not build bitmap font metrics" );
    return false;
  }

  // FIXME (?): Setting the font face name as the filename, for lack of a
  // better idea at the moment...
  this->metrics_.name = filename;

  return true;
}

const FontMetrics& BitmapFont::metrics( void ) const
{
  return this->metrics_;
}

bool BitmapFont::build ( uint32 character_size )
{
  // The glyph used to base every glyph's height, Y bounds coordinate and
  // newline calculations off
  const uint32 baseline_glyph = 'A'; // ASCII character for 'A'
  uint32 tile_width = 0;
  uint32 tile_height = 0;
  uint32 top = 0; // Ascent (I think)
  uint32 base = 0; // Baseline of character 'A'
  uint32 current_char = 0; // counter
  uint32 background_color = 0;

  FontPage& page = this->pages_[0]; // Our font's current glyph page

  NOM_ASSERT ( page.texture->valid() );

  if ( page.texture->lock() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not lock surface." );
    return false;
  }

  // The color mask used during our glyph scan -- in other words, this is the
  // pixel color we filter out (ignore) -- any other pixel color becomes part
  // of our glyph atlas.
  background_color = RGBA (
                            page.texture->colorkey(),
                            page.texture->pixel_format()
                          );

  tile_width = page.texture->width() / this->sheet_width();
  tile_height = page.texture->height() / this->sheet_height();

  top = tile_height;
  base = tile_height;

  for ( int32 rows = 0; rows < this->sheet_width(); rows++ )
  {
    for ( int32 cols = 0; cols < this->sheet_height(); cols++ )
    {
      // Initialize the glyph offsets
      page.glyphs[current_char].bounds.x = tile_width * cols;
      page.glyphs[current_char].bounds.y = tile_height * rows;
      page.glyphs[current_char].bounds.w = tile_width;
      page.glyphs[current_char].bounds.h = tile_height;

      //Find the left side of the glyph
      for ( int pCol = 0; pCol < tile_width; pCol++ )
      {
        for ( int pRow = 0; pRow < tile_height; pRow++ )
        {
          //Get the pixel offsets
          uint32 pX = ( tile_width * cols ) + pCol;
          uint32 pY = ( tile_height * rows ) + pRow;

          // Bingo -- a match is found (non-mask pixel color)
          if( page.texture->pixel ( pX, pY ) != background_color )
          {
            //Set the x offset
            page.glyphs[current_char].bounds.x = pX;

            //Break the loops
            pCol = tile_width;
            pRow = tile_height;
          }
        }
      }

      //Find the right side of the glyph
      for ( int pCol_w = tile_width - 1; pCol_w >= 0; pCol_w-- )
      {
        for ( int pRow_w = 0; pRow_w < tile_height; pRow_w++ )
        {
          //Get the pixel offsets
          uint32 pX = ( tile_width * cols ) + pCol_w;
          uint32 pY = ( tile_height * rows ) + pRow_w;

          // Bingo -- a match is found (non-mask pixel color)
          if ( page.texture->pixel ( pX, pY ) != background_color )
          {
            //Set the width
            page.glyphs[current_char].bounds.w = ( pX - page.glyphs[current_char].bounds.x ) + 1;

            //Break the loops
            pCol_w = -1;
            pRow_w = tile_height;
          }
        }
      }

      //Find the top of the glyph
      for ( int pRow = 0; pRow < tile_height; pRow++ )
      {
        for ( int pCol = 0; pCol < tile_width; pCol++ )
        {
          //Get the pixel offsets
          uint32 pX = ( tile_width * cols ) + pCol;
          uint32 pY = ( tile_height * rows ) + pRow;

          // Bingo -- a match is found (non-mask pixel color)
          if( page.texture->pixel ( pX, pY ) != background_color )
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

      // Calculate character spacing
      page.glyphs[current_char].advance = page.glyphs[current_char].bounds.w;

      // Calculate baseline from chosen glyph
      if ( current_char == baseline_glyph )
      {
        for ( int pRow = tile_height - 1; pRow >= 0; pRow-- )
        {
          for ( int pCol = 0; pCol < tile_width; pCol++ )
          {
            // Get the pixel offsets
            uint32 pX = ( tile_width * cols ) + pCol;
            uint32 pY = ( tile_height * rows ) + pRow;

            // Bingo -- a match is found (non-mask pixel color)
            if ( page.texture->pixel ( pX, pY ) != background_color )
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
      current_char++;
    } // end sheet height loop
  } // end sheet width loop

  page.texture->unlock(); // Finished messing with pixels

  // Calculate new line by subtracting the baseline of the "chosen glyph"
  // from the bitmap's ascent.
  //
  // (See also: baseline_glyph variable)
  this->metrics_.newline = base - top;

  // Loop off excess top pixels
  for ( uint32 top_pixels = 0; top_pixels < current_char; ++top_pixels )
  {
    page.glyphs[top_pixels].bounds.y += top;
    page.glyphs[top_pixels].bounds.h -= top;
  }

  // Dump table of calculated bitmap character positions
  #if defined (NOM_DEBUG_SDL2_BITMAP_FONT)
    NOM_DUMP_VAR(this->spacing());
    NOM_DUMP_VAR(this->newline());
    for ( uint32 glyph = 0; glyph < current_char; ++glyph )
    {
      NOM_DUMP_VAR ( glyph );
      NOM_DUMP_VAR ( page.glyphs[glyph].bounds.x );
      NOM_DUMP_VAR ( page.glyphs[glyph].bounds.y );
      NOM_DUMP_VAR ( page.glyphs[glyph].bounds.w );
      NOM_DUMP_VAR ( page.glyphs[glyph].bounds.h );
    }
  #endif

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
