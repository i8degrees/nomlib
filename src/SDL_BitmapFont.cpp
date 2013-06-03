/******************************************************************************
    SDL_BitmapFont.cpp

  SDL-based Bitmap Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

  Portions Copyright Lazy Foo' Productions 2004-2013
  All rights reserved.

******************************************************************************/
#include "SDL_BitmapFont.h"

SDLBitmapFont::SDLBitmapFont ( void )
{
  #ifdef DEBUG_BITMAP_FONT_OBJ
    std::cout << "SDLBitmapFont::SDLBitmapFont (): " << "Hello, world!" << "\n" << std::endl;
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

signed int SDLBitmapFont::getX ( void )
{
  return this->coords.getX();
}

signed int SDLBitmapFont::getY ( void )
{
  return this->coords.getY();
}

nom::Coords SDLBitmapFont::getXY ( void )
{
  return this->coords;
}

void SDLBitmapFont::setX ( signed int x_ )
{
  this->coords.setX ( x_ );
}

void SDLBitmapFont::setY ( signed int y_ )
{
  this->coords.setY ( y_ );
}

void SDLBitmapFont::setXY ( signed int x_, signed int y_ )
{
  this->coords.setXY ( x_, y_ );
}

void SDLBitmapFont::greyedOutText ( unsigned char opacity )
{
  if ( this->bitmap_font )
    Gfx::setAlpha ( this->bitmap_font, ( unsigned int ) opacity );
}

// I don't think this is entirely accurate; this->spacing - 2 is fudged ...
// We probably ought to be calculating the width based off the same algorithm as
// is shown in SDLBitmapFont::LoadImage
signed int SDLBitmapFont::getTextWidth ( void )
{
  unsigned int text_width = 0;

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

signed int SDLBitmapFont::getTextHeight ( void )
{
  unsigned int text_height = 0;

  for ( int t = 0; t < this->text_buffer.length(); t++ )
  {
    if ( this->text_buffer[t] == '\n' )
      text_height += this->newline;
    else
      text_height = this->chars[t].getHeight();
  }

  return text_height;
}

std::string SDLBitmapFont::getText ( void )
{
  return this->text_buffer;
}

void SDLBitmapFont::setText ( std::string text )
{
  this->text_buffer = text;
}

unsigned int SDLBitmapFont::getSpacing ( void )
{
  return this->spacing;
}

void SDLBitmapFont::setSpacing ( unsigned int spaces )
{
  this->spacing = spaces;
}

unsigned int SDLBitmapFont::getNewline ( void )
{
  return this->newline;
}

// Needs testing; not sure if this even does anything currently
void SDLBitmapFont::setNewline ( unsigned int newline )
{
  this->newline = newline;
}

// Reference: http://lazyfoo.net/SDL_tutorials/lesson30/index.php
// TODO: add spacing / padding so that we can export with black guidelines
bool SDLBitmapFont::Load ( std::string filename, nom::Color colorkey, unsigned int sheet_width, unsigned int sheet_height )
{
  unsigned int tile_width = 0;
  unsigned int tile_height = 0;
  unsigned int top = 0;
  unsigned int baseA = 0;
  unsigned int currentChar = 0;
  unsigned int background_color = 0;

  this->bitmap_font = Gfx::LoadImage ( filename, colorkey );

  if ( this->bitmap_font == NULL )
  {
    #ifdef DEBUG_BITMAP_FONT
      std::cout << "ERR in SDLBitmapFont::LoadImage() at Gfx::LoadImage(): " << std::endl;
    #endif
    return false;
  }

  background_color = GColor::mapRGB ( this->bitmap_font->format, colorkey );

  tile_width = this->bitmap_font->w / sheet_width;
  tile_height = this->bitmap_font->h / sheet_height;
  top = tile_height;
  baseA = tile_height;

  for ( int rows = 0; rows < sheet_width; rows++ )
  {
    for ( int cols = 0; cols < sheet_height; cols++ )
    {
      // Set character offsets
      this->chars[ currentChar ].setXY ( tile_width * cols, tile_height * rows );
      this->chars[ currentChar ].setDimensions ( tile_width, tile_height );

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
              this->chars[ currentChar ].setX ( pX );

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
            unsigned int width = ( pX - this->chars[ currentChar ].getX() ) + 1;
            this->chars[ currentChar ].setWidth ( width );

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
    signed int y = this->chars[ t ].getY();
    signed int height = this->chars[ t ].getHeight();

    this->chars[ t ].setY ( y += top );
    this->chars[ t ].setHeight ( height -= top );
  }

  return true;
}

// Reference: http://lazyfoo.net/SDL_tutorials/lesson30/index.php
bool SDLBitmapFont::Draw ( SDL_Surface *video_buffer )
{
  //If the font has been built
  if ( this->bitmap_font != NULL )
  {
    for ( int show = 0; show < this->text_buffer.length(); show++ )
    {
      //If the current character is a space
      if ( this->text_buffer[show] == ' ' )
      {
        //Move over
        this->coords.updateXY ( this->spacing, 0 );
      }
      // If the current character is a newline
      else if( this->text_buffer[show] == '\n' )
      {
        //Move down and back over to the beginning of line
        this->coords.updateXY ( this->coords.getX(), this->newline );
      }
      else
      {
        //Get the ASCII value of the character
        unsigned int ascii = (unsigned char)this->text_buffer[show];

        if ( Gfx::DrawSurface ( this->bitmap_font, video_buffer, this->coords, this->chars[ascii] ) == false )
        {
          std::cout << "ERR in SDLBitmapFont::DrawText(): " << SDL_GetError() << std::endl;

          SDL_FreeSurface ( this->bitmap_font );
          this->bitmap_font = NULL;
          return false;
        }

        // Move over the width of the character with one pixel of padding
        this->coords.updateXY ( ( this->chars[ascii].getWidth() ) + 1, 0 );
      } // end else
    } // end for loop
  } // end if this->bitmap_font != NULL

  return true;
}
