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

TrueTypeFont::TrueTypeFont ( void ) :
  sheet_width_ ( 16 ),  // Arbitrary; based on nom::BitmapFont
  sheet_height_ ( 16 ), // Arbitrary; based on nom::BitmapFont
  font_size_ ( 14 ), // Terrible Eyesight (TM)
  newline_ ( 0 ),
  spacing_ ( 0 ),
  use_cache_ ( false ),
  type_ ( IFont::FontType::TrueTypeFont )
{
  NOM_LOG_TRACE ( NOM );
}

TrueTypeFont::~TrueTypeFont ( void )
{
  NOM_LOG_TRACE ( NOM );
}

TrueTypeFont::TrueTypeFont ( const TrueTypeFont& copy )
{
  //this->font_buffer_ = copy.font_buffer_;
  this->font_ = copy.font_;
  this->pages_ = copy.pages();
  this->type_ = copy.type();
  this->font_size_ = copy.font_size();
  this->newline_ = copy.newline();
  this->spacing_ = copy.spacing();
  this->filename_ = copy.filename_;
  this->use_cache_ = copy.use_cache_;
}

IFont::SharedPtr TrueTypeFont::clone ( void ) const
{
  return TrueTypeFont::SharedPtr ( new TrueTypeFont ( *this ) );
}

bool TrueTypeFont::valid ( void ) const
{
  if ( this->font_.get() != nullptr ) return true;

  return false;
}

enum IFont::FontType TrueTypeFont::type ( void ) const
{
  return this->type_;
}

SDL_SURFACE::RawPtr TrueTypeFont::image ( void ) const
{
  return this->pages_[0].texture->image();
}

uint TrueTypeFont::spacing ( void ) const
{
  return this->spacing_;
}

int32 TrueTypeFont::font_size ( void ) const
{
  return this->font_size_;
}

void TrueTypeFont::set_spacing ( uint spaces )
{
  this->spacing_ = spaces;
}

uint TrueTypeFont::newline ( void ) const
{
  return this->newline_;
}

void TrueTypeFont::set_newline ( uint newline )
{
  this->newline_ = newline;
}

/*
void TrueTypeFont::set_font_size ( sint point_size )
{
  sint original_font_size = this->font_size_;

  this->font_size_ = point_size;

  if ( this->rebuild() == false )
  {
NOM_LOG_ERR ( NOM, "Could not set new font size." );
    this->font_size_ = original_font_size;
    return;
  }
}
*/

/*
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
*/

/*
int32 TrueTypeFont::getFontOutline ( void ) const
{
  return TTF_GetFontOutline ( this->font.get() );
}

void TrueTypeFont::setFontOutline ( int32 depth )
{
  TTF_SetFontOutline ( this->font.get(), depth );
}
*/

/*
void TrueTypeFont::setText ( const std::string& text )
{
  if ( text.length() < 1 ) return;

  this->text_buffer = text;

  // Font is not valid -- TTF_SizeText will crash if we go further
  if ( ! this->valid() ) return;

  // Update the width & height of text string, if we can
  if ( TTF_SizeText ( this->font.get(), this->text_buffer.c_str(), &this->coords.width, &this->coords.height ) == -1 )
  {
NOM_LOG_ERR ( NOM, "Failed to set font width & height." );
  }
}
*/

const Glyph& TrueTypeFont::glyph ( uint32 codepoint, uint32 character_size ) const
{
  GlyphAtlas& glyphs = this->pages_[character_size].glyphs;

  return glyphs[codepoint];
/* TODO
  GlyphAtlas::const_iterator it = glyphs.find(codepoint);

  if ( it != glyphs.end() )
  {
    return it->second;
  }
  else
  {
    // FIXME: implement support for handling this condition
    return Glyph();
  }
*/
}

bool TrueTypeFont::load ( const std::string& filename, const Color4u& colorkey,
                          bool use_cache
                        )
{
  this->font_ = std::shared_ptr<TTF_Font> ( TTF_OpenFont ( filename.c_str(), this->font_size() ), priv::TTF_FreeFont );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not load TTF file: " + filename );
    return false;
  }

  // Store the new filename & caching choice for future reference; primarily
  // used when rebuilding font metrics, such as when we change the font point
  // size or load a new font.
  this->filename_ = filename;
  this->use_cache_ = use_cache;

  // Attempt to build font metrics
  if ( this->build() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not build TrueType font metrics" );
    return false;
  }

  return true;
}

/*
void TrueTypeFont::update ( void )
{
  // Font is not valid -- nothing to draw
  if ( this->valid() == false ) return;

  // No text string set -- nothing to draw
  if ( this->getText().length() < 1 ) return;

  // Update display coordinates
  this->font_buffer_.set_position ( Point2i ( this->coords.x, this->coords.y ) );

  // Update the rendered text surface here for drawing
  if ( this->rendering == RenderStyle::Shaded ) // Moderate-quality
  {
    this->font_buffer_.initialize ( TTF_RenderText_Shaded
                                  (
                                    this->font.get(),
                                    this->getText().c_str(),
                                    SDL_COLOR(this->color),
                                    // TODO; implement me -- a second color
                                    // possibility means needing two colors in
                                    // class
                                    SDL_COLOR ( Color4u ( 97, 97, 97 ) )
                                  )
                                );
  }
  else if ( this->rendering == RenderStyle::Blended ) // Highest-quality
  {
    this->font_buffer_.initialize ( TTF_RenderText_Blended
                                  (
                                    this->font.get(),
                                    this->getText().c_str(),
                                    SDL_COLOR(this->color)
                                  )
                                );
  }
  else // Low-quality rendering (the default)
  {
    this->font_buffer_.initialize ( TTF_RenderText_Solid
                                  (
                                    this->font.get(),
                                    this->getText().c_str(),
                                    SDL_COLOR(this->color)
                                  )
                                );
  }

  if ( this->text_style == FontStyle::Faded )
  {
    this->font_buffer_.set_alpha ( this->style_options );
  }
}
*/

