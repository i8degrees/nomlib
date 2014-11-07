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

// Private headers
#include "nomlib/system/File.hpp"
#include "nomlib/graphics/fonts/BitmapFont.hpp"
#include "nomlib/graphics/fonts/TrueTypeFont.hpp"

namespace nom {

Font::Font() :
  font_(nullptr)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_FONTS, NOM_LOG_PRIORITY_DEBUG);
}

Font::~Font()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_FONTS, NOM_LOG_PRIORITY_DEBUG);
}

Font::Font(const std::shared_ptr<Font::font_type>& font) :
  font_(font)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_FONTS, NOM_LOG_PRIORITY_DEBUG);
}

// Used by nom::Text::set_font and nom::Text::valid.
Font::Font(font_type* font) :
  font_( std::shared_ptr<Font::font_type>(font) )
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_FONTS, NOM_LOG_PRIORITY_DEBUG);
}

Font::font_type* Font::operator ->() const
{
  if( this->font_ != nullptr ) {
    return this->font_.operator->();
  }

  // Not initialized
  return nullptr;
}

bool Font::operator ==(const self_type& rhs) const
{
  return(this->font_ == rhs.font_);
}

bool Font::operator !=(const self_type& rhs) const
{
  return ! (this->font_ == rhs.font_);
}

bool Font::unique() const
{
  return this->font_.unique();
}

IFont::FontType Font::type(const std::string& filename) const
{
  File fp;

  if( fp.extension( filename ) == "png" ) {
    return IFont::FontType::BitmapFont;

  } else if( fp.extension( filename ) == "bmp" ) {
    return IFont::FontType::BitmapFont;

  } else if( fp.extension( filename ) == "ttf" ) {
    return IFont::FontType::TrueTypeFont;

  } else if( fp.extension( filename ) == "ttc" ) {  // TrueType Collection
    return IFont::FontType::TrueTypeFont;

  } else if( fp.extension( filename ) == "otf" ) {  // OpenType
    return IFont::FontType::TrueTypeFont;

  } else if( fp.extension( filename ) == "dfont" ) {  // Data Fork Suitcase
    return IFont::FontType::TrueTypeFont;
  }

  // No matching file extension found
  return IFont::FontType::NotDefined;
}

bool Font::load(const std::string& filename)
{
  this->font_ = nullptr;

  // Initialize the font interface
  if( this->type( filename ) == IFont::FontType::BitmapFont ) {
    // this->font_ = std::make_shared<BitmapFont>( BitmapFont() );
    this->font_ = std::shared_ptr<BitmapFont>( new BitmapFont() );
  } else if( this->type( filename ) == IFont::FontType::TrueTypeFont ) {
    // this->font_ = std::make_shared<TrueTypeFont>( TrueTypeFont() );
    this->font_ = std::shared_ptr<TrueTypeFont>( new TrueTypeFont() );

  } else {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not determine the font type for ", filename );
    return false;
  }

  NOM_ASSERT(this->font_ != nullptr);

  if( this->font_ != nullptr ) {
    return this->font_->load(filename);
  }

  return false;
}

} // namespace nom
