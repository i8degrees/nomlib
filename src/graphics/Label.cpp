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
#include "nomlib/graphics/Label.hpp"

namespace nom {

Label::Label ( void ) :
  font_ ( nullptr ), text_ (), text_size_ ( 14 ),
  color_ { NOM_COLOR4U_WHITE },
  style_ ( Label::FontStyle::Regular ),
  alignment_ ( Label::TextAlignment::MiddleLeft )
{
  NOM_LOG_TRACE ( NOM );
}

Label::~Label ( void )
{
  NOM_LOG_TRACE ( NOM );
}

Label::Label ( /*const*/ IFont& font )  :
//Label::Label ( IFont& font )  :
  font_ { &font },
  text_ (), text_size_ ( 14 ),
  color_ { NOM_COLOR4U_WHITE },
  style_ ( Label::FontStyle::Regular ),
  alignment_ ( Label::TextAlignment::MiddleLeft )
{
  NOM_LOG_TRACE ( NOM );
}

Label::RawPtr Label::get ( void )
{
  return this;
}

IFont::RawPtr Label::font ( void ) const
{
  return this->font_.get();
}

bool Label::valid ( void ) const
{
  if ( this->font() != nullptr ) return true;

  return false;
}

enum IFont::FileType Label::type ( void ) const
{
  if ( this->valid() ) return this->font()->type();

