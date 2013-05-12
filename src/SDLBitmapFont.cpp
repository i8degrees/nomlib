/******************************************************************************
    SDLBitmapFont.cpp

  SDL-based Bitmap Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

  Portions Copyright Lazy Foo' Productions 2004-2013
  All rights reserved.

******************************************************************************/
#include "SDLBitmapFont.h"

SDLBitmapFont::SDLBitmapFont ( void )
{
  #ifdef DEBUG_BITMAP_FONT_OBJ
    std::cout << "SDLBitmapFont::SDLBitmapFont (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->bitmap_font = NULL;

  this->text_buffer = "\0";
  this->newline = 0; // holds the y coords value to increment upon newline break char
  this->spacing = 0; // holds the x coords value to increment upon space char
}

SDLBitmapFont::~SDLBitmapFont ( void )
{
  #ifdef DEBUG_BITMAP_FONT_OBJ
    std::cout << "SDLBitmapFont::~SDLBitmapFont (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->bitmap_font != NULL )
  {
    SDL_FreeSurface ( this->bitmap_font );
    this->bitmap_font = NULL;
  }
}

unsigned int SDLBitmapFont::getTextWidth ( void )
{
  unsigned int text_width = 0;

  for ( int t = 0; t < this->text_buffer.length(); t++ )
  {
    if ( this->text_buffer[t] == ' ' )
      text_width += chars[t].w / 2;
    else if ( this->text_buffer[t] == '\n' )
      text_width = 0;
    else
      text_width += chars[t].w - this->spacing - 2;
  }

  return text_width;
}

unsigned int SDLBitmapFont::getTextHeight ( void )
{
  unsigned int text_height = 0;

  for ( int t = 0; t < this->text_buffer.length(); t++ )
  {
    if ( this->text_buffer[t] == '\n' )
      text_height += this->newline;
    else
      text_height = chars[t].h;
  }

  return text_height;
}

std::string SDLBitmapFont::getTextBuffer ( void )
{
  return this->text_buffer;
}

void SDLBitmapFont::setTextBuffer ( std::string text )
{
  this->text_buffer = text;
}

unsigned int SDLBitmapFont::getSpacing ( void )
{
  return this->spacing;
}

// FIXME: Should be set after the LoadImage call is made
void SDLBitmapFont::setSpacing ( unsigned int spaces )
{
  this->spacing = spaces;
}

// Reference: http://lazyfoo.net/SDL_tutorials/lesson30/index.php
// TODO: add spacing / padding so that we can export with black guidelines
bool SDLBitmapFont::LoadImage ( std::string filename, unsigned int sheet_width, unsigned int sheet_height, unsigned int r, unsigned int g, unsigned int b )
{
  unsigned int tile_width = 0;
  unsigned int tile_height = 0;
  unsigned int top = 0;
  unsigned int baseA = 0;
  unsigned int currentChar = 0;
  unsigned int background_color = 0;

  this->bitmap_font = Gfx::LoadImage ( filename, r, g, b );

  if ( this->bitmap_font == NULL )
  {
    #ifdef DEBUG_BITMAP_FONT
      std::cout << "ERR in SDLBitmapFont::LoadImage() at Gfx::LoadImage(): " << std::endl;
    #endif
    return false;
  }

  background_color = SDL_MapRGB ( this->bitmap_font->format, r, g, b );

  tile_width = this->bitmap_font->w / sheet_width;
  tile_height = this->bitmap_font->h / sheet_height;
  top = tile_height;
  baseA = tile_height;

  for ( int rows = 0; rows < sheet_width; rows++ )
  {
    for ( int cols = 0; cols < sheet_height; cols++ )
    {
      // Set character offsets
      chars[ currentChar ].x = tile_width * cols;
      chars[ currentChar ].y = tile_height * rows;

      chars[ currentChar ].w = tile_width;
      chars[ currentChar ].h = tile_height;

      //Find Left Side; go through pixel columns
      for ( int pCol = 0; pCol < tile_width; pCol++ )
      {
        //Go through pixel rows
        for ( int pRow = 0; pRow < tile_height; pRow++ )
        {
          //Get the pixel offsets
          int pX = ( tile_width * cols ) + pCol;
          int pY = ( tile_height * rows ) + pRow;

          //If a non colorkey pixel is found
          if( Gfx::getPixel ( this->bitmap_font, pX, pY ) != background_color )
          {
              //Set the x offset
              chars[ currentChar ].x = pX;

              //Break the loops
              pCol = tile_width;
              pRow = tile_height;
          }
        }
      }

      //Find Right Side; go through pixel columns
      for ( int pCol_w = tile_width - 1; pCol_w >= 0; pCol_w-- )
      {
        //Go through pixel rows
        for ( int pRow_w = 0; pRow_w < tile_height; pRow_w++ )
        {
          //Get the pixel offsets
          unsigned int pX = ( tile_width * cols ) + pCol_w;
          unsigned int pY = ( tile_height * rows ) + pRow_w;

          //If a non colorkey pixel is found
          if ( Gfx::getPixel ( this->bitmap_font, pX, pY ) != background_color )
          {
            //Set the width
            chars[ currentChar ].w = ( pX - chars[ currentChar ].x ) + 1;

            //Break the loops
            pCol_w = -1;
            pRow_w = tile_height;
          }
        }
      }

      //Find Top; go through pixel rows
      for ( int pRow = 0; pRow < tile_height; pRow++ )
      {
        //Go through pixel columns
        for ( int pCol = 0; pCol < tile_width; pCol++ )
        {
          //Get the pixel offsets
          unsigned int pX = ( tile_width * cols ) + pCol;
          unsigned int pY = ( tile_height * rows ) + pRow;

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
        for ( int pRow = tile_height - 1; pRow >= 0; pRow-- )
        {
          // Go through pixel columns
          for ( int pCol = 0; pCol < tile_width; pCol++ )
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

  for ( int t = 0; t < 256; t++ )
  {
    chars[ t ].y += top;
    chars[ t ].h -= top;
  }

  return true;
}

// Reference: http://lazyfoo.net/SDL_tutorials/lesson30/index.php
bool SDLBitmapFont::DrawText ( Gfx *engine, unsigned int x, unsigned int y )
{
  //Temp offsets
  unsigned int x_offset = x, y_offset = y;

  //If the font has been built
  if ( this->bitmap_font != NULL )
  {
    for ( int show = 0; show < this->text_buffer.length(); show++ )
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
        //Move down
        y_offset += this->newline;

        //Move back
        x_offset = x;
      }
      else
      {
        //Get the ASCII value of the character
        unsigned int ascii = (unsigned char)this->text_buffer[show];

        if ( engine->DrawSurface ( this->bitmap_font, x_offset, y_offset, chars[ascii].x, chars[ascii].y, chars[ascii].w, chars[ascii].h ) == false )
        {
          std::cout << "ERR in SDLBitmapFont::DrawText(): " << SDL_GetError() << std::endl;

          SDL_FreeSurface ( this->bitmap_font );
          this->bitmap_font = NULL;
          return false;
        }

        // Move over the width of the character with one pixel of padding
        x_offset += chars[ascii].w + 1;
      } // end else
    } // end for loop
  } // end if this->bitmap_font != NULL

  return true;
}
