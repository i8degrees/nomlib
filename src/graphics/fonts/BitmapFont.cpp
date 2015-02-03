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

// Private headers
#include "nomlib/math/Rect.hpp"
#include "nomlib/system/File.hpp"
#include "nomlib/system/SDL_helpers.hpp"

// Forward declarations
#include "nomlib/graphics/Image.hpp"

namespace nom {

BitmapFont::BitmapFont ( void ) :
  type_ ( IFont::FontType::BitmapFont ),
  sheet_width_ ( 16 ),
  sheet_height_ ( 16 )
{
  // NOM_LOG_TRACE( NOM );
}

BitmapFont::~BitmapFont ( void )
{
  // NOM_LOG_TRACE( NOM );
}

BitmapFont::BitmapFont ( const BitmapFont& copy ) :
  type_( copy.type() ),
  sheet_width_( copy.sheet_width() ),
  sheet_height_( copy.sheet_height() ),
  pages_( copy.pages() ),
  metrics_( copy.metrics() )
{
  // NOM_LOG_TRACE( NOM );
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

const Image* BitmapFont::image(uint32 character_size) const
{
  return this->pages_[0].texture.get();
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
  return 0;
}

int BitmapFont::hinting( void ) const
{
  return 0; // Not implemented
}

uint32 BitmapFont::font_style( void ) const
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
  // NOM_STUBBED( NOM );

  return false;
}

bool BitmapFont::set_hinting( int type )
{
  // NOM_STUBBED( NOM );

  return false;
}

bool BitmapFont::set_outline( int )
{
  // NOM_STUBBED( NOM );

  return false;
}

void BitmapFont::set_font_style( uint32 style )
{
  // Not implemented

  // NOM_STUBBED( NOM );
}

void BitmapFont::set_font_kerning( bool state )
{
  // Not implemented

  // NOM_STUBBED( NOM );
}

bool BitmapFont::load( const std::string& filename )
{
  // Set the font's face name as the filename of the bitmap font.
  this->metrics_.name = filename;

  if( this->pages_[0].texture->load(filename, SDL_PIXELFORMAT_ARGB8888) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not load bitmap font image file: " + filename );
    return false;
  }

  if ( this->pages_[0].texture->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "Internal bitmap texture is invalid" );
    return false;
  }

  // Set pixel at coordinates 0, 0 to be the color mask
  uint32 key = this->pages_[0].texture->pixel(0, 0);
  this->color_mask_ = nom::pixel( key, this->pages_[0].texture->pixel_format() );

  // Attempt to build font metrics
  if ( this->build(0) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not build bitmap font metrics" );
    return false;
  }

  // Encode the color mask as a fully transparent alpha channel; this will hide
  // the filtered pixel data -- visually seen as the glyph's background color
  // from the image we load. This must be done **after** the glyph data is
  // built, or else it won't have anything to compare against.
  //
  // This exists as an optimization for nom::Text; we could have just set the
  // color key to the value of the color mask before building the glyph metrics,
  // but when the pixel data is copied to update a SDL_TEXTURE_STREAMING texture
  // type, we lose the color key in the process. We can't use
  // nom::Texture::set_colorkey at that time, due to inefficiency -- not
  // surprisingly, performance drops drastically.
  Image* source = this->pages_[0].texture.get();
  NOM_ASSERT(source != nullptr);

  uint32* pixels = NOM_SCAST(uint32*, source->pixels() );

  Color4i trans_color = this->color_mask_;
  trans_color.a = 0;

  uint32 trans_pixel = nom::RGBA( trans_color, source->pixel_format() );

  for( auto idx = 0; idx < (source->pitch() / 4) * source->height(); ++idx ) {

    if( pixels[idx] == key ) {
      pixels[idx] = trans_pixel;
    }
  }

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
                            this->color_mask_,
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

  this->metrics_.newline = base - top;

  // NOTE: The original new line feed computation -- 'base - top', breaks text
  // rendering bounds within nom::Text::update_cache because the height
  // calculation it relies on was inaccurate.
  this->metrics_.newline = base + top + 1;

  // Loop off excess top pixels
  for ( uint32 top_pixels = 0; top_pixels < current_char; ++top_pixels )
  {
    page.glyphs[top_pixels].bounds.y += top;
    page.glyphs[top_pixels].bounds.h -= top;
  }

  // Dump table of calculated bitmap character positions
  #if defined (NOM_DEBUG_SDL2_BITMAP_FONT)
    NOM_DUMP(this->spacing());
    NOM_DUMP(this->newline());
    for ( uint32 glyph = 0; glyph < current_char; ++glyph )
    {
      NOM_DUMP( glyph );
      NOM_DUMP( page.glyphs[glyph].bounds.x );
      NOM_DUMP( page.glyphs[glyph].bounds.y );
      NOM_DUMP( page.glyphs[glyph].bounds.w );
      NOM_DUMP( page.glyphs[glyph].bounds.h );
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