  return IFont::FileType::NotDefined;
}

int Label::width ( void ) const
{
  int text_width = 0;
  std::string text_buffer = this->text();

  if ( this->valid() == false )
  {
    NOM_LOG_ERR( NOM, "Invalid label font for width calculation" );
    return -1;
  }

  for ( uint32 char_pos = 0; char_pos < text_buffer.length(); ++char_pos )
  {
    if ( text_buffer[ char_pos ] == ' ' )
    {
      text_width += this->font()->spacing();

      // Dump each character's table used for calculation
      #if defined (NOM_DEBUG_LABEL)
        NOM_DUMP_VAR ( char_pos );
        NOM_DUMP_VAR ( text_buffer[ char_pos ] );
        NOM_DUMP_VAR ( text_width );
      #endif

    }
    else if ( text_buffer[ char_pos ] == '\n' ) // TODO
    {
      text_width = 0;
    }
    else
    {
      text_width += this->font()->glyph(text_buffer[ char_pos ]).width + 1;

      // Dump each character's table used for calculation
      #if defined (NOM_DEBUG_LABEL)
        NOM_DUMP_VAR ( char_pos );
        NOM_DUMP_VAR ( text_buffer[ char_pos ] );
        NOM_DUMP_VAR ( this->font()->glyph(text_buffer[ char_pos ]).width + 1 );
      #endif

    }
  } // end for loop

  #if defined (NOM_DEBUG_LABEL)
    NOM_DUMP_VAR ( text_width );
  #endif

  return text_width;
}

int Label::height ( void ) const
{
  int text_height = 0;
  std::string text_buffer = this->text();

  if ( this->valid() == false )
  {
    NOM_LOG_ERR( NOM, "Invalid label font for height calculation" );
    return -1;
  }

  for ( uint32 char_pos = 0; char_pos < text_buffer.length(); ++char_pos )
  {
    if ( text_buffer[ char_pos ] == '\n' )
    {
      text_height += this->font()->newline();

      // Dump each character's table used for calculation
      #if defined (NOM_DEBUG_LABEL)
        NOM_DUMP_VAR ( char_pos );
        NOM_DUMP_VAR ( text_buffer[ char_pos ] );
        NOM_DUMP_VAR ( text_height );
      #endif

    }
    else
    {
      text_height = this->font()->glyph(text_buffer[ char_pos ]).height;

      // Dump each character's table used for calculation
      #if defined (NOM_DEBUG_LABEL)
        NOM_DUMP_VAR ( char_pos );
        NOM_DUMP_VAR ( text_buffer[ char_pos ] );
        NOM_DUMP_VAR ( this->font()->glyph(text_buffer[ char_pos ]).height );
      #endif
    }
  }

  #if defined (NOM_DEBUG_LABEL)
    NOM_DUMP_VAR ( text_height );
  #endif

  return text_height;
}

const std::string& Label::text ( void ) const
{
  return this->text_;
}

uint Label::text_size ( void ) const
{
  return this->text_size_;
}

const Color4u& Label::color ( void ) const
{
  return this->color_;
}

enum Label::FontStyle Label::style ( void ) const
{
  return this->style_;
}

/*
const Point2i& Label::local_bounds ( void ) const
{
}
*/

/*
const Point2i& Label::global_bounds ( void ) const
{
}
*/

enum Label::TextAlignment Label::alignment ( void ) const
{
  return this->alignment_;
}

//void Label::set_font ( IFont& font )
void Label::set_font ( /*const*/ IFont& font )
{
  if ( this->font() != &font )
  {
    this->font_ = std::shared_ptr<IFont>(font.clone() );
    this->render_font_.initialize ( this->font()->image() );

    // Update logic
  }
}

void Label::set_text ( const std::string& text )
{
  if ( text != this->text() )
  {
    this->text_ = text;

    // Update logic
  }
}

void Label::set_text_size ( uint character_size )
{
  if ( character_size != this->text_size() )
  {
    this->text_size_ = character_size;

    // Update logic
  }
}

void Label::set_color ( const Color4u& color )
{
  if ( color != this->color() )
  {
    this->color_ = color;
    this->render_font_.set_color_modulation ( color );

    // Update logic
  }
}

void Label::set_style ( enum Label::FontStyle style )
{
  // We do not have an atlas map to go from -- nothing to set a style on!
  if ( ! this->render_font_.valid() ) return;

  // Style being set is already set -- nothing to do!
  if ( style == this->style() ) return;

  // Set new style if sanity checks pass
  this->style_ = style;

  this->update();

}

void Label::set_alignment ( enum Label::TextAlignment align )
{
  if ( align != this->alignment() )
  {
    this->alignment_ = align;

    // Update logic
  }
}

void Label::draw ( RenderTarget target ) const
{
  // No font has been loaded -- nothing to draw!
  if ( ! this->valid() )
  {
    NOM_LOG_ERR( NOM, "Invalid label font" );
    return;
  }

  // Text string to draw is empty -- nothing to draw!
  if ( this->text().length() < 1 ) return;

  // Use coordinates provided by interface user as our starting origin
  // coordinates to compute from
  int32 x_offset = this->position_.x;
  int32 y_offset = this->position_.y;

  switch ( this->alignment() )
  {
    default:
    case Label::TextAlignment::MiddleLeft:
    {
      x_offset = this->position_.x;
      y_offset = this->position_.y;
      break;
    }

    case Label::TextAlignment::MiddleCenter:
    {
      x_offset = this->position_.x + ( this->position_.w / 2 ) - ( this->width() / 2 );
      y_offset = this->position_.y + ( this->position_.h / 2 ) - ( this->height() / 2 );
      break;
    }

    case Label::TextAlignment::MiddleRight:
    {
      // TODO
      break;
    }

  } // end switch

  for ( uint32 show = 0; show < this->text().length(); show++ )
  {
    //If the current character is a space
    if ( this->text()[show] == ' ' )
    {
      //Move over
      x_offset += this->font()->spacing();
    }

    // If the current character is a newline
    else if( this->text()[show] == '\n' )
    {
      //Move down and back over to the beginning of line
      y_offset += this->font()->newline();
      x_offset = this->position_.x;
    }
    // If the current character is a tab
    else if( this->text()[show] == '\t' )
    {
      x_offset += this->font()->spacing();
    }
    else
    {
      //Get the ASCII value of the character
      uint32 ascii = static_cast<uint32>( this->text()[show] );

      this->render_font_.set_position ( Point2i ( x_offset, y_offset ) );
      this->render_font_.set_bounds ( this->font()->glyph(ascii) );

      this->render_font_.draw ( target.renderer() );

      // Move over the width of the character with one pixel of padding
      x_offset += ( this->font()->glyph(ascii).width ) + 1;
    } // end else
  } // end for loop
}

bool Label::resize ( enum Texture::ResizeAlgorithm scaling_algorithm )
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

void Label::update ( void )
{
  // No font has been loaded -- nothing to draw!
  if ( this->valid() == false ) return;

  // Text string to draw is empty -- nothing to draw!
  if ( this->text().length() < 1 ) return;

  //this->font_->update();
  switch ( style_ )
  {
    default: break;

    case FontStyle::Regular:
    case FontStyle::Bold:
    case FontStyle::Italic:
    case FontStyle::Underlined:
      // Do nothing stub
    break;

    /// Text effect utilizing alpha channels for the appearance of gray text
    case FontStyle::Faded:
    {
      if ( this->render_font_.set_alpha ( 150 ) == true )
      {
        this->style_ = Label::FontStyle::Faded;
      }
      break;
    }
  } // end switch
}

} // namespace nom