/*
void TrueTypeFont::draw ( RenderTarget target ) const
{
  if ( this->font_buffer_.valid() )
  {
    this->font_buffer_.draw ( target );
  }
}
*/

bool TrueTypeFont::build ( uint32 character_size )
{
  int ret = 0; // Error code
  uint16 ascii_char; // Integer type expected by SDL2_ttf

  // Glyph metrics
  int advance = 0;
  //int x_offset = 0;
  //int y_offset = 0;
  //int width = 0;
  //int height = 0;

  // Counters used in sheet calculations;
  int rows = 0; // Left to right (X axis)
  int cols = 0; // Top to bottom (Y axis)
  int padding = 1;
  int spacing = 2;

  std::vector<Glyph> glyph_sort;
  Image glyph_image; // Raster bitmap of a glyph

  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "TTF pointer is invalid" );
    return false;
  }

  const Point2i sheet_size =  Point2i ( sheet_width() * sheet_height(), // 256
                                        sheet_width() * sheet_height()  // 256
                                      );

  this->pages_[0].texture->initialize ( sheet_size );
  glyph_sort.resize ( sheet_size.y );

  for ( uint32 glyph = 33; glyph < 100; ++glyph )
  //for ( uint32 glyph = 0; glyph < (sheet_width() * sheet_height()); ++glyph )
  {
    ascii_char = static_cast<uint16> ( glyph );
    //ascii_char = static_cast<uint16> ( 'g' );

    if ( TTF_GlyphIsProvided ( this->font_.get(), ascii_char ) )
    {
      if ( (rows + 1) > sheet_width() )
      {
        rows = 0;
        cols += 1;

        padding = padding * cols + 1;
      }

      // We obtain width & height of a glyph from its rendered form
      glyph_image.initialize ( TTF_RenderGlyph_Solid ( this->font_.get(), ascii_char, SDL_COLOR(NOM_COLOR4U_WHITE) ) );

      if ( glyph_image.valid() == false )
      {
        NOM_LOG_ERR(NOM, TTF_GetError() );
        //NOM_LOG_ERR(NOM, "Could not store surface from glyph character: " + std::to_string(ascii_char) );
        return false;
      }

      // -_-
      // Disappointedly, the only metric that we can use here is the advance
      ret = TTF_GlyphMetrics  ( this->font_.get(),
                                ascii_char,
                                nullptr, // Left (X) origin
                                nullptr, // Width
                                nullptr, // Top (Y) origin
                                nullptr, // Height
                                &advance
                              );

      if ( ret != 0 ) // Likely to be a missing glyph
      {
        NOM_LOG_ERR ( NOM, TTF_GetError() );
        return false;
      }

      // FIXME: we need to deal with the varying widths
      this->pages_[0].glyphs[glyph].bounds.x = ( spacing + rows ) + glyph_image.width() * rows;
      this->pages_[0].glyphs[glyph].bounds.y = glyph_image.height() * ( cols ) + padding;
      this->pages_[0].glyphs[glyph].bounds.w = glyph_image.width();
      this->pages_[0].glyphs[glyph].bounds.h = glyph_image.height();
      this->pages_[0].glyphs[glyph].advance = advance;

      #if defined(NOM_DEBUG_SDL2_TRUE_TYPE_FONT_GLYPHS)
        NOM_DUMP_VAR(static_cast<uchar>(glyph));
        NOM_DUMP_VAR(this->glyph(glyph).bounds);
        //NOM_DUMP_VAR(advance);
      #endif

      IntRect bounds;
      bounds.x = this->pages_[0].glyphs[glyph].bounds.x;
      bounds.y = this->pages_[0].glyphs[glyph].bounds.y;
      bounds.w = -1;
      bounds.h = -1;

      glyph_image.draw( this->pages_[0].texture->image(), bounds );
      //glyph_image.draw( this->pages_[0].texture->image(), this->pages_[0].glyphs[glyph].bounds );

      //this->pages_[0].texture->set_alpha(255);
      //this->pages_[0].texture->set_blend_mode(SDL_BLENDMODE_NONE);
      //this->pages_[0].texture->draw ( glyph_image, this->pages_[0].glyphs[glyph].bounds );

      //this->pages_[0].texture->initialize ( glyph_image );
      //priv::FreeSurface ( glyph );

      this->pages_[0].texture->set_colorkey ( Color4u(0,0,0,255), true );

      #if defined(NOM_DEBUG_SDL2_TRUE_TYPE_FONT_GLYPHS_PNG)
        std::string ascii_fname = std::to_string(ascii_char);
        ascii_fname.append(".png");
        glyph_image.save_png(ascii_fname);
      #endif
    } // end if glyph is provided
    rows++;
  } // end for loop

  //std::sort ( glyph_sort.begin(), glyph_sort.end(), std::greater<Glyph>() );
  //for ( GlyphAtlas::iterator it = this->pages_[0].glyphs.begin(); it != this->pages_[0].glyphs.end(); ++it )
  //{
    //std::cout << it->first << " => " << it->second << "\n";
  //}

  this->pages_[0].texture->save_png("dest.png");

  return true;
}

const GlyphPage& TrueTypeFont::pages ( void ) const
{
  return this->pages_;
}

sint TrueTypeFont::sheet_width ( void ) const
{
  return this->sheet_width_;
}

sint TrueTypeFont::sheet_height ( void ) const
{
  return this->sheet_height_;
}

} // namespace nom
