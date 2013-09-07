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

BitmapFont::BitmapFont ( void ) : sheet_width ( 16 ), sheet_height ( 16 )
{
NOM_LOG_TRACE ( NOM );

  this->text_buffer = "\0";
  this->text_style = FontStyle::Regular;

  this->newline = 0;
  this->spacing = 0;

  this->color = Color ( 0, 0, 0 );
  this->coords.setPosition ( 0, 0 );

  for ( unsigned int idx = 0; idx < 256; idx++ )
  {
    this->chars[idx] = Coords ( 0, 0, 0, 0 );
  }
}

BitmapFont::~BitmapFont ( void )
{
NOM_LOG_TRACE ( NOM );
}

IFont::SharedPtr BitmapFont::clone ( void ) const
{
  return IFont::SharedPtr ( new BitmapFont ( *this ), priv::Free_BitmapFont );
}

const std::string& BitmapFont::getText ( void ) const
{
  return this->text_buffer;
}

int32 BitmapFont::getFontWidth ( void ) const
{
  int32 text_width = 0;

  for ( int32 char_pos = 0; char_pos < this->text_buffer.length(); char_pos++ )
  {
    if ( this->text_buffer [ char_pos ] == ' ' )
    {
      text_width += this->spacing;

// Dump each character's table used for calculation
#if defined ( NOM_DEBUG_SDL_BITMAP_FONT )
NOM_DUMP_VAR ( char_pos );
NOM_DUMP_VAR ( this->text_buffer [ char_pos ] );
NOM_DUMP_VAR ( text_width );
#endif

    }
    else if ( this->text_buffer [ char_pos ] == '\n' ) // TODO
    {
      text_width = 0;
    }
    else
    {
      uint8 ascii_char = static_cast<uchar> ( this->text_buffer [ char_pos ] );
      text_width += this->chars [ ascii_char ].width + 1;

// Dump each character's table used for calculation
#if defined ( NOM_DEBUG_SDL_BITMAP_FONT )
NOM_DUMP_VAR ( char_pos );
NOM_DUMP_VAR ( this->text_buffer [ char_pos ] );
NOM_DUMP_VAR ( this->chars [ ascii_char ].width + 1 );
NOM_DUMP_VAR ( text_width );
#endif

    }
  } // end for loop

  return text_width;
}

int32 BitmapFont::getFontHeight ( void ) const
{
  int32 text_height = 0;

  for ( int32 char_pos = 0; char_pos < this->text_buffer.length(); char_pos++ )
  {
    if ( this->text_buffer [ char_pos ] == '\n' )
    {
      text_height += this->newline;

// Dump each character's table used for calculation
#if defined ( NOM_DEBUG_SDL_BITMAP_FONT )
NOM_DUMP_VAR ( char_pos );
NOM_DUMP_VAR ( this->text_buffer [ char_pos ] );
NOM_DUMP_VAR ( text_height );
#endif

    }
    else
    {
      uint8 ascii_char = static_cast<uchar> ( this->text_buffer [ char_pos ] );

      text_height = this->chars [ ascii_char ].height;

// Dump each character's table used for calculation
#if defined ( NOM_DEBUG_SDL_BITMAP_FONT )
NOM_DUMP_VAR ( char_pos );
NOM_DUMP_VAR ( this->text_buffer [ char_pos ] );
NOM_DUMP_VAR ( this->chars [ ascii_char ].height );
NOM_DUMP_VAR ( text_height );
#endif

    }
  }

  return text_height;
}

const Color& BitmapFont::getColor ( void ) const
{
  return this->color;
}

const Coords& BitmapFont::getPosition ( void ) const
{
  return this->coords;
}

void BitmapFont::setColor ( const Color& color )
{
  this->color = color;
}

void BitmapFont::setPosition ( const Coords& coords )
{
  this->coords = coords;
}

void BitmapFont::setText ( const std::string& text )
{
  this->text_buffer = text;
}

uint32 BitmapFont::getSpacing ( void ) const
{
  return this->spacing;
}

void BitmapFont::setSpacing ( uint32 spaces )
{
  this->spacing = spaces;
}

