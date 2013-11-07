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
#include "nomlib/graphics/TrueTypeFont.hpp"

namespace nom {

TrueTypeFont::TrueTypeFont ( void )
{
NOM_LOG_TRACE ( NOM );

  this->font = nullptr;
  this->coords = Coords ( 0, 0, 0, 0 );
  this->color = Color ( 0, 0, 0 );
  this->text_buffer = "\0";
  this->text_style = FontStyle::Regular; // default text styling effect
  this->text_alignment = TextAlignment::MiddleLeft;
  this->rendering = RenderStyle::Solid; // Fast, but ugly
  this->style_options = 0;
  this->filename = "\0";
  this->font_size = 12;
  this->use_cache = false;

  if ( TTF_Init () == -1 )
  {
NOM_LOG_ERR ( NOM, TTF_GetError() );
  }
}

TrueTypeFont::~TrueTypeFont ( void )
{
NOM_LOG_TRACE ( NOM );

  // Possible bug -- a segmentation fault occurs here if we do not reset the
  // smart pointer on destruction.
  this->font.reset();

  TTF_Quit();
}

IFont::SharedPtr TrueTypeFont::clone ( void ) const
{
  return IFont::SharedPtr ( new TrueTypeFont ( *this ), priv::Free_TrueTypeFont );
}

bool TrueTypeFont::valid ( void ) const
{
  if ( this->font.get() != nullptr )
  {
    return true;
  }
  else
  {
    return false;
  }
}

const std::string& TrueTypeFont::getText ( void ) const
{
  return this->text_buffer;
}

int32 TrueTypeFont::getFontWidth ( void ) const
{
  return this->coords.width;
}

int32 TrueTypeFont::getFontHeight ( void ) const
{
  return this->coords.height;
}

IFont::FontStyle TrueTypeFont::getFontStyle ( void ) const
{
  return this->text_style;
}

const Color& TrueTypeFont::getColor ( void ) const
{
  return this->color;
}

const Coords& TrueTypeFont::getPosition ( void ) const
{
  return this->coords;
}

uint32 TrueTypeFont::getNewline ( void ) const
{
  // Not implemented
  return 0;
}

uint32 TrueTypeFont::getSpacing ( void ) const
{
  // Not implemented
  return 0;
}

IFont::TextAlignment TrueTypeFont::getTextJustification ( void ) const
{
  return this->text_alignment;
}

void TrueTypeFont::setColor ( const Color& color )
{
  this->color = color;
}

void TrueTypeFont::setPosition ( const Coords& coords )
{
  this->coords.x = coords.x;
  this->coords.y = coords.y;
}

void TrueTypeFont::setFontSize ( int32 point_size )
{
  int32 original_font_size = this->font_size;

  this->font_size = point_size;

  if ( this->rebuild() == false )
  {
NOM_LOG_ERR ( NOM, "Could not set new font size." );
    this->font_size = original_font_size;
    return;
  }
}

void TrueTypeFont::setFontStyle ( int32 style, uint8 options )
{
  switch ( style )
  {
    default: break;
    case FontStyle::Regular:
    {
      TTF_SetFontStyle ( this->font.get(), TTF_STYLE_NORMAL );
    }
    break;

    case FontStyle::Bold:
    {
      TTF_SetFontStyle ( this->font.get(), TTF_STYLE_BOLD );
    }
    break;

    case FontStyle::Italic:
    {
      TTF_SetFontStyle ( this->font.get(), TTF_STYLE_ITALIC );
    }
    break;

    case FontStyle::Underlined:
    {
      TTF_SetFontStyle ( this->font.get(), TTF_STYLE_UNDERLINE );
    }
    break;

    /// Text effect utilizing alpha channels for the appearance of gray text
    case FontStyle::Faded:
    {
      this->text_style = FontStyle::Faded;
      this->style_options = options;
    break;
    }
  }
}

int32 TrueTypeFont::getFontOutline ( void ) const
{
  return TTF_GetFontOutline ( this->font.get() );
}

void TrueTypeFont::setFontOutline ( int32 depth )
{
  TTF_SetFontOutline ( this->font.get(), depth );
}

IFont::RenderStyle TrueTypeFont::getRenderingStyle ( void ) const
{
  return this->rendering;
}

void TrueTypeFont::setRenderingStyle ( IFont::RenderStyle style )
{
  this->rendering = style;
}

void TrueTypeFont::setText ( const std::string& text )
{
  if ( text.length() < 1 ) return;

  this->text_buffer = text;

  // Update the width & height of text string, if we can
  if ( TTF_SizeText ( this->font.get(), this->text_buffer.c_str(), &this->coords.width, &this->coords.height ) == -1 )
  {
NOM_LOG_ERR ( NOM, "Failed to set font width & height." );
  }
}

void TrueTypeFont::setSpacing ( uint32 spaces )
{
  // Not implemented
}

void TrueTypeFont::setTextJustification ( IFont::TextAlignment alignment )
{
  this->text_alignment = alignment;
}

bool TrueTypeFont::load ( const std::string& filename, const Color& colorkey,
                          bool use_cache
                        )
{
  this->font = std::shared_ptr<TTF_Font> ( TTF_OpenFont ( filename.c_str(), this->font_size ), nom::priv::TTF_FreeFont );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not load TTF file: " + filename );
    return false;
  }

