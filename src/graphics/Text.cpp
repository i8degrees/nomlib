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
#include "nomlib/graphics/Text.hpp"

// Private headers (third-party)
#include "SDL.h"

// Private headers
#include "nomlib/graphics/fonts/Glyph.hpp"

namespace nom {

Text::Text( void ) :
  Transformable { Point2i::null, Size2i::null }, // Base class
  text_size_ ( nom::DEFAULT_FONT_SIZE ),
  color_ ( Color4i::White ),
  style_ ( Text::Style::Normal ),
  dirty_(false)
{
  // NOM_LOG_TRACE( NOM );
}

Text::~Text( void )
{
  // NOM_LOG_TRACE( NOM );
}

Text::Text  (
              const std::string& text,
              const Font& font,
              uint character_size,        // Default parameter
              const Color4i& text_color   // Default parameter
            )  :
  Transformable { Point2i::null, Size2i::null }, // Base class
  text_ ( text ),
  text_size_( character_size ),
  style_( Text::Style::Normal ),
  dirty_(false)
{
  // NOM_LOG_TRACE( NOM );

  this->set_font( font );
  this->set_color( text_color );
}

Text::Text  (
              const std::string& text,
              Font* font,
              uint character_size,        // Default parameter
              const Color4i& text_color   // Default parameter
            )  :
  Transformable { Point2i::null, Size2i::null }, // Base class
  text_ ( text ),
  text_size_( character_size ),
  style_( Text::Style::Normal ),
  dirty_(false)
{
  // NOM_LOG_TRACE( NOM );

  this->set_font( font );
  this->set_color( text_color );
}

IDrawable::raw_ptr Text::clone( void ) const
{
  return Text::raw_ptr( new Text( *this ) );
}

ObjectTypeInfo Text::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

const Font& Text::font() const
{
  return this->font_;
}

const Texture& Text::texture ( void ) const
{
  return this->texture_;
}

bool Text::valid ( void ) const
{
  if( this->font() != nullptr ) {
    return this->font()->valid();
  }
  return false;
}

// enum IFont::FontType Text::font_type( void ) const
// {
//   if ( this->valid() ) return this->font()->type();

//   return IFont::FontType::NotDefined;
// }

sint Text::text_width ( const std::string& text_string ) const
{
  sint text_width = 0;
  uint32 previous_char = 0; // Kerning calculation
  int kerning_offset = 0;
  std::string text_buffer = text_string;

  // Ensure that our font pointer is still valid
  if ( this->valid() == false )
  {
    NOM_LOG_ERR( NOM, "Invalid font for width calculation" );
    return text_width;
  }

  // Calculate text width up to either: a) end of string; b) newline character.
  //
  // We add kerning offsets, the glyph's advance offsets, and spacing onto the
  // total text_width count.
  for( auto pos = 0; pos < text_buffer.length(); ++pos )
  {
    // Apply kerning offset
    uint32 current_char = text_buffer[pos];

    kerning_offset = this->font()->kerning( previous_char, current_char, this->text_size() );

    if( kerning_offset != nom::int_min ) {
      text_width += kerning_offset;
    }

    previous_char = current_char;

    if ( current_char == ' ' ) // ASCII space glyph (32)
    {
      text_width += this->font()->spacing ( this->text_size() );
    }
    else if ( current_char == '\t' ) // Tab character (we indent two spaces)
    {
      text_width += this->font()->spacing ( this->text_size() ) * 2;
    }
    else if( current_char == '\n' )
    {
      break;
    }
    else // Printable ASCII glyph (33..127)
    {
      // Match the offset calculations done in the text rendering -- hence the
      // plus one (+1) spacing.
      text_width += this->font()->glyph(current_char, this->text_size() ).advance + 1;
    }
  } // end for loop

  return text_width;
}

sint Text::text_height ( const std::string& text_string ) const
{
  sint text_height = 0;
  std::string text_buffer = text_string;

  // Ensure that our font pointer is still valid
  if ( this->valid() == false )
  {
    NOM_LOG_ERR( NOM, "Invalid font for height calculation" );
    return text_height;
  }

  // Initialize our text string's height to be whatever the font's newline
  // metric was set to.
  text_height = this->font()->newline ( this->text_size() );

  // Calculate the text string's height adding the font's newline metric onto
  // our text_height counter every time we find a newline character.
  //
  // If no newline characters are found, we fall back to returning our
  // initialized text height value.
  for ( uint32 pos = 0; pos < text_buffer.length(); ++pos )
  {
    uint32 current_char = text_buffer[pos];

    if ( current_char == '\n' || current_char == '\v' ) // Multi-line case
    {
      text_height += this->font()->newline ( this->text_size() );
    }
  } // end for loop

  return text_height;
}

const std::string& Text::text ( void ) const
{
  return this->text_;
}

uint Text::text_size ( void ) const
{
  return this->text_size_;
}

const Color4i& Text::color( void ) const
{
  return this->color_;
}

uint32 Text::style( void ) const
{
  return this->style_;
}

void Text::set_font(const Font& font)
{
  this->font_ = font;

  this->set_text_size( this->text_size() );
}

void Text::set_font(Font* font)
{
  this->font_ = *font;

  this->set_text_size( this->text_size() );
}

void Text::set_text( const std::string& text )
{
  if( text != this->text() ) {
    this->text_ = text;

    this->dirty_ = true;
    this->update();
  }
}

void Text::set_text_size ( uint character_size )
{
  if ( this->valid() == false )
  {
    NOM_LOG_ERR( NOM, "Could not set text size: the font is invalid." );
    return;
  }

  this->text_size_ = character_size;

  // Force a rebuild of the texture atlas at the specified point size
  this->font()->set_point_size( this->text_size() );

  // We can only (sanely) initialize our texture once we know the text size; we
  // will update this texture only as it becomes necessary (see ::update).
  if( this->texture_.create( *this->font()->image( this->text_size() ), SDL_PIXELFORMAT_ARGB8888, Texture::Access::Streaming ) == false ) {
    NOM_LOG_ERR(  NOM, "Could not create texture at point size:",
                  std::to_string( this->text_size() ) );
    return;
  }

  this->dirty_ = true;
  this->update();
}

void Text::set_color( const Color4i& color )
{
  if( color == this->color() )
  {
    return;
  }

  this->color_ = color;

  this->dirty_ = true;
  this->update();
}

void Text::set_style( uint32 style )
{
  if( this->style() == style )
  {
    return;
  }

  // Set new style if sanity checks pass
  this->style_ = style;

  this->dirty_ = true;
  this->update();
}

void Text::draw ( RenderTarget& target ) const
{
  int kerning_offset = 0;
  uint32 previous_char = 0;
  uint32 current_char = 0;

  // Use coordinates provided by interface user as our starting origin
  // coordinates to compute from
  Point2i pos( this->position() );

  // Additional rendering offsets; applicable to nom::BMFont
  Point2i glyph_offset(Point2i::zero);

  // Final rendering position
  Point2i tex_pos(Point2i::zero);

  double angle = 0;

  std::string text_buffer = this->text();

  // No font has been loaded -- nothing to draw!
  if ( this->valid() == false )
  {
    NOM_LOG_ERR( NOM, "Invalid Text font" );
    return;
  }

  if ( this->style() == Text::Style::Italic )
  {
    // angle = 12; // 12 degrees as per SDL2_ttf
  }

  for( auto i = 0; i < text_buffer.length(); ++i ) {

    // Apply kerning offset
    current_char = text_buffer[i];
    kerning_offset = this->font()->kerning( previous_char, current_char, this->text_size() );

    if( kerning_offset != nom::int_min ) {
      pos.x += kerning_offset;
    }

    previous_char = current_char;

    glyph_offset =
      this->font()->glyph( current_char, this->text_size() ).offset;

    if ( current_char == ' ' ) // Space character
    {
      // Move over
      pos.x += this->font()->spacing ( this->text_size() );
    }
    else if( current_char == '\n' || current_char == '\v' ) // Vertical chars
    {
      // Move down and back over to the beginning of line
      pos.y += this->font()->newline ( this->text_size() );
      pos.x = this->position().x;
    }
    else if( current_char == '\t' ) // Tab character (we indent two spaces)
    {
      pos.x += this->font()->spacing ( this->text_size() ) * 2;
    }
    else // The time to render is now!
    {
      // Apply rendering offsets; applicable to nom::BMFont glyphs
      tex_pos = Point2i(pos.x + glyph_offset.x, pos.y + glyph_offset.y);

      this->texture_.set_position(tex_pos);
      this->texture_.set_bounds ( this->font()->glyph(current_char, this->text_size() ).bounds );

      this->texture_.draw ( target.renderer(), angle );

      // Move over the width of the character with one pixel of padding
      pos.x += ( this->font()->glyph(current_char, this->text_size() ).advance ) + 1;

    } // end else
  } // end for loop
}

void Text::update()
{
  uint32 style = this->style();
  uint32 e_style = 0;

  if( this->dirty_ == false ) {
    return;
  }

  this->dirty_ = false;

  // No font has been loaded -- nothing to draw!
  if( this->valid() == false ) {
    return;
  }

  e_style = this->font()->font_style();

  if( style & Text::Style::Bold ) {
    e_style |= TTF_STYLE_BOLD;
  }

  if( style & Text::Style::Italic ) {
    e_style |= TTF_STYLE_ITALIC;
  }

  if( style & Text::Style::Underlined ) {
    e_style |= TTF_STYLE_UNDERLINE;
  }

  if( style & Text::Style::Strikethrough ) {
    e_style |= TTF_STYLE_STRIKETHROUGH;
  }

  // Expensive call
  if( ! (style & Text::Style::Normal) ) {
    this->font()->set_font_style(e_style);
  }

  // Update the texture atlas; this is necessary anytime we rebuild the font's
  // glyphs cache, such as when we change the text's font point size or rendering
  // style.
  const Image* source = this->font()->image( this->text_size() );

  if( source->valid() == false ) {
    NOM_LOG_ERR(  NOM,
                  "Could not update texture: invalid glyph image source." );
    return;
  }

  this->texture_.lock();
  this->texture_.copy_pixels( source->pixels(),
                              source->pitch() * source->height() );
  this->texture_.unlock();

  // Preserve alpha channel
  this->texture_.set_blend_mode(SDL_BLENDMODE_BLEND);

  // Update the font's text color.
  this->texture_.set_color_modulation( this->color() );

  // Set the overall size of this text label to the width & height of the text,
  // with consideration to the specific font in use.
  this->set_size( Size2i( this->width(), this->height() ) );
}

// Private scope

int Text::width() const
{
  return this->text_width( this->text() );
}

int Text::height() const
{
  return this->text_height( this->text() );
}

} // namespace nom
