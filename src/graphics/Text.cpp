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

namespace nom {

Text::Text ( void ) :
  Transformable { Point2i (0, 0), Size2i (0, 0) }, // Our inherited class
  text_size_ ( 14 ),
  color_ ( Color4i::White ),
  style_ ( Text::Style::Regular ),
  alignment_ ( Text::Alignment::TopLeft )
{
  NOM_LOG_TRACE ( NOM );
}

Text::~Text ( void )
{
  NOM_LOG_TRACE ( NOM );
}

Text::Text ( const Text& copy ) :
  Transformable { copy.position(), copy.size() }, // Our inherited class
  font_ { copy.font() },
  texture_ { copy.texture() },
  text_ { copy.text() },
  text_size_ { copy.text_size() },
  color_ { copy.color() },
  style_ { copy.style() },
  alignment_ { copy.alignment() }
{
  NOM_LOG_TRACE ( NOM );
}

Text& Text::operator = ( const Text& other )
{
  this->set_position ( other.position() );
  this->set_size ( other.size() );

  this->font_ = other.font();
  this->texture_ = other.texture();
  this->text_ = other.text();
  this->text_size_ = other.text_size();
  this->color_ = other.color();
  this->style_ = other.style();
  this->alignment_ = other.alignment();

  return *this;
}

Text::Text  ( const std::string& text,
              const IFont& font,
              uint character_size,        // Default parameter
              enum Text::Alignment align // Default parameter
            )  :
  Transformable { Point2i(0, 0), Size2i(0, 0) }, // Our inherited class
  text_ ( text ),
  text_size_ ( character_size ),
  color_ ( Color4i::White ),
  style_ ( Text::Style::Regular )
{
  NOM_LOG_TRACE(NOM);

  this->set_font ( font );
  this->set_alignment ( align );
}

Text::Text  ( const std::string& text,
              const IFont::SharedPtr& font,
              uint character_size,        // Default parameter
              enum Text::Alignment align // Default parameter
            )  :
  Transformable { Point2i(0, 0), Size2i(0, 0) }, // Our inherited class
  text_ ( text ),
  text_size_ ( character_size ),
  color_ ( Color4i::White ),
  style_ ( Text::Style::Regular )
{
  NOM_LOG_TRACE(NOM);

  this->set_font ( *font.get() );
  this->set_alignment ( align );
}

Text::RawPtr Text::get ( void )
{
  return this;
}

IFont::SharedPtr Text::font ( void ) const
{
  return this->font_;
}

const Texture& Text::texture ( void ) const
{
  return this->texture_;
}

bool Text::valid ( void ) const
{
  if ( this->font() != nullptr ) return true;

  return false;
}

enum IFont::FontType Text::type ( void ) const
{
  if ( this->valid() ) return this->font()->type();

