/******************************************************************************
    SDL_BitmapFont.cpp

  SDL-based Bitmap Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

  Portions Copyright Lazy Foo' Productions 2004-2013
  All rights reserved.

  1. http://lazyfoo.net/SDL_tutorials/lesson30/index.php

******************************************************************************/
#include "SDL_BitmapFont.h"

using namespace nom;

SDL_BitmapFont::SDL_BitmapFont ( void )
{
  #ifdef DEBUG_BITMAP_FONT_OBJ
    std::cout << "SDL_BitmapFont::SDL_BitmapFont (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->bitmap_font = NULL;

  this->text_buffer = "\0";
  this->newline = 0; // holds the y coords value to increment upon newline break char
  this->spacing = 0; // holds the x coords value to increment upon space char

  this->coords.setCoords ( 0, 0, 0, 0 );

  for ( unsigned int idx = 0; idx < 256; idx++ )
  {
    this->chars[idx].setCoords ( 0, 0, 0, 0 );
  }
}

SDL_BitmapFont::~SDL_BitmapFont ( void )
{
  #ifdef DEBUG_BITMAP_FONT_OBJ
    std::cout << "SDL_BitmapFont::~SDL_BitmapFont (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->bitmap_font != NULL )
  {
    SDL_FreeSurface ( this->bitmap_font );
    this->bitmap_font = NULL;
  }
}

const int32_t SDL_BitmapFont::getX ( void ) const
{
  return this->coords.getX();
}

const int32_t SDL_BitmapFont::getY ( void ) const
{
  return this->coords.getY();
}

const Coords& SDL_BitmapFont::getXY ( void ) const
{
  return this->coords;
}

void SDL_BitmapFont::setX ( int32_t x_ )
{
  this->coords.setX ( x_ );
}

void SDL_BitmapFont::setY ( int32_t y_ )
{
  this->coords.setY ( y_ );
}

void SDL_BitmapFont::setXY ( int32_t x_, int32_t y_ )
{
  this->coords.setXY ( x_, y_ );
}

void SDL_BitmapFont::greyedOutText ( u_char opacity )
{
  if ( this->bitmap_font )
    Gfx::setAlpha ( this->bitmap_font, ( uint32_t ) opacity );
}

// I don't think this is entirely accurate; this->spacing - 2 is fudged ...
// We probably ought to be calculating the width based off the same algorithm as
// is shown in SDL_BitmapFont::LoadImage
int32_t SDL_BitmapFont::getTextWidth ( void )
{
  int32_t text_width = 0;

  if ( this->bitmap_font != NULL )
  {
    for ( int t = 0; t < this->text_buffer.length(); t++ )
    {
      if ( this->text_buffer[t] == ' ' )
        text_width += this->chars[t].getWidth() / this->spacing;
      else if ( this->text_buffer[t] == '\n' )
        text_width = 0;
      else
        text_width += this->chars[t].getWidth() - ( this->spacing + 2 );
    }
  }

  return text_width;
}

int32_t SDL_BitmapFont::getTextHeight ( void )
{
  int32_t text_height = 0;

  for ( int t = 0; t < this->text_buffer.length(); t++ )
  {
    if ( this->text_buffer[t] == '\n' )
      text_height += this->newline;
    else
      text_height = this->chars[t].getHeight();
  }

  return text_height;
}

const std::string& SDL_BitmapFont::getText ( void ) const
{
  return this->text_buffer;
}

void SDL_BitmapFont::setText ( std::string text )
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

// Needs testing; not sure if this even does anything currently
void SDL_BitmapFont::setNewline ( uint32_t newline )
{
  this->newline = newline;
}

