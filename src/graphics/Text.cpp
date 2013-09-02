/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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

******************************************************************************/
#include "nomlib/graphics/Text.hpp"

namespace nom {

Text::Text ( void )
{
NOM_LOG_TRACE;

  this->font.reset();
  this->file_type = Unknown;
}

Text::~Text ( void )
{
NOM_LOG_TRACE;
}

bool Text::load ( const std::string& filename, bool use_cache )
{
  File file;
  std::string extension = "\0";

  extension = file.mime ( filename );

  // If we still have a null terminated string here, it generally means we had
  // failed loading the libmagic mime database. Of course, there's the fringe
  // chance perhaps that libmagic could not be initialized for some bizarre
  // reason!
  if ( extension == "\0" )
    return false;

  // If we find that the file MIME type is not TTF, we first will try loading
  // the input file as a bitmap font
  if ( extension != "application/x-font-ttf" )
  {
    this->font = std::unique_ptr<IFont> ( new BitmapFont() );
    this->font->load ( filename, Color ( 110, 144, 190 ), use_cache );

    if ( this->font != nullptr )
    {
      this->setFontType ( FontType::Bitmap );

      return true;
    }
  }
  else // Try as a TrueType font
  {
    this->font = std::unique_ptr<IFont> ( new TrueTypeFont() );

    this->font->load ( filename, Color::Black, use_cache );

    if ( this->font != nullptr )
    {
      this->setFontType ( FontType::TrueType );

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

const std::string Text::getText ( void ) const
{
  if ( this->font )
    return this->font->getText();
  else
    return "\0";
}

int32 Text::getFontWidth ( void ) const
{
  if ( this->font )
    return this->font->getFontWidth();
  else
    return -1;
}

int32 Text::getFontHeight ( void ) const
{
  if ( this->font )
    return this->font->getFontHeight();
  else
    return -1;
}

FontStyle Text::getFontStyle ( void ) const
{
  if ( this->font )
    return this->font->getFontStyle();
  else
    return FontStyle::Regular; // FIXME; should be Unknown or such
}

const Color& Text::getColor ( void ) const
{
  if ( this->font )
    return this->font->getColor();
  else
    return nom::Color::Black; // FIXME
}

const Coords Text::getPosition ( void ) const
{
  if ( this->font )
    return this->font->getPosition();
  else
    return nom::Coords ( 0, 0 ); // FIXME?
}

void Text::setText ( const std::string& text )
{
  if ( this->font )
    this->font->setText ( text );
}

void Text::setColor ( const Color& color )
{
  if ( this->font )
    this->font->setColor ( color );
}

void Text::setPosition ( const Coords& coords )
{
  if ( this->font )
    this->font->setPosition ( coords );
}

void Text::setFontSize ( int32 size )
{
  if ( this->font )
    this->font->setFontSize( size );
}

void Text::setFontStyle ( uint8 style, uint8 options )
{
  if ( this->font )
    this->font->setFontStyle ( style, options );
}

void Text::Update ( void )
{
  if ( this->font )
    this->font->Update();
}

void Text::Draw ( void* video_buffer ) const
{
  if ( this->font )
    this->font->Draw ( video_buffer );
}

bool Text::resize ( enum ResizeAlgorithm scaling_algorithm )
{
  if ( ! this->font )
  {
NOM_LOG_ERR ( "Text font is invalid." );
    return false;
  }

  if ( this->font->resize( scaling_algorithm ) == false ) return false;

  return true;
}


} // namespace nom