  return IFont::FontType::NotDefined;
}

sint Text::text_width ( const std::string& text_string ) const
{
  sint text_width = 0;
  uint32 previous_char = 0; // Kerning calculation
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
  for ( uint32 pos = 0; pos < text_buffer.length() && text_buffer[pos] != '\n'; ++pos )
  {
    // Apply kerning offset
    uint32 current_char = text_buffer[pos];
    text_width += this->font()->kerning ( previous_char, current_char, this->text_size() );
    previous_char = current_char;

    if ( current_char == ' ' ) // ASCII space glyph (32)
    {
      text_width += this->font()->spacing ( this->text_size() );
    }
    else if ( current_char == '\t' ) // Tab character (we indent two spaces)
    {
      text_width += this->font()->spacing ( this->text_size() ) * 2;
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

sint Text::width ( void ) const
{
  return this->text_width ( this->text() );
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

sint Text::height ( void ) const
{
  return this->text_height ( this->text() );
}

const std::string& Text::text ( void ) const
{
  return this->text_;
}

uint Text::text_size ( void ) const
{
  return this->text_size_;
}

const Color4i& Text::color ( void ) const
{
  return this->color_;
}

enum Text::Style Text::style ( void ) const
{
  return this->style_;
}

/*
const Point2i& Text::local_bounds ( void ) const
{
}
*/

/*
const Point2i& Text::global_bounds ( void ) const
{
}
*/

enum Text::Alignment Text::alignment ( void ) const
{
  return this->alignment_;
}

void Text::set_font ( const IFont& font )
{
  this->font_ = std::shared_ptr<IFont>( font.clone() );

  this->font()->set_point_size ( this->text_size() );

  if ( this->valid() == false || this->texture_.create ( this->font()->image (this->text_size()) ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not initialize Text from given IFont" );
    return;
  }
}

void Text::set_text ( const std::string& text )
{
  if ( text != this->text() )
  {
    this->text_ = text;

    // Update logic
  }
}

void Text::set_text_size ( uint character_size )
{
  if ( this->valid() == false ) return;

  if ( this->text_size() != character_size )
  {
    this->text_size_ = character_size;

    this->set_font ( *this->font().get() );
    // Update logic
  }
}

void Text::set_color ( const Color4i& color )
{
  if ( color != this->color() )
  {
    this->color_ = color;
    this->texture_.set_color_modulation ( color );

    // Update logic
  }
}

void Text::set_style ( enum Text::Style style )
{
  // We do not have an atlas map to go from -- nothing to set a style on!
  if ( this->texture_.valid() == false ) return;

  // Style being set is already set -- nothing to do!
  if ( style == this->style() ) return;

  // Set new style if sanity checks pass
  this->style_ = style;

  this->update();
}

void Text::set_alignment ( enum Text::Alignment align )
{
  this->alignment_ = align;

  int x_offset = 0;
  int y_offset = 0;

  switch ( this->alignment() )
  {
    default:
    case Text::Alignment::TopLeft: // Default case
    {
      x_offset = this->position().x;
      y_offset = this->position().y;
      break;
    }

    case Text::Alignment::TopCenter:
    {
      x_offset = this->position().x + ( this->size().w - this->width() ) / 2;
      y_offset = this->position().y;
      break;
    }

    case Text::Alignment::TopRight:
    {
      x_offset = this->position().x + ( this->size().w - this->width() );
      y_offset = this->position().y;
      break;
    }

    case Text::Alignment::MiddleLeft:
    {
      x_offset = this->position().x;
      y_offset = this->position().y + ( this->size().h - this->height() ) / 2;
      break;
    }

    case Text::Alignment::MiddleCenter:
    {
      x_offset = this->position().x + ( this->size().w - this->width() ) / 2;
      y_offset = this->position().y + ( this->size().h - this->height() ) / 2;
      break;
    }

    case Text::Alignment::MiddleRight:
    {
      x_offset = this->position().x + ( this->size().w - this->width() );
      y_offset = this->position().y + ( this->size().h - this->height() ) / 2;
      break;
    }

    case Text::Alignment::BottomLeft:
    {
      x_offset = this->position().x;
      y_offset = this->position().y + ( this->size().h - this->height() );
      break;
    }

    case Text::Alignment::BottomCenter:
    {
      x_offset = this->position().x + ( this->size().w - this->width() ) / 2;
      y_offset = this->position().y + ( this->size().h - this->height() );
      break;
    }

    case Text::Alignment::BottomRight:
    {
      x_offset = this->position().x + ( this->size().w - this->width() );
      y_offset = this->position().y + ( this->size().h - this->height() );
      break;
    }
  } // end switch

  this->set_position (Point2i(x_offset, y_offset) );
}

void Text::draw ( RenderTarget target ) const
{
  // Use coordinates provided by interface user as our starting origin
  // coordinates to compute from
  int x_offset = this->position().x;
  int y_offset = this->position().y;

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
    angle = 12; // 12 degrees as per SDL2_ttf
  }

  uint32 previous_char = 0;
  for ( uint32 pos = 0; pos < text_buffer.length(); pos++ )
  {
    // Apply kerning offset
    uint32 current_char = text_buffer[pos];
    x_offset += this->font()->kerning ( previous_char, current_char, this->text_size() );
    previous_char = current_char;

    if ( current_char == ' ' ) // Space character
    {
      //Move over
      x_offset += this->font()->spacing ( this->text_size() );
    }
    else if( current_char == '\n' || current_char == '\v' ) // Vertical chars
    {
      //Move down and back over to the beginning of line
      y_offset += this->font()->newline ( this->text_size() );
      x_offset = this->position().x;
    }
    else if( current_char == '\t' ) // Tab character (we indent two spaces)
    {
      x_offset += this->font()->spacing ( this->text_size() ) * 2;
    }
    else // The time to render is now!
    {
      this->texture_.set_position ( Point2i ( x_offset, y_offset ) );
      this->texture_.set_bounds ( this->font()->glyph(current_char, this->text_size() ).bounds );

      this->texture_.draw ( target.renderer(), angle );

      // Move over the width of the character with one pixel of padding
      x_offset += ( this->font()->glyph(current_char, this->text_size() ).advance ) + 1;
    } // end else
  } // end for loop
}

void Text::update ( void )
{
  // No font has been loaded -- nothing to draw!
  if ( this->valid() == false ) return;

  switch ( style_ )
  {
    default:
    case Text::Style::Regular: /* Do nothing */ break; // Default

    case Text::Style::Bold:
    {
      if ( this->type() == IFont::FontType::BitmapFont )
      {
        NOM_LOG_ERR ( NOM, "nom::BitmapFont does not support nom::Text::Style::Bold." );
        break;
      }

      // TODO: Bold logic
      break;
    }

    case Text::Style::Italic:
     // This style is handled in Text's draw method
    break;

    case Text::Style::Underlined:
    {
      // TODO: Underline logic
      break;
    }
  } // end switch
}

} // namespace nom