// TODO: add spacing / padding so that we can export with black guidelines
bool SDL_BitmapFont::Load ( std::string filename, const nom::Color& colorkey, uint32_t sheet_width, uint32_t sheet_height )
{
  uint32_t tile_width = 0;
  uint32_t tile_height = 0;
  uint32_t top = 0;
  uint32_t baseA = 0;
  uint32_t currentChar = 0;
  int32_t background_color = 0;

  this->bitmap_font = (SDL_Surface*) Gfx::LoadImage ( filename, colorkey );

  if ( this->bitmap_font == NULL )
  {
    #ifdef DEBUG_BITMAP_FONT
      std::cout << "ERR in SDL_BitmapFont::LoadImage() at Gfx::LoadImage(): " << std::endl;
    #endif
    return false;
  }

  background_color = colorkey.getColorAsInt ( this->bitmap_font->format );

  tile_width = this->bitmap_font->w / sheet_width;
  tile_height = this->bitmap_font->h / sheet_height;
  top = tile_height;
  baseA = tile_height;

  for ( uint32_t rows = 0; rows < sheet_width; rows++ )
  {
    for ( uint32_t cols = 0; cols < sheet_height; cols++ )
    {
      // Set character offsets
      this->chars[ currentChar ].setXY ( tile_width * cols, tile_height * rows );
      this->chars[ currentChar ].setDimensions ( tile_width, tile_height );

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
          if( Gfx::getPixel ( this->bitmap_font, pX, pY ) != background_color )
          {
              //Set the x offset
              this->chars[ currentChar ].setX ( pX );

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
        for ( int32_t pRow_w = 0; pRow_w < tile_height; pRow_w++ )
        {
          //Get the pixel offsets
          uint32_t pX = ( tile_width * cols ) + pCol_w;
          uint32_t pY = ( tile_height * rows ) + pRow_w;

          //If a non colorkey pixel is found
          if ( Gfx::getPixel ( this->bitmap_font, pX, pY ) != background_color )
          {
            //Set the width
            uint32_t width = ( pX - this->chars[ currentChar ].getX() ) + 1;
            this->chars[ currentChar ].setWidth ( width );

            //Break the loops
            pCol_w = -1;
            pRow_w = tile_height;
          }
        }
      }

      //Find Top; go through pixel rows
      for ( int32_t pRow = 0; pRow < tile_height; pRow++ )
      {
        //Go through pixel columns
        for ( int32_t pCol = 0; pCol < tile_width; pCol++ )
        {
          //Get the pixel offsets
          uint32_t pX = ( tile_width * cols ) + pCol;
          uint32_t pY = ( tile_height * rows ) + pRow;

          // If a non colorkey pixel is found
          if( Gfx::getPixel ( this->bitmap_font, pX, pY ) != background_color )
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

      // Find Bottom of A
      if ( currentChar == 'A' )
      {
        // Go through pixel rows
        for ( int32_t pRow = tile_height - 1; pRow >= 0; pRow-- )
        {
          // Go through pixel columns
          for ( int32_t pCol = 0; pCol < tile_width; pCol++ )
          {
            // Get the pixel offsets
            unsigned int pX = ( tile_width * cols ) + pCol;
            unsigned int pY = ( tile_height * rows ) + pRow;

            // If a non colorkey pixel is found
            if ( Gfx::getPixel ( this->bitmap_font, pX, pY ) != background_color )
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
    int32_t y = this->chars[ t ].getY();
    int32_t height = this->chars[ t ].getHeight();

    this->chars[ t ].setY ( y += top );
    this->chars[ t ].setHeight ( height -= top );
  }

  return true;
}

void SDL_BitmapFont::Draw ( void* video_buffer ) const
{
  //  Use coordinates provided by interface user as our starting origin
  //  coordinates to compute from
  int32_t x_offset = this->coords.getX();
  int32_t y_offset = this->coords.getY();

  //If the font has been built
  if ( this->bitmap_font != NULL )
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
        x_offset = this->coords.getX();
      }
      else
      {
        //Get the ASCII value of the character
        uint32_t ascii = ( u_char ) this->text_buffer[show];

        if ( Gfx::DrawSurface ( this->bitmap_font, (SDL_Surface*) video_buffer, Coords ( x_offset, y_offset ), this->chars[ascii] ) == false )
        {
          std::cout << "ERR in SDL_BitmapFont::DrawText(): " << SDL_GetError() << std::endl;

          SDL_FreeSurface ( this->bitmap_font );
        }

        // Move over the width of the character with one pixel of padding
        x_offset += ( this->chars[ascii].getWidth() ) + 1;
      } // end else
    } // end for loop
  } // end if this->bitmap_font != NULL
}