  // Store the new filename & caching choice for future reference; primarily
  // used when rebuilding font metrics, such as when we change the font point
  // size or load a new font.
  this->filename = filename;
  this->use_cache = use_cache;

  return true;
}

void TrueTypeFont::update ( void )
{
  // Update display coordinates
  this->font_buffer.set_position ( Point2i ( this->coords.x, this->coords.y ) );

  // Update the rendered text surface here for drawing
  if ( this->getText().c_str() == nullptr ) return;

  if ( this->rendering == RenderStyle::Shaded ) // Moderate-quality
  {
    this->font_buffer.initialize ( TTF_RenderText_Shaded
                                  (
                                    this->font.get(),
                                    this->getText().c_str(),
                                    SDL_COLOR(this->color),
                                    // TODO; implement me -- a second color
                                    // possibility means needing two colors in
                                    // class
                                    SDL_COLOR ( Color ( 97, 97, 97 ) )
                                  )
                                );
  }
  else if ( this->rendering == RenderStyle::Blended ) // Highest-quality
  {
    this->font_buffer.initialize ( TTF_RenderText_Blended
                                  (
                                    this->font.get(),
                                    this->getText().c_str(),
                                    SDL_COLOR(this->color)
                                  )
                                );
  }
  else // Low-quality rendering (the default)
  {
    this->font_buffer.initialize ( TTF_RenderText_Solid
                                  (
                                    this->font.get(),
                                    this->getText().c_str(),
                                    SDL_COLOR(this->color)
                                  )
                                );
  }

  if ( this->text_style == FontStyle::Faded )
  {
    this->font_buffer.set_alpha ( this->style_options );
  }
}

void TrueTypeFont::draw ( SDL_Renderer* target ) const
{
  if ( this->font_buffer.valid() )
  {
    this->font_buffer.draw ( target );
  }
}

void TrueTypeFont::draw ( const Window& target ) const
{
  if ( this->font_buffer.valid() )
  {
    this->font_buffer.draw ( target.renderer() );
  }
}

bool TrueTypeFont::rebuild ( void )
{
  if ( this->load ( this->filename, Color::Black, this->use_cache ) == false )
  {
NOM_LOG_ERR ( NOM, "Could not rebuild font metrics." );
    return false;
  }

  return true;
}

namespace priv {

void Free_TrueTypeFont ( TrueTypeFont* ptr )
{
  // Do nothing custom deleter
  //
  // FIXME; this is a known bug (memory leak).
}

} // namespace priv
} // namespace nom

