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
#include "nomlib/graphics/fonts/TrueTypeFont.hpp"

namespace nom {

TrueTypeFont::TrueTypeFont ( void ) :
  sheet_width_ ( 16 ),  // Arbitrary; based on nom::BitmapFont
  sheet_height_ ( 16 ), // Arbitrary; based on nom::BitmapFont
  font_size_ ( 14 ), // Terrible Eyesight (TM)
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
  this->font_ = copy.font_;
  this->pages_ = copy.pages();
  this->type_ = copy.type();
  this->font_size_ = copy.font_size();
  this->filename_ = copy.filename_;
  this->use_cache_ = copy.use_cache_;
  this->metrics_ = copy.metrics_;
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

SDL_SURFACE::RawPtr TrueTypeFont::image ( uint32 character_size ) const
{
  return this->pages_[character_size].texture->image();
}

int TrueTypeFont::spacing ( uint32 character_size ) const
{
  return this->pages_[character_size].glyphs[32].advance; // FIXME?
}

sint TrueTypeFont::font_size ( void ) const
{
  return this->font_size_;
}

int TrueTypeFont::newline ( uint32 character_size ) const
{
  return this->metrics_.newline;
}

sint TrueTypeFont::kerning ( uint32 first_char, uint32 second_char, uint32 character_size ) const
{
  return -1; // TODO
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
  const uint32 starting_glyph = 32; // space character
  const uint32 ending_glyph = 127; // tilde character

  // Glyph metrics
  int advance = 0; // spacing in between characters separated by a space char
  int glyph_width = 0; // glyph's width in pixels
  int glyph_height = 0; // glyph's height in pixels

  // Texture sheet calculations
  int padding = 1;
  int spacing = 2;

  Image glyph_image; // Raster bitmap of a glyph
  IntRect blit; // rendering bounding coordinates
  //FontPage& page = this->pages_[0]; // our font's glyph page

  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "TTF pointer is invalid" );
    return false;
  }

  const Point2i sheet_size =  Point2i ( sheet_width() * sheet_height(), // 256
                                        sheet_width() * sheet_height()  // 256
                                      );

  this->pages_[0].texture->initialize ( sheet_size );

  // ASCII 32..127 is the standard glyph set -- 94 printable characters;
  //
  // 32 = ' ' (space)
  // 65 = 'A'
  // 122 = 'z'
  // 126 = '~' (tilde)
  //
  // glyph < sheet_width() * sheet_height() = 256 glyphs
  for ( uint32 glyph = starting_glyph; glyph < ending_glyph; ++glyph )
  {
    ascii_char = static_cast<uint16> ( glyph );

    if ( TTF_GlyphIsProvided ( this->font_.get(), ascii_char ) )
    {
      // We obtain width & height of a glyph from its rendered form
      glyph_image.initialize ( TTF_RenderGlyph_Solid ( this->font_.get(), ascii_char, SDL_COLOR(NOM_COLOR4U_WHITE) ) );

      if ( glyph_image.valid() == false )
      {
        NOM_LOG_ERR(NOM, TTF_GetError() );
        return false;
      }

      glyph_width = glyph_image.width();
      glyph_height = glyph_image.height();

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
      this->pages_[0].glyphs[glyph].advance = advance;

      // Calculate the best packing of the glyph, so that we are able to fit
      // everything on the sheet without overlap, etc.
      this->pages_[0].glyphs[glyph].bounds = this->glyph_rect ( this->pages_[0], glyph_width + spacing * padding, glyph_height + spacing * padding );

      #if defined(NOM_DEBUG_SDL2_TRUE_TYPE_FONT_GLYPHS)
        NOM_DUMP_VAR(glyph); // integer position
        NOM_DUMP_VAR(static_cast<uchar>(glyph)); // printable
        NOM_DUMP_VAR(this->glyph(glyph).bounds); // bounding box
        NOM_DUMP_VAR(advance); // spacing
      #endif

      // Prepare the rendering coordinates for writing the glyph our texture map
      // Prepare the coordinates for rendering a glyph onto our texture sheet
      // we are creating.
      blit.x = this->pages_[0].glyphs[glyph].bounds.x;
      blit.y = this->pages_[0].glyphs[glyph].bounds.y;
      blit.w = -1; // Why -1 ???
      blit.h = -1; // Why -1 ???
      glyph_image.draw( this->pages_[0].texture->image(), blit );

      //this->pages_[0].texture->set_alpha(255);
      //this->pages_[0].texture->set_blend_mode(SDL_BLENDMODE_NONE);

      // Turn color key transparency on so we are not left with a black,
      // AKA non-transparent background.
      this->pages_[0].texture->set_colorkey ( NOM_COLOR4U_BLACK, true );

      // Dump all of the rendered glyphs as a series of image files -- the
      // filenames will be the ASCII numeric values. The output should consist
      // of individual glyph files, at whatever font scale previously loaded.
      //
      // You could easily create a new sprite sheet out of these individual
      // frames!
      #if defined(NOM_DEBUG_SDL2_TRUE_TYPE_FONT_GLYPHS_PNG)
        std::string ascii_filename = std::to_string(ascii_char);
        ascii_filename.append(".png");
        glyph_image.save_png(ascii_filename);
      #endif
    } // end if glyph is provided
  } // end for glyphs loop

  // Export the destination texture -- this should be a texture sheet that we
  // can render from within the nom::Label class.
  #if defined(NOM_DEBUG_SDL2_TRUE_TYPE_FONT_GLYPHS)
    // We intentionally flip color keying off here temporarily so that our eyes
    // have an easier time.
    //
    // You'll may want to leave color keying on if you are interested in using
    // these bitmap glyphs for anything else!
    this->pages_[0].texture->set_colorkey ( NOM_COLOR4U_BLACK, false );
    this->pages_[0].texture->save_png("ttf_dest.png");
    this->pages_[0].texture->set_colorkey ( NOM_COLOR4U_BLACK, true );
  #endif

  // Build up our font-wide metrics
  if (this->valid() == false )
  {
    NOM_LOG_ERR(NOM, "Could not build font metrics; our font is invalid" );
    return false;
  }

  this->metrics_.height = TTF_FontHeight ( this->font_.get() );
  this->metrics_.newline = TTF_FontLineSkip ( this->font_.get() );
  this->metrics_.ascent = TTF_FontAscent ( this->font_.get() );
  this->metrics_.descent = TTF_FontDescent ( this->font_.get() );

  std::string family = TTF_FontFaceFamilyName ( this->font_.get() );
  NOM_DUMP_VAR(family);
  this->metrics_.family = family;

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

