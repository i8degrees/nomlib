/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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

// Third-party headers
#include "SDL_ttf.h"

// Private headers
#include "nomlib/math/Rect.hpp"
#include "nomlib/system/SDL_helpers.hpp"
#include "nomlib/graphics/Texture.hpp"

// Forward declarations
#include "nomlib/graphics/Image.hpp"

namespace nom {

TrueTypeFont::TrueTypeFont ( void ) :
  type_ ( IFont::FontType::TrueTypeFont ),
  sheet_width_ ( 16 ),
  sheet_height_ ( 16 ),
  point_size_ ( nom::DEFAULT_FONT_SIZE ),   // Terrible Eyesight (TM)
  use_kerning_(true)
{
  // NOM_LOG_TRACE( NOM );

  // TTF_Init is done in the nom::init_third_party function
}

TrueTypeFont::~TrueTypeFont ( void )
{
  // NOM_LOG_TRACE( NOM );

  // Workaround hack to get SDL2_ttf to shutdown properly without crashing when
  // ran within nom::GameStates
  //this->font_.reset();
}

TrueTypeFont::TrueTypeFont ( const TrueTypeFont& copy ) :
  type_ { copy.type() },
  sheet_width_ { copy.sheet_width() },
  sheet_height_ { copy.sheet_height() },
  font_ { copy.font_ },
  pages_ { copy.pages() },
  metrics_ { copy.metrics() },
  filename_ { copy.filename_ },
  point_size_ { copy.point_size() },
  use_kerning_(copy.use_kerning_)
{
  // NOM_LOG_TRACE( NOM );
}

IFont::raw_ptr TrueTypeFont::clone( void ) const
{
  return new TrueTypeFont( *this );
}

bool TrueTypeFont::valid ( void ) const
{
  if ( this->font() != nullptr ) return true;

  return false;
}

enum IFont::FontType TrueTypeFont::type ( void ) const
{
  return this->type_;
}

TTF_Font* TrueTypeFont::font ( void ) const
{
  return this->font_.get();
}

const Image* TrueTypeFont::image(uint32 character_size) const
{
  return this->pages_[character_size].texture.get();
}

int TrueTypeFont::spacing ( uint32 character_size ) const
{
  return this->pages_[character_size].glyphs[32].advance;
}

sint TrueTypeFont::point_size ( void ) const
{
  return this->point_size_;
}

int TrueTypeFont::newline( uint32 character_size ) const
{
  if ( this->valid() == true )
  {
    return this->metrics_.newline;
  }
  else
  {
    return 0;
  }
}

int TrueTypeFont::kerning( uint32 first_char, uint32 second_char, uint32 character_size ) const
{
  int kerning_offset = 0;

  // if ( first_char == 0 || second_char == 0 ) {
  //   // Null character
  //   return kerning_offset;
  // }

  if( this->use_kerning_ == false ) {
    // Use of kerning pair offsets are disabled
    return kerning_offset;
  }

  if( this->valid() == false ) {
    // Invalid font
    return nom::int_min;
  }

  // NOM_LOG_INFO( NOM, "sdl2_ttf kerning: ", TTF_GetFontKerning( this->font() ) );

  // Check for state consistency between SDL_TTF and us
  NOM_ASSERT(TTF_GetFontKerning( this->font() ) == this->use_kerning_);

  kerning_offset = TTF_GetFontKerningSize(this->font(), first_char, second_char);

  if( kerning_offset == nom::int_min ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not obtain kerning offset: ", TTF_GetError() );

    // Err; SDL_TTF related
    return kerning_offset;
  }

  // Success
  return kerning_offset;
}

const Glyph& TrueTypeFont::glyph ( uint32 codepoint, uint32 character_size ) const
{
  GlyphAtlas& glyphs = this->pages_[character_size].glyphs;

  GlyphAtlas::const_iterator it = glyphs.find(codepoint);
  if ( it != glyphs.end() )
  {
    return it->second; // Found a match
  }
  else
  {
    return glyphs[codepoint]; // Err (probably going to receive an empty glyph)
  }
}

int TrueTypeFont::outline ( /*uint32 character_size*/void ) /*const*/
{
  if ( this->valid() == true && this->set_point_size ( /*character_size*/this->point_size() ) == true )
  {
    return TTF_GetFontOutline ( this->font() );
  }

  return 0;
}

int TrueTypeFont::hinting( void ) const
{
  if( this->valid() == true ) {
    return TTF_GetFontHinting( this->font() );
  }

  // Default
  return TTF_HINTING_NONE;
}

uint32 TrueTypeFont::font_style( void ) const
{
  if( this->valid() == true ) {
    return TTF_GetFontStyle( this->font() );
  }

  // Default
  return TTF_STYLE_NORMAL;
}

bool TrueTypeFont::set_point_size( int point_size )
{
  // Expensive method call
  if ( this->point_size() != point_size )
  {
    int original_font_size = this->point_size();

    this->point_size_ = point_size;

    if( this->load( this->filename_ ) == false )
    {
      NOM_LOG_ERR( NOM, "Could not set new point size: " + std::to_string( point_size ) );

      // Restore the original point size if we fail to reload font.
      this->point_size_ = original_font_size;

      return false;
    }
  }

  return true;
}

bool TrueTypeFont::set_hinting( int type )
{
  // Expensive function call
  if( this->hinting() != type ) {

    if( this->valid() == true ) {
      TTF_SetFontHinting( this->font(), type );

      int point_size = this->point_size();

      // Force a rebuild of the font page by clearing / invalidating it.
      // this->pages_[point_size].invalidate();

      if( this->build(point_size) == false ) {
        NOM_LOG_ERR ( NOM, "Could not set requested font hinting." );
        return false;
      }

      // Success
      return true;
    } // end if valid

    // Err; invalid
    return false;
  } // end if hinting != type

  // Nothing to do; the font hinting has not changed
  return true;
}

bool TrueTypeFont::set_outline( int outline )
{
  // Expensive function call
  if ( this->outline() != outline )
  {
    TTF_SetFontOutline ( this->font(), outline );

    if ( this->build( this->point_size() ) == false )
    {
      NOM_LOG_ERR ( NOM, "Could not set new point size." );
      return false;
    }

    return true;
  }

  return true;
}

void TrueTypeFont::set_font_style( uint32 style )
{
  if( style != this->font_style() )
  {
    TTF_SetFontStyle( this->font(), style );

    int point_size = this->point_size();

    // Force a rebuild of the font page by clearing / invalidating it.
    this->pages_[point_size].invalidate();

    if( this->build( point_size ) == false )
    {
      NOM_LOG_ERR( NOM, "Could not rebuild glyph metrics." );
    }
  }
}

void TrueTypeFont::set_font_kerning( bool state )
{
  if( state == true )
  {
    TTF_SetFontKerning( this->font(), 1 );
    this->use_kerning_ = true;
  }
  else
  {
    TTF_SetFontKerning( this->font(), 0 );
    this->use_kerning_ = false;
  }
}

bool TrueTypeFont::load( const std::string& filename )
{
  this->font_ = std::shared_ptr<TTF_Font> ( TTF_OpenFont ( filename.c_str(), this->point_size() ), priv::TTF_FreeFont );

  if ( this->valid() == false )
  {
    NOM_LOG_ERR( NOM, "Could not load TTF file: " + filename );
    NOM_LOG_ERR( NOM, TTF_GetError() );
    return false;
  }

  // Store the filename for future reference; we use this cached filename for
  // when a new font point size is requested.
  this->filename_ = filename;

  // Save global font metrics; these values are dependent upon the font's
  // current point size, so we need to regenerate them even when the glyph page
  // for the given point size exists.
  this->metrics_.height = TTF_FontHeight( this->font() );
  this->metrics_.newline = TTF_FontLineSkip( this->font() );
  this->metrics_.ascent = TTF_FontAscent( this->font() );
  this->metrics_.descent = TTF_FontDescent( this->font() );
  this->metrics_.family = TTF_FontFaceFamilyName( this->font() );

  // Set the font face style name
  this->metrics_.name = std::string( TTF_FontFaceFamilyName( this->font() ) );

  // FIXME: This feature is broken
  // this->set_hinting( this->hinting() );

  // Attempt to build font metrics
  if ( this->build ( this->point_size() ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not build TrueType font metrics" );
    return false;
  }

  return true;
}

const FontMetrics& TrueTypeFont::metrics( void ) const
{
  return this->metrics_;
}

bool TrueTypeFont::build ( uint32 character_size )
{
  int ret = 0;                      // Error code
  uint16 ascii_char;                // Integer type expected by SDL2_ttf
  const uint32 starting_glyph = 32; // Space character
  const uint32 ending_glyph = 127;  // Tilde character

  // Glyph metrics
  int advance = 0;      // Spacing between characters
  int glyph_width = 0;  // Glyph's width in pixels
  int glyph_height = 0; // Glyph's height in pixels

  // Texture sheet calculations
  int padding = 1;
  int spacing = 2;

  Image glyph_image;                              // Raster bitmap of a glyph
  IntRect blit;                                   // Rendering bounding coords
  FontPage& page = this->pages_[character_size];  // Our font's current glyph
                                                  // page
  Point2i sheet_size;                             // Texture atlas dimensions

  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "TTF pointer is invalid" );
    return false;
  }

