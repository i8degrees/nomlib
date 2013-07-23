/******************************************************************************

    Unified text output interface; designed for wrapping Bitmap & TrueType
  font objects to co-exist under one roof.

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/gfx/Text.hpp"

namespace nom {

Text::Text ( void )
{
NOMLIB_LOG_INFO;

  this->font.reset();
  this->file_type = Unknown;
}

Text::~Text ( void )
{
NOMLIB_LOG_INFO;

  this->font.reset();
}

bool Text::Load ( const std::string& filename, int32 font_size )
{
  OSXFS file;
  std::string extension = "\0";

  extension = file.getFileType ( filename );

  // If we find that the file MIME type is not TTF, we first will try loading
  // the input file as a bitmap font
  if ( extension.compare ( "application/x-font-ttf" ) != 0 )
  {
    this->font = std::shared_ptr<IFont> ( new SDL_BitmapFont );
    this->font->Load ( filename, nom::Color ( 110, 144, 190 ), font_size, true );

    if ( this->font != nullptr )
    {
      this->setFontType ( FontType::BitmapFont );

      return true;
    }
  }
  else // Try as a TrueType font
  {
    this->font = std::shared_ptr<IFont> ( new SDL_Font );

    this->font->Load ( filename, nom::Color::Black, font_size, false );

    if ( this->font != nullptr )
    {
      this->setFontType ( FontType::TrueTypeFont );

      return true;
    }
  }

  // If we reach this point, neither a TrueType or a bitmap font was found to be
  // usable
  this->setFontType ( FontType::Unknown );

  return false;
}

FontType Text::getFontType ( void )
{
  return this->file_type;
}

void Text::setFontType ( enum FontType type )
{
  this->file_type = type;
}

const std::string& Text::getText ( void ) const
{
  return this->font->getText();
}

int32 Text::getFontWidth ( void ) const
{
  return this->font->getFontWidth();
}

int32 Text::getFontHeight ( void ) const
{
  return this->font->getFontHeight();
}

FontStyle Text::getFontStyle ( void ) const
{
  return this->font->getFontStyle();
}

const Color Text::getColor ( void ) const
{
  return this->font->getColor();
}

const Coords Text::getPosition ( void ) const
{
  return this->font->getPosition();
}

void Text::setText ( const std::string& text )
{
  this->font->setText ( text );
}

void Text::setColor ( const Color& color )
{
  this->font->setColor ( color );
}

void Text::setPosition ( const Coords& coords )
{
  this->font->setPosition ( coords );
}

void Text::setFontSize ( int32 size )
{
  this->font->setFontSize( size );
}

void Text::setFontStyle ( uint8 style, uint8 options )
{
  this->font->setFontStyle ( style, options );
}

void Text::Update ( void )
{
  if ( this->font )
    this->font->Update();
}

void Text::Draw ( void* video_buffer )
{
  if ( this->font )
    this->font->Draw ( video_buffer );
}


} // namespace nom
