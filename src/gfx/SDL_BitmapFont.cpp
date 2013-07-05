/******************************************************************************

    SDL-based Bitmap Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

  Portions Copyright Lazy Foo' Productions 2004-2013
  All rights reserved.

  1. http://lazyfoo.net/SDL_tutorials/lesson30/index.php

******************************************************************************/
#include "SDL_BitmapFont.hpp"

using namespace nom;

SDL_BitmapFont::SDL_BitmapFont ( void )
{
NOMLIB_LOG_INFO;

  this->text_buffer = "\0";
  this->newline = 0; // holds the y coords value to increment upon newline break char
  this->spacing = 0; // holds the x coords value to increment upon space char
  this->text_style = Style::Regular; // default text styling effect

  this->setPosition ( 0, 0 );

  for ( unsigned int idx = 0; idx < 256; idx++ )
  {
    this->chars[idx] = nom::Coords ( 0, 0, 0, 0 );
  }
}

SDL_BitmapFont::~SDL_BitmapFont ( void )
{
NOMLIB_LOG_INFO;
}

int32_t SDL_BitmapFont::getTextWidth ( void )
{
  int32_t text_width = 0;

  if ( this->bitmap_font.valid() )
  {
    for ( nom::ulong t = 0; t < this->text_buffer.length(); t++ )
    {
      if ( this->text_buffer[t] == ' ' )
        text_width += this->chars[t].width / this->spacing;
      else if ( this->text_buffer[t] == '\n' )
        text_width = 0;
      else
        text_width += this->chars[t].width - ( this->spacing + 2 );
    }
  }

  return text_width;
}

int32_t SDL_BitmapFont::getTextHeight ( void )
{
  int32_t text_height = 0;

  for ( nom::ulong t = 0; t < this->text_buffer.length(); t++ )
  {
    if ( this->text_buffer[t] == '\n' )
      text_height += this->newline;
    else
      text_height = this->chars[t].height;
  }

  return text_height;
}

const std::string& SDL_BitmapFont::getText ( void ) const
{
  return this->text_buffer;
}

void SDL_BitmapFont::setText ( const std::string& text )
{
  this->text_buffer = text;
}

uint32_t SDL_BitmapFont::getSpacing ( void )
{
  return this->spacing;
}

void SDL_BitmapFont::setSpacing ( uint32_t spaces )
{
  this->spacing = spaces;
}

uint32_t SDL_BitmapFont::getNewline ( void )
{
  return this->newline;
}

void SDL_BitmapFont::setNewline ( uint32_t newline )
{
  this->newline = newline;
}

uint8_t nom::SDL_BitmapFont::getStyle ( void ) const
{
  return this->text_style;
}

void SDL_BitmapFont::setStyle ( uint8_t style, uint8_t options )
{
  switch ( style )
  {
    default:
    break;
    case Style::Regular:
    case Style::Bold:
    case Style::Italic:
    case Style::Underlined:
      // Do nothing stub
    break;

    /// Text effect utilizing alpha channels for the appearance of gray text
    case Style::Faded:
    {
      if ( this->bitmap_font.valid() )
        if ( this->bitmap_font.setAlpha ( options ) == true )
          this->text_style = Style::Faded;
    break;
    }
  }
}

bool SDL_BitmapFont::Load ( const std::string& filename, const nom::Color& colorkey, uint32_t sheet_width, uint32_t sheet_height )
{
  uint32_t tile_width = 0;
  uint32_t tile_height = 0;
  uint32_t top = 0;
  uint32_t baseA = 0;
  uint32_t currentChar = 0;
  int32_t background_color = 0;

  if ( this->bitmap_font.loadFromImage ( filename, colorkey ) == false )
  {
NOMLIB_LOG_ERR ( "Could not load bitmap font image file: " + filename );
    return false;
  }

NOMLIB_ASSERT ( this->bitmap_font.valid() );

  background_color = colorkey.getColorAsInt ( this->bitmap_font.getCanvasPixelsFormat() );

  tile_width = this->bitmap_font.getCanvasWidth() / sheet_width;
  tile_height = this->bitmap_font.getCanvasHeight() / sheet_height;
  top = tile_height;
  baseA = tile_height;

  for ( uint32_t rows = 0; rows < sheet_width; rows++ )
  {
    for ( uint32_t cols = 0; cols < sheet_height; cols++ )
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
        for ( nom::uint32 pRow_w = 0; pRow_w < tile_height; pRow_w++ )
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
      for ( nom::uint32 pRow = 0; pRow < tile_height; pRow++ )
      {
        //Go through pixel columns
        for ( nom::uint32 pCol = 0; pCol < tile_width; pCol++ )
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

      // FIXME: I don't think this does anything for us
      // Find Bottom of A
      if ( currentChar == 'A' )
      {
        // Go through pixel rows
        for ( int32_t pRow = tile_height - 1; pRow >= 0; pRow-- )
        {
          // Go through pixel columns
          for ( nom::uint32 pCol = 0; pCol < tile_width; pCol++ )
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
          }
        }
      }

      // Go to the next character
      currentChar++;
    }
  }

  // Checks to see if we have modified the instance variable before calling this
  // method
  if ( this->spacing == 0 )
  {
    // Calculate space
    this->spacing = tile_width / 2;
  }

  // Calculate new line
  this->newline = baseA - top;

  // Loop off excess top pixels
  for ( uint32_t t = 0; t < 256; t++ )
  {
    this->chars[ t ].y += top;
    this->chars[ t ].height -= top;
  }

  return true;
}

const nom::Coords nom::SDL_BitmapFont::findGlyph ( const std::string& glyph )
{
  std::istringstream i ( glyph );

  uint8_t ascii;
  i >> ascii;

  return this->chars[ascii];
}

void nom::SDL_BitmapFont::Update ( void )
{
  // Stub
}

// TODO: test \t (horizontal tabbing) feature
void SDL_BitmapFont::Draw ( void* video_buffer )
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
      // If the current character is a newline
      else if( this->text_buffer[show] == '\t' )
      {
        x_offset += this->spacing * 4;
      }
      else
      {
        //Get the ASCII value of the character
        uint8_t ascii = static_cast<u_char>( this->text_buffer[show] );

        this->bitmap_font.setPosition ( nom::Coords ( x_offset, y_offset ) );
        this->bitmap_font.setOffsets ( this->chars[ascii] );
        this->bitmap_font.Draw ( video_buffer );

        // Move over the width of the character with one pixel of padding
        x_offset += ( this->chars[ascii].width ) + 1;
      } // end else
    } // end for loop
  } // end if this->bitmap_font != nullptr
}