  // Glyph page has already been calculated, so we can use this cache and spare
  // many CPU cycles. This resolves the thrashing issue we observed on Windows
  // OS, when quickly increasing & decreasing the font's point size, such as in
  // nomlib's app example.
  //
  // NOTE: Glyph caching trade memory for performance.
  if( page.valid() )
  {
    // NOM_DUMP("cached");
    return true;
  }

  // Our starting sheet size; we will allocate a larger sheet size if needed
  // during glyph rasterization -- see ::glyph_rect.
  //
  // NOTE: The initial size of the texture sheet directly affects loading time.
  sheet_size = Point2i  (
                          this->sheet_width() * this->sheet_height(),
                          this->sheet_width() * this->sheet_height()
                        );

  page.texture->initialize ( sheet_size );

  // this->set_hinting( this->hinting() );

  // ASCII 32..127 is the standard glyph set -- 94 printable characters;
  //
  // 32 = ' ' (space)
  // 65 = 'A'
  // 122 = 'z'
  // 126 = '~' (tilde)

  // glyph < sheet_width() * sheet_height() = 256 glyphs
  for ( uint32 glyph = starting_glyph; glyph < ending_glyph; ++glyph )
  {
    ascii_char = static_cast<uint16> ( glyph );

    if ( TTF_GlyphIsProvided ( this->font(), ascii_char ) )
    {
      // We obtain width & height of a glyph from its rendered form
      glyph_image.initialize ( TTF_RenderGlyph_Solid( this->font(), ascii_char, SDL_COLOR(Color4i::White) ) );

      if ( glyph_image.valid() == false )
      {
        NOM_LOG_ERR(NOM, TTF_GetError() );
        return false;
      }

      glyph_width = glyph_image.width();
      glyph_height = glyph_image.height();

      // -_-
      // Disappointedly, the only metric that we can use here is the advance
      ret = TTF_GlyphMetrics  ( this->font(),
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
      page.glyphs[glyph].advance = advance;

      // Calculate the best packing of the glyph, so that we are able to fit
      // everything on the sheet without overlaps; if we cannot fit all of our
      // glyphs onto the default sheet dimensions, we allocate a larger sheet
      // size.
      page.glyphs[glyph].bounds = this->glyph_rect ( page, glyph_width + spacing * padding, glyph_height + spacing * padding );

      #if defined(NOM_DEBUG_SDL2_TRUE_TYPE_FONT_GLYPHS)
        NOM_DUMP(glyph); // integer position
        NOM_DUMP(static_cast<uchar>(glyph)); // printable
        NOM_DUMP(this->glyph(glyph).bounds); // bounding box
        NOM_DUMP(advance); // spacing
      #endif

      // Prepare the coordinates for rendering a glyph onto our texture sheet
      // we are creating.
      blit.x = page.glyphs[glyph].bounds.x;
      blit.y = page.glyphs[glyph].bounds.y;
      blit.w = -1; // Why -1 ???
      blit.h = -1; // Why -1 ???
      glyph_image.draw( page.texture->image(), blit );

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

  // Turn color key transparency on so we are not left with a black,
  // AKA non-transparent background.
  page.texture->set_colorkey ( Color4i::Black, true );

  // Export the destination texture -- this should be a texture sheet that we
  // expect to be able to render from within the nom::Text class.
  #if defined(NOM_DEBUG_SDL2_TRUE_TYPE_FONT_GLYPHS)
    // We intentionally flip color keying off here temporarily so that our eyes
    // have an easier time.
    //
    // You'll may want to leave color keying on if you are interested in using
    // these bitmap glyphs for anything else!
    page.texture->set_colorkey ( Color4i::Black, false );
    page.texture->save_png("ttf_dest.png");
    page.texture->set_colorkey ( Color4i::Black, true );
  #endif

  // Build up our font-wide metrics
  if (this->valid() == false )
  {
    NOM_LOG_ERR(NOM, "Could not build font metrics; our font is invalid" );
    return false;
  }

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

const IntRect TrueTypeFont::glyph_rect ( FontPage& page, int width, int height ) const
{
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

  // If we didn't find a matching row, create a new one
  // (10% taller than the glyph)
  if ( ! row )
  {
    int row_height = height + height / 10;
    while( page.next_row + row_height >= page.texture->height() || ( width >= page.texture->width() ) )
    {
      // Not enough space: resize the texture if possible
      uint texture_width  = page.texture->width();
      uint texture_height = page.texture->height();
      if  (
            ( texture_width * 2 <= Texture::maximum_size().x )
            &&
            ( texture_height * 2 <= Texture::maximum_size().y )
          )
      {
        // Make the texture 2 times bigger
        Image sheet; // new (destination) sheet
        sheet.initialize ( Point2i( texture_width * 2, texture_height * 2 ) );

        // Copy existing texture sheet to new sheet
        page.texture->draw ( sheet.image(), IntRect(0, 0, -1, -1) );

        #if defined(NOM_DEBUG_SDL2_TRUE_TYPE_FONT_GLYPHS)
          sheet.save_png("ttf_src.png");
        #endif

        // Re-initialize our texture sheet with a copy of the resized sheet
        page.texture->initialize ( sheet.clone() );
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