void TrueTypeFont::set_spacing ( int spaces )
{
  return; // TODO
  //this->spacing_ = spaces;
}

void TrueTypeFont::set_newline ( int newline )
{
  return; // TODO
  //this->newline_ = newline;
}

const IntRect TrueTypeFont::glyph_rect ( FontPage& page, int width, int height ) const
{
  //std::unique_ptr<FontRow> row;
  FontRow* row = nullptr;
  float best_ratio = 0;

  // Find an optimal layout within our texture's sheet for a specified glyph
  for ( std::vector<FontRow>::iterator it = page.rows.begin(); it != page.rows.end() && ! row; ++it )
  {
    float ratio = static_cast<float> ( height ) / it->height;

    // Ignore rows that are either too small or too high
    if ( ( ratio < 0.7f ) || ( ratio > 1.f ) )
    {
      continue;
    }

    // Check if there's enough horizontal space left in the row
    if ( width > page.texture->width() - it->width )
    {
      continue;
    }

    // Make sure that this new row is the best found so far
    if ( ratio < best_ratio )
    {
      continue;
    }

    // The current row passed all the tests: we can select it
    row = &*it;
    best_ratio = ratio;
  }

  // If we didn't find a matching row, create a new one (10% taller than the glyph)
  if ( ! row )
  {
    int row_height = height + height / 10;
    while ( page.next_row + row_height >= page.texture->height() )
    {
      // Not enough space: resize the texture if possible
      uint texture_width  = page.texture->width();
      uint texture_height = page.texture->height();
      if ( ( texture_width * 2 <= Texture::maximum_size().x )
           && ( texture_height * 2 <= Texture::maximum_size().y ) )
      {
        // Make the texture 2 times bigger
        NOM_LOG_ERR(NOM,"WATCH OUT -- texture rescaling is 50/50 chance of working");
        Image sheet;
        sheet.initialize ( Point2i( texture_width * 2, texture_height * 2 ) );
        sheet.draw ( page.texture->image(), IntRect(0,0,-1,-1) );
      }
      else
      {
        // Oops, we've reached the maximum texture size...
        NOM_LOG_ERR( NOM, "Failed to add new character to sheet: sheet >= maximum texture size" );
        return IntRect ( 0, 0, 2, 2 );
      }
    }

    // We can now create the new row
    page.rows.push_back ( FontRow ( page.next_row, row_height ) );
    page.next_row += row_height;
    row = &page.rows.back();
  }

  // Find the glyph's rectangle on the selected row
  IntRect rect ( row->width, row->top, width, height );

  // Update the row informations
  row->width += width;

  return rect;
}

} // namespace nom
