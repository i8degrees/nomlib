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
#include <SDL.h>
#include <SDL_ttf.h>

// Private headers
#include "nomlib/graphics/fonts/Glyph.hpp"
#include "nomlib/graphics/shapes/Rectangle.hpp"

// #define NOM_OLD_TEXT_RENDER

namespace nom {

Text::Text( void ) :
  Transformable(Point2i::zero, Size2i::null), // Base class
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
  Transformable(Point2i::zero, Size2i::null), // Base class
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
  Transformable(Point2i::zero, Size2i::null), // Base class
  text_ ( text ),
  text_size_( character_size ),
  style_( Text::Style::Normal ),
  dirty_(false)
{
  // NOM_LOG_TRACE( NOM );

  this->set_font( font );
  this->set_color( text_color );
}

Text::Text(const self_type& rhs) :
  Transformable( rhs.position(), rhs.size() ),
  font_(rhs.font_),
  glyphs_texture_(rhs.glyphs_texture_),
  text_(rhs.text_),
  text_size_(rhs.text_size_),
  color_(rhs.color_),
  style_(rhs.style_),
  dirty_(rhs.dirty_)
{
  // NOM_LOG_TRACE( NOM );

  if( this->rendered_text_ != nullptr ) {
    this->rendered_text_.reset( new Texture(*rhs.rendered_text_) );
  }
}

Text::self_type& Text::operator =(const self_type& rhs)
{
  Transformable::set_position( rhs.position() );
  Transformable::set_size( rhs.size() );
  this->font_ = rhs.font_;
  this->glyphs_texture_ = rhs.glyphs_texture_;

  if( this->rendered_text_ != nullptr ) {
    this->rendered_text_.reset( new Texture(*rhs.rendered_text_) );
  }

  this->text_ = rhs.text_;
  this->text_size_ = rhs.text_size_;
  this->color_ = rhs.color_;
  this->style_ = rhs.style_;
  this->dirty_ = rhs.dirty_;

  return *this;
}

void Text::set_position(const Point2i& pos)
{
  Transformable::set_position(pos);

  if( this->rendered_text_ != nullptr &&
      this->rendered_text_->valid() == true )
  {
    this->rendered_text_->set_position( this->position() );
  }
}

Text* Text::clone() const
{
  return( new self_type(*this) );
}

ObjectTypeInfo Text::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

const Font& Text::font() const
{
  return this->font_;
}

Texture* Text::texture() const
{
  // NOTE: It is necessary to always return a new instance because the stored
  // texture may be reallocated at any time, i.e.: glyph rebuild from
  // point size modification -- leaving the end-user with a dangling pointer!
  return( new Texture(*this->rendered_text_) );
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

int Text::text_width(const std::string& text_buffer) const
{
  int text_width = 0;
  int max_text_width = 0;

  // Ensure that our font pointer is still valid
  if( this->valid() == false ) {
    NOM_LOG_ERR( NOM, "Invalid font for width calculation" );
    return text_width;
  }

  for( auto pos = 0; pos < text_buffer.length(); ++pos ) {

    uint32 current_char = text_buffer[pos];

    if( current_char == '\n' ) {

      max_text_width =
        std::max( max_text_width, this->multiline_width(text_buffer, pos) );
    } else {

      // Printable ASCII glyph; 33..127
      text_width =
        std::max(text_width, this->multiline_width(text_buffer, pos) );
    }
  } // end for loop

  return( std::max(text_width, max_text_width) );
}

// Private call
int
Text::multiline_width(const std::string& text_buffer, nom::size_type pos) const
{
  int text_width = 0;
  int max_text_width = 0;
  uint32 previous_kerning_char = 0;
  int kerning_offset = 0;
  nom::size_type text_buffer_end = text_buffer.length();

  nom::size_type next_newline =
    text_buffer.find('\n', pos + 1);

  if( next_newline != std::string::npos ) {
    text_buffer_end = next_newline;
  } else {
    // Use default initialized result
  }

  for(  auto character_pos = pos;
        character_pos != text_buffer_end;
        ++character_pos )
  {
    uint32 current_char = text_buffer[character_pos];

    kerning_offset =
      this->font()->kerning(  previous_kerning_char, current_char,
                              this->text_size() );

    if( kerning_offset != nom::int_min ) {
      text_width += kerning_offset;
    }

    previous_kerning_char = current_char;

    if( current_char == ' ' ) {

      // ASCII 32; space glyph
      text_width +=
        this->font()->spacing( this->text_size() );

    } else if( current_char == '\t' ) {

      // Tab character; indent two space glyphs
      text_width +=
        this->font()->spacing( this->text_size() ) * 2;

    } else if( current_char == '\n' ) {
      // Do nothing
    } else {

      text_width +=
        this->font()->glyph(current_char, this->text_size() ).advance + 1;
    }

    max_text_width = std::max(text_width, max_text_width);
  }

  // Padding for rendered_text_
  // max_text_width += this->font()->spacing( this->text_size() );

  return max_text_width;
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
  RenderWindow* context = nom::render_interface();

  // Default pixel format used when context is NULL
  uint32 pixel_format = SDL_PIXELFORMAT_ARGB8888;

  if ( this->valid() == false )
  {
    NOM_LOG_ERR( NOM, "Could not set text size: the font is invalid." );
    return;
  }

  this->text_size_ = character_size;

  // Force a rebuild of the texture atlas at the specified point size
  this->font()->set_point_size( this->text_size() );

  if( context != nullptr ) {
    RendererInfo caps = context->caps();
    pixel_format = caps.optimal_texture_format();
  }

  // We can only (sanely) initialize our texture once we know the text size; we
  // will update this texture only as it becomes necessary (see ::update).
  if( this->glyphs_texture_.create( *this->font()->image( this->text_size() ), pixel_format, Texture::Access::Streaming ) == false ) {
    NOM_LOG_ERR(  NOM, "Could not create texture atlas at point size:",
                  std::to_string( this->text_size() ) );
    return;
  }

  if( this->rendered_text_ == nullptr ) {
    this->rendered_text_.reset( new Texture() );
  }

  NOM_ASSERT(this->rendered_text_ != nullptr);
  if( this->rendered_text_ == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not create texture cache:",
                  "failed to allocate texture memory." );
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

void Text::set_text_kerning(bool state)
{
  this->font()->set_font_kerning(state);

  this->dirty_ = true;
  this->update();
}

void Text::draw(RenderTarget& target) const
{

#if defined(NOM_OLD_TEXT_RENDER)
  if( this->valid() == true ) {
    this->render_text(target);
  }
#else
  if( this->rendered_text_ != nullptr &&
      this->rendered_text_->valid() == true )
  {
    this->rendered_text_->draw(target);
  }
#endif
}

// Private scope

void Text::render_text(RenderTarget& target) const
{
  int kerning_offset = 0;
  uint32 previous_char = 0;
  uint32 current_char = 0;

  // Our position should always start from zero, when rendering text to a
  // rendering target (glyphs_texture_ -> rendered_text_), so that updating
  // text positions only effect the rendered text.
#if defined(NOM_OLD_TEXT_RENDER)
  Point2i pos( this->position() );
#else
  Point2i pos(Point2i::zero);
#endif
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
      IntRect glyph_bounds =
        this->font()->glyph( current_char, this->text_size() ).bounds;

      Size2i tex_dims;
      tex_dims.w = glyph_bounds.w;
      tex_dims.h = glyph_bounds.h;

      // Apply rendering offsets; applicable to nom::BMFont glyphs
      tex_pos =
        Point2i(pos.x + glyph_offset.x, pos.y + glyph_offset.y);
      this->glyphs_texture_.set_position(tex_pos);

      this->glyphs_texture_.set_bounds(glyph_bounds);
      this->glyphs_texture_.set_size(tex_dims);

      this->glyphs_texture_.draw(target.renderer(), angle);

      // Move over the width of the character with one pixel of padding
      pos.x += ( this->font()->glyph(current_char, this->text_size() ).advance ) + 1;

    } // end else
  } // end for loop

// Debugging overlay to show the bounds of the overall text rendering
#if 0
    IntRect text_overlay;
    text_overlay.x = this->position().x;
    text_overlay.y = this->position().y;
    text_overlay.w = this->size().w;
    text_overlay.h = this->size().h;
    Color4i text_overlay_color = Color4i(151, 161, 225, 128);
    Rectangle text_bounds(text_overlay, text_overlay_color);
    text_bounds.draw(target);
#endif
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

  if( style & Text::Style::Underline ) {
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
  // This is an expensive call.
  const Image* source = this->font()->image( this->text_size() );

  NOM_ASSERT(source != nullptr);
  if( source->valid() == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update glyphs texture: invalid image source." );
    return;
  }

  // Expensive call
  if( glyphs_texture_.lock() == true ) {
    glyphs_texture_.copy_pixels( source->pixels(), source->pitch() * source->height() );
    glyphs_texture_.unlock();
  } else {
    NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION,
                "Could not update glyphs texture: could not lock texture." );
    return;
  }

  // NOTE: By **not** preserving the alpha channel here, we introduce a bug that
  // can clip off the edges of certain glyphs when using kerning -- as can be
  // seen in the letter 'A' of the 'Kerning' tests of BMFont and TrueTypeFont.
  //
  // The downside to blending is that we lose the original pixel data around the
  // edges of text; the input color gets blended in with the destination color.
  this->glyphs_texture_.set_blend_mode(SDL_BLENDMODE_BLEND);

  // Update the font's text color.
  this->glyphs_texture_.set_color_modulation( this->color() );

  // Set the overall size of this text label to the width & height of the text,
  // with consideration to the font.
  this->set_size( Size2i( this->width(), this->height() ) );

#if ! defined(NOM_OLD_TEXT_RENDER)
  if( this->size().w > 0 && this->size().h > 0 ) {
    // Expensive call
    this->update_cache();
  }
#endif
}

int Text::width() const
{
  return this->text_width( this->text() );
}

int Text::height() const
{
  return this->text_height( this->text() );
}

bool Text::update_cache()
{
  // Our cached texture dimensions should always be the same as the rendered
  // text's dimensions; if the rendered text appears cut off, the calculated
  // text width or height is likely to blame!
  Size2i texture_dims(Size2i::zero);
  texture_dims = this->size();
  // Padding
  // texture_dims.w += this->text_width(" ");

  RenderWindow* context = nom::render_interface();
  NOM_ASSERT(context != nullptr);
  if( context == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION, "Could not update cache",
                  "invalid renderer." );
    return false;
  }

