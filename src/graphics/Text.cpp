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

Text::Text( void ) :
  Transformable { Point2i::null, Size2i::null }, // Base class
  text_size_ ( nom::DEFAULT_FONT_SIZE ),
  color_ ( Color4i::White ),
  style_ ( Text::Style::Normal ),
  alignment_ ( Text::Alignment::TopLeft )
{
  // NOM_LOG_TRACE( NOM );
}

Text::~Text( void )
{
  // NOM_LOG_TRACE( NOM );
}

Text::Text ( const self_type& copy ) :
  Transformable { copy.position(), copy.size() }, // Our inherited class
  font_{ copy.font() },
  texture_ { copy.texture() },
  text_ { copy.text() },
  text_size_ { copy.text_size() },
  color_ { copy.color() },
  style_ { copy.style() },
  alignment_ { copy.alignment() },
  features_{ copy.features() }
{
  // NOM_LOG_TRACE( NOM );

  // Set the overall size of this text label to the width & height of the text,
  // with consideration to the specific font in use.
  // this->set_size( Size2i( this->text_width( copy.text() ), this->text_height( copy.text() ) ) );
}

Text::self_type& Text::operator = ( const self_type& other )
{
  this->set_position ( other.position() );

  this->set_size ( other.size() );

  // Set the overall size of this text label to the width & height of the text,
  // with consideration to the specific font in use.
  // this->set_size( Size2i( this->text_width( other.text() ), this->text_height( other.text() ) ) );

  this->font_ = other.font();

  this->texture_ = other.texture();
  this->text_ = other.text();
  this->text_size_ = other.text_size();
  this->color_ = other.color();
  this->style_ = other.style();
  this->alignment_ = other.alignment();
  this->features_ = other.features();

  return *this;
}

Text::Text  (
              const std::string& text,
              const Font& font,
              uint character_size,        // Default parameter
              enum Text::Alignment align, // Default parameter
              const Color4i& text_color
            )  :
  Transformable { Point2i::null, Size2i::null }, // Base class
  text_ ( text ),
  text_size_( character_size ),
  style_( Text::Style::Normal )
{
  // NOM_LOG_TRACE( NOM );

  this->set_font( font );
  this->set_color( text_color );
  this->set_alignment ( align );
}

IDrawable::raw_ptr Text::clone( void ) const
{
  return Text::raw_ptr( new Text( *this ) );
}

ObjectTypeInfo Text::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

Text::raw_ptr Text::get( void )
{
  return this;
}

Text::font_type& Text::font( void ) const
{
  return this->font_;
}

const Texture& Text::texture ( void ) const
{
  return this->texture_;
}

bool Text::valid ( void ) const
{
  return this->font().valid();
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

uint32 Text::style( void ) const
{
  return this->style_;
}

/*
const Point2i& Text::local_bounds ( void ) const
{
}
*/

IntRect Text::global_bounds( void ) const
{
  IntRect bounds;

  // Positioning coordinates as would be rendered on-screen
  bounds.x = this->position().x;
  bounds.y = this->position().y;

  // Text width and height, in pixels, with respect to set font
  bounds.w = this->width();
  bounds.h = this->height();

  return bounds;
}

enum Text::Alignment Text::alignment ( void ) const
{
  return this->alignment_;
}

uint32 Text::features( void ) const
{
  return this->features_;
}

void Text::set_font( const Text::font_type& font )
{
  this->font_ = font;

  this->set_text_size( this->text_size() );
}

void Text::set_font( Text::font_type* font )
{
  this->font_ = *font;

  this->set_text_size( this->text_size() );

  // Set the overall size of this text label to the width & height of the text,
  // with consideration to the specific font in use.
  // this->set_size( Size2i( this->text_width( this->text() ), this->text_height( this->text() ) ) );
}

void Text::set_text ( const std::string& text )
{
  if( this->font_.valid() == false )
  {
    NOM_LOG_ERR( NOM, "Could not set text string; the font resource is invalid." );
    return;
  }

  if ( text != this->text() )
  {
    this->text_ = text;

    // Update logic

    // Set the overall size of this text label to the width & height of the text,
    // with consideration to the specific font in use.
    // this->set_size( Size2i( this->text_width( text ), this->text_height( text ) ) );
  }
}

void Text::set_text_size ( uint character_size )
{
  if ( this->valid() == false ) return;

  this->text_size_ = character_size;

  // Force a rebuild of the texture atlas at the specified point size
  this->font()->set_point_size( this->text_size() );

  if ( this->valid() == false || this->texture_.create ( this->font()->image (this->text_size()) ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not initialize Text from given IFont" );
    return;
  }

  // Set the overall size of this text label to the width & height of the text,
  // with consideration to the specific font in use.
  // this->set_size( Size2i( this->text_width( this->text() ), this->text_height( this->text() ) ) );
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

void Text::set_style( uint32 style )
{
  // We do not have an atlas map to go from -- nothing to set a style on!
  if( this->texture_.valid() == false ) return;

  // FIXME: The conditional if statement for ensuring that we don't waste time
  // rebuilding glyph metrics for the same font style breaks when used with GUI
  // widgets.
  // if( this->style() == style )
  // {
  //   return;
  // }

  // Set new style if sanity checks pass
  this->style_ = style;

  if( style & Text::Style::Normal )
  {
    this->font()->set_font_style( TTF_STYLE_NORMAL );
  }

  if( style & Text::Style::Bold )
  {
    this->font()->set_font_style( TTF_STYLE_BOLD );
  }

  if( style & Text::Style::Italic )
  {
    this->font()->set_font_style( TTF_STYLE_ITALIC );
  }

  if( style & Text::Style::Underlined )
  {
    this->font()->set_font_style( TTF_STYLE_UNDERLINE );
  }

  if( this->valid() == false || this->texture_.create( this->font()->image( this->text_size() ) ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not initialize Text from given IFont" );
    return;
  }

  // this->update();
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

void Text::draw ( RenderTarget& target ) const
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
  for ( uint32 pos = 0; pos < text_buffer.length(); ++pos )
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

      // Prevent rendering of text that is longer than its contained size;
      // this generally must be set by the developer.
      if( this->features() & ExtraRenderingFeatures::CropText &&
          this->size() != Size2i::null
        )
      {
        // Maximal cropping bounds
        int bounds = this->position().x + this->size().w;

        if( x_offset >= bounds ) break;
      }

    } // end else
  } // end for loop
}

bool Text::resize ( enum Texture::ResizeAlgorithm scaling_algorithm )
{
  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "Video surface is invalid." );
    return false;
  }
/* TODO: (an implementation in nom::Image)
  if ( this->bitmap_font.resize ( scaling_algorithm ) == false )
  {
NOM_LOG_ERR ( NOM, "Failed to resize the video surface." );
    return false;
  }
TODO */

/* TODO (implement in IFont, BitmapFont, TrueTypeFont classes)
  if ( this->font_->build() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not build bitmap font metrics" );
    return false;
  }
TODO */

  return true;
}

void Text::set_features( uint32 flags )
{
  this->features_ = flags;
}

void Text::update ( void )
{
  // No font has been loaded -- nothing to draw!
  if ( this->valid() == false ) return;

  // FIXME: This never gets called..!
  if( features() & ExtraRenderingFeatures::CropText )
  {
    // this->set_alignment( alignment() );
  }
}

} // namespace nom
