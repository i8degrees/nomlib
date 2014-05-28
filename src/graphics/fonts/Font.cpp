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
#include "nomlib/graphics/fonts/Font.hpp"

namespace nom {

Font::Font( void ) :
  font_{ nullptr },
  sharable_{ false }
{
  // NOM_LOG_TRACE( NOM );
}

Font::~Font( void )
{
  // NOM_LOG_TRACE( NOM );
}

// Font::Font( const font_type& font ) :
//   font_{ font.clone() }, // FIXME
//   sharable_{ false }
// {
//   NOM_LOG_TRACE( NOM );
// }

Font::Font( const value_type& font ) :
  font_{ font },
  sharable_{ false }
{
  // NOM_LOG_TRACE( NOM );
}

// Font::Font( font_type* font ) :
//   font_( value_type( font ) )
// {
//   NOM_LOG_TRACE( NOM );
// }

Font::self_type& Font::operator =( const self_type* rhs )
{
  this->font_ = rhs->font_;
  this->set_sharable( rhs->sharable() );

  return *this;
}

// const Font::font_type& Font::operator *( void ) const
// {
//   return *this->font_;
// }

// Font::font_type& Font::operator *( void )
// {
//   this->share();

//   return *this->font_;
// }

const Font::font_type* Font::operator ->( void ) const
{
  return this->font_.operator->();
}

Font::font_type* Font::operator ->( void )
{
  this->share();

  return this->font_.operator->();
}

bool Font::operator ==( const self_type& rhs ) const
{
  return ( this->font_ == rhs.font_ );
}

bool Font::operator !=( const self_type& rhs ) const
{
  return ! ( this->font_ == rhs.font_ );
}

bool Font::unique( void ) const
{
  return this->font_.unique();
}

bool Font::valid( void ) const
{
  if( this->font_.get() != nullptr )
  {
    return true;
  }

  return false;
}

bool Font::sharable( void ) const
{
  return this->sharable_;
}

IFont::FontType Font::type( const std::string& filename ) const
{
  File fp;

  if( fp.extension( filename ) == "png" )
  {
    return IFont::FontType::BitmapFont;
  }
  else if( fp.extension( filename ) == "bmp" )
  {
    return IFont::FontType::BitmapFont;
  }
  else if( fp.extension( filename ) == "ttf" )
  {
    return IFont::FontType::TrueTypeFont;
  }
  else if( fp.extension( filename ) == "ttc" )
  {
    return IFont::FontType::TrueTypeFont;
  }
  else if( fp.extension( filename ) == "otf" )
  {
    return IFont::FontType::TrueTypeFont;
  }

  return IFont::FontType::NotDefined;
}

bool Font::load( const std::string& filename )
{
  File fp;

  // this->share();

  if( this->type( filename ) == IFont::FontType::BitmapFont )
  {
    this->font_ = value_type( new BitmapFont() );
  }
  else if( this->type( filename ) == IFont::FontType::TrueTypeFont )
  {
    this->font_ = value_type( new TrueTypeFont() );
  }
  else
  {
    this->font_ = nullptr;

    NOM_LOG_ERR( NOM, "Could not determine the font type for: " + filename );

    return false;
  }

  NOM_ASSERT( this->font_ );

  return this->font_->load( filename );
}

bool Font::set_point_size( int point_size )
{
  NOM_ASSERT( this->font_ );

  // this->share();

  return this->font_->set_point_size( point_size );
}

void Font::set_hinting( int type )
{
  NOM_ASSERT( this->font_ );

  // this->share();

  this->font_->set_hinting( type );
}

bool Font::set_outline( int outline )
{
  NOM_ASSERT( this->font_ );

  // this->share();

  return this->font_->set_outline( outline );
}

void Font::set_sharable( bool state )
{
  this->sharable_ = state;
}

void Font::share( void )
{
  NOM_ASSERT( this->font_ );

  IFont* data = this->font_.get();

  if( data != nullptr && this->unique() == false )
  {
    if( this->sharable() == true )
    {
      this->font_ = value_type( data->clone() );

      // Debugging aids
      if( this->font_->type() == IFont::FontType::BitmapFont )
      {
        #if defined( NOM_DEBUG_FONT_SHARING )
          NOM_DUMP_EXT("Font::font_face", this->font_->metrics().name + " (BitmapFont)" );
        #endif
      }
      else if( this->font_->type() == IFont::FontType::TrueTypeFont )
      {
        #if defined( NOM_DEBUG_FONT_SHARING )
          NOM_DUMP_EXT("Font::font_face", this->font_->metrics().name + " (TrueTypeFont)" );
        #endif
      }
      else
      {
        #if defined( NOM_DEBUG_FONT_SHARING )
          NOM_DUMP_EXT("Font::font_face", this->font_->metrics().name + " (Undefined)" );
        #endif
      }
    } // end if sharable
  } // end if the font is NOT unique
}

} // namespace nom