  // Obtain the optimal pixel format for the platform
  RendererInfo caps = context->caps();

  if( this->rendered_text_->size() != texture_dims ) {

    // Poor man's counter of how often we are re-allocating this texture
    // NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_RENDER, NOM_LOG_PRIORITY_DEBUG);
    // NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_RENDER,
    //                 "old_size:", this->rendered_text_->size(),
    //                 "new_size:", texture_dims );

    if( this->rendered_text_->initialize( caps.optimal_texture_format(),
        SDL_TEXTUREACCESS_TARGET, texture_dims ) == false )
    {
      NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                    "Could not update cache: failed texture creation." );
      return false;
    }
  }

  // Use an alpha channel; otherwise the text is rendered on a black
  // background!
  this->rendered_text_->set_blend_mode(SDL_BLENDMODE_BLEND);

  // Set the destination (screen) positioning of the rendered text
  this->rendered_text_->set_position( this->position() );

  if( context->set_render_target( this->rendered_text_.get() ) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update cache: render targets not supported." );
    return false;
  }

  // Clear the rendering backdrop color to be fully transparent; this preserves
  // any existing alpha channel data from the rendered text
  if( context->fill(Color4i::Transparent) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update cache:",
                  "failed to set the render target's color." );
    return false;
  }

  this->render_text(*context);

  if( context->reset_render_target() == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update cache:",
                  "failed to reset the rendering target." );
    return false;
  }

  return true;
}

} // namespace nom