uint32 BitmapFont::getNewline ( void ) const
{
  return this->newline;
}

void BitmapFont::setNewline ( uint32 newline )
{
  this->newline = newline;
}

FontStyle BitmapFont::getFontStyle ( void ) const
{
  return this->text_style;
}

void BitmapFont::setFontStyle ( uint8 style, uint8 options )
{
  switch ( style )
  {
    default:
    break;
    case FontStyle::Regular:
    case FontStyle::Bold:
    case FontStyle::Italic:
    case FontStyle::Underlined:
      // Do nothing stub
    break;

    /// Text effect utilizing alpha channels for the appearance of gray text
    case FontStyle::Faded:
    {
      if ( this->bitmap_font.valid() )
        if ( this->bitmap_font.setAlpha ( options ) == true )
          this->text_style = FontStyle::Faded;
    break;
    }
  }
}

bool BitmapFont::rebuild ( void )
{
  uint32 tile_width = 0;
  uint32 tile_height = 0;
  uint32 top = 0;
  uint32 baseA = 0;
  uint32 currentChar = 0;
  uint32 background_color = 0;

NOM_ASSERT ( this->bitmap_font.valid() );

  background_color = getColorAsInt  ( this->bitmap_font.getCanvasPixelsFormat(),
                                      this->colorkey
                                    );

  this->bitmap_font.setTransparent ( this->colorkey, SDL_SRCCOLORKEY );

  tile_width = this->bitmap_font.getCanvasWidth() / this->sheet_width;
  tile_height = this->bitmap_font.getCanvasHeight() / this->sheet_height;
  top = tile_height;
  baseA = tile_height;

  for ( uint32_t rows = 0; rows < this->sheet_width; rows++ )
  {
    for ( uint32_t cols = 0; cols < this->sheet_height; cols++ )
    {
      // Set character offsets
      this->chars[ currentChar ].setPosition ( tile_width * cols, tile_height * rows );
      this->chars[ currentChar ].setSize ( tile_width, tile_height );

      //Find Left Side; go through pixel columns
      for ( uint32_t pCol = 0; pCol < tile_width; pCol++ )
      {
        //Go through pixel rows
        for ( uint32_t pRow = 0; pRow < tile_height; pRow++ )
        {
          //Get the pixel offsets
          int pX = ( tile_width * cols ) + pCol;
          int pY = ( tile_height * rows ) + pRow;

          //If a non colorkey pixel is found
          if( this->bitmap_font.getPixel ( pX, pY ) != background_color )
          {
              //Set the x offset
              this->chars[ currentChar ].x = pX;

              //Break the loops
              pCol = tile_width;
              pRow = tile_height;
          }
        }
      }

      //Find Right Side; go through pixel columns
      for ( int32_t pCol_w = tile_width - 1; pCol_w >= 0; pCol_w-- )
      {
        //Go through pixel rows
        for ( uint32 pRow_w = 0; pRow_w < tile_height; pRow_w++ )
        {
          //Get the pixel offsets
          uint32_t pX = ( tile_width * cols ) + pCol_w;
          uint32_t pY = ( tile_height * rows ) + pRow_w;

          //If a non colorkey pixel is found
          if ( this->bitmap_font.getPixel ( pX, pY ) != background_color )
          {
            //Set the width
            this->chars[ currentChar ].width = ( pX - this->chars[ currentChar ].x ) + 1;

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
          uint32_t pX = ( tile_width * cols ) + pCol;
          uint32_t pY = ( tile_height * rows ) + pRow;

          // If a non colorkey pixel is found
          if( this->bitmap_font.getPixel ( pX, pY ) != background_color )
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
        for ( int32_t pRow = tile_height - 1; pRow >= 0; pRow-- )
        {
          // Go through pixel columns
          for ( uint32 pCol = 0; pCol < tile_width; pCol++ )
          {
            // Get the pixel offsets
            unsigned int pX = ( tile_width * cols ) + pCol;
            unsigned int pY = ( tile_height * rows ) + pRow;

            // If a non colorkey pixel is found
            if ( this->bitmap_font.getPixel ( pX, pY ) != background_color )
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

  // Calculate space
  this->spacing = tile_width / 2;

  // Calculate new line
  this->newline = baseA - top;

  // Loop off excess top pixels
  for ( uint32_t t = 0; t < 256; t++ )
  {
    this->chars[ t ].y += top;
    this->chars[ t ].height -= top;
  }

// Dump table of calculated bitmap character positions
#if defined ( NOM_DEBUG_SDL_BITMAP_FONT )
  for ( uint32 pos = 0; pos < 256; pos++ )
  {
NOM_DUMP_VAR ( pos );
NOM_DUMP_VAR ( static_cast<uchar> ( pos ) );
NOM_DUMP_VAR ( this->chars [ pos ].x );
NOM_DUMP_VAR ( this->chars [ pos ].y );
NOM_DUMP_VAR ( this->chars [ pos ].width );
NOM_DUMP_VAR ( this->chars [ pos ].height );
  }
#endif

  return true;
}

bool BitmapFont::load ( const std::string& filename, const Color& colorkey,
                        bool use_cache
                      )
{
  if ( this->bitmap_font.load ( filename, colorkey, use_cache ) == false )
  {
NOM_LOG_ERR ( NOM, "Could not load bitmap font image file: " + filename );
    return false;
  }

  this->colorkey = colorkey;

  // Attempt to rebuild font metrics
  if ( this->rebuild() == false )
  {
NOM_LOG_ERR ( NOM, "Could not rebuild bitmap font metrics" );
    return false;
  }

  return true;
}

const Coords BitmapFont::findGlyph ( const std::string& glyph )
{
  uint8 ascii = 0;
  std::istringstream i ( glyph );

  i >> ascii;

  return this->chars[ascii];
}

void BitmapFont::Update ( void )
{
  // Stub
}

// TODO: test \t (horizontal tabbing) feature
void BitmapFont::Draw ( void* video_buffer ) const
{
  // Use coordinates provided by interface user as our starting origin
  // coordinates to compute from
  int32_t x_offset = this->coords.x;
  int32_t y_offset = this->coords.y;

  //If the font has been built
  if ( this->bitmap_font.valid() )
  {
    for ( uint32_t show = 0; show < this->text_buffer.length(); show++ )
    {
      //If the current character is a space
      if ( this->text_buffer[show] == ' ' )
      {
        //Move over
        x_offset += this->spacing;
      }
      // If the current character is a newline
      else if( this->text_buffer[show] == '\n' )
      {
        //Move down and back over to the beginning of line
        y_offset += this->newline;
        x_offset = this->coords.x;
      }
      // If the current character is a tab
      else if( this->text_buffer[show] == '\t' )
      {
        x_offset += this->spacing * 4;
      }
      else
      {
        //Get the ASCII value of the character
        uint8_t ascii = static_cast<u_char>( this->text_buffer[show] );

        this->bitmap_font.setPosition ( Coords ( x_offset, y_offset ) );
        this->bitmap_font.setOffsets ( this->chars[ascii] );
        this->bitmap_font.Draw ( video_buffer );

        // Move over the width of the character with one pixel of padding
        x_offset += ( this->chars[ascii].width ) + 1;
      } // end else
    } // end for loop
  } // end if this->bitmap_font != nullptr
}

bool BitmapFont::resize ( enum ResizeAlgorithm scaling_algorithm )
{
  if ( this->bitmap_font.valid() == false )
  {
NOM_LOG_ERR ( NOM, "Video surface is invalid." );
    return false;
  }

  if ( this->bitmap_font.resize ( scaling_algorithm ) == false )
  {
NOM_LOG_ERR ( NOM, "Failed to resize the video surface." );
    return false;
  }

  if ( this->rebuild() == false )
  {
NOM_LOG_ERR ( NOM, "Could not rebuild bitmap font metrics" );
    return false;
  }

  return true;
}

  namespace priv {

void Free_BitmapFont ( BitmapFont* ptr )
{
  // Do nothing custom deleter
  //
  // FIXME; this is a known bug (memory leak).
}


  } // namespace priv
} // namespace nom

