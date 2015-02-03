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
#include "nomlib/graphics/fonts/BMFont.hpp"

// Private headers
#include "nomlib/graphics/RenderWindow.hpp"
#include "nomlib/graphics/RendererInfo.hpp"

namespace nom {

std::ostream& operator <<(std::ostream& os, const BMFontKerningPair& k)
{
  os
  << "kerning "
  << "first="
  << k.first_char_id
  << " second="
  << k.second_char_id
  << " amount="
  << k.x_offset;

  return os;
}

BMFont::BMFont() :
  page_size_(16,16),
  point_size_(nom::DEFAULT_FONT_SIZE),
  use_kerning_(true)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_FONTS, NOM_LOG_PRIORITY_VERBOSE);
}

BMFont::~BMFont()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_FONTS, NOM_LOG_PRIORITY_VERBOSE);
}

IFont* BMFont::clone() const
{
  return( new self_type(*this) );
}

bool BMFont::valid() const
{
  NOM_ASSERT(this->pages_[0].texture != nullptr);

  if(this->pages_[0].texture != nullptr) {
    return this->pages_[0].texture->valid();
  }

  return false;
}

IFont::FontType BMFont::type() const
{
  return IFont::FontType::BMFont;
}

const Image* BMFont::image(uint32 character_size) const
{
  return this->pages_[0].texture.get();
}

const Glyph& BMFont::glyph(uint32 codepoint, uint32 character_size) const
{
  GlyphAtlas& glyphs = this->pages_[0].glyphs;

  GlyphAtlas::const_iterator it = glyphs.find(codepoint);

  if( it != glyphs.end() ) {
    // Found a match
    return it->second;
  }
  else {
    // Err; probably going to receive an empty glyph...
    return glyphs[codepoint];
  }
}

int BMFont::spacing(uint32 character_size) const
{
  return this->pages_[0].glyphs[32].advance;
}

int BMFont::kerning(uint32 first_char, uint32 second_char, uint32 character_size) const
{
  int first = 0;
  int second = 0;

  // Possible FIXME: BMFontTest::KerningParserSanity fails here if we do
  // validity check
  // if( this->valid() == false ) {
  //   return nom::int_min;
  // }

  if( this->use_kerning() == false ) {
    return 0;
  }

  for( auto itr = this->kernings_.begin(); itr != this->kernings_.end(); ++itr ) {

    first = itr->first_char_id;
    second = itr->second_char_id;

    // Found matching kerning pair
    if( first_char == first && second_char == second ) {
      return itr->x_offset;
    }
  }

  return 0;
}

int BMFont::hinting() const
{
  return 0; // Not implemented
}

uint32 BMFont::font_style() const
{
  return 0; // Not implemented
}

const Size2i& BMFont::page_size(uint32 page) const
{
  return this->page_size_;
}

int BMFont::newline(uint32 character_size) const
{
  return this->metrics_.newline;
}

const FontMetrics& BMFont::metrics() const
{
  return this->metrics_;
}

int BMFont::point_size() const
{
  return this->point_size_;
}

bool BMFont::use_kerning() const
{
  return this->use_kerning_;
}

bool BMFont::set_point_size(int)
{
  // Not implemented
  return false;
}

bool BMFont::set_hinting(int)
{
  // Not implemented
  return false;
}

bool BMFont::set_outline(int)
{
  // Not implemented
  return false;
}

void BMFont::set_font_style(uint32)
{
  // Not implemented
}

void BMFont::set_font_kerning(bool state)
{
  this->use_kerning_ = state;
}

bool BMFont::load(const std::string& filename)
{
  std::ifstream fp;

  fp.open(filename);

  if( fp.is_open() == false || fp.good() == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not open BMFont file:", filename );
    return false;
  }

  if( this->parse_ascii_file(fp) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not parse BMFont file:", filename );
    return false;
  }

  // Attempt to build font metrics
  if( this->build(0) == false ) {
    NOM_LOG_ERR ( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not build BMFont metrics" );
    return false;
  }

  return true;
}

bool BMFont::build(uint32 character_size)
{
  RendererInfo caps = nom::render_interface()->caps();

  if( this->pages_[0].texture->load(nom::file_root() + this->page_.filename, caps.optimal_texture_format() ) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not load BMFont texture source:",
                  this->page_.filename );
    return false;
  }

  if( this->pages_[0].texture->valid() == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "BMFont texture source is invalid." );
    return false;
  }

  Point2i size( this->pages_[0].texture->size() );

  // Sanity checks
  NOM_ASSERT( size.x == this->page_size_.w );
  NOM_ASSERT( size.y == this->page_size_.h );

  return true;
}

bool BMFont::parse_ascii_file(std::istream& fp)
{
  nom::size_type i;   // position (cursor)
  std::string line;   // current row of buffer
  std::string buffer; // textual representation of the input
  std::string key;    // left-hand constituent of the buffer
  std::string value;  // right-hand constituent of the buffer

  if( fp.good() == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not parse BMFont file stream." );
    return false;
  }

  while( !fp.eof() )
  {
    std::stringstream stream;
    std::getline( fp, line );
    stream << line;

    stream >> buffer;
    if( buffer == "info" ) {

      // Info tag parsing
      while( !stream.eof() ) {

        std::stringstream converter;
        stream >> buffer;
        i = buffer.find('=');
        key = buffer.substr(0, i);
        value = buffer.substr(i + 1);

        // Assignment
        converter << value;

        if( key == "face" ) {
          // Not implemented; parsing of string sequences with space characters
          // needs to be implemented
        } else if( key == "size" ) {
          converter >> this->point_size_;
        }
      } // end while stream != EOF
    } // end if buffer == "info"
    else if( buffer == "common" ) {

      int pages = 0;

      // Common tag parsing
      while( !stream.eof() ) {

        std::stringstream converter;
        stream >> buffer;
        i = buffer.find('=');
        key = buffer.substr(0, i);
        value = buffer.substr(i + 1);

        // Assignment
        converter << value;

        if( key == "lineHeight" ) {
          converter >> this->metrics_.newline;
        } else if( key == "base" ) {
          converter >> this->metrics_.ascent;
        } else if( key == "scaleW" ) {
          converter >> this->page_size_.w;
        } else if( key == "scaleH" ) {
          converter >> this->page_size_.h;
        } else if( key == "pages" ) {
          converter >> pages;

          // Only one page texture of glyphs is supported
          NOM_ASSERT(pages == 1);
        }

      } // end while not EOF
    }
    else if( buffer == "page" ) {

      // Page tag parsing
      while( !stream.eof() ) {

        std::stringstream converter;
        stream >> buffer;
        i = buffer.find('=');
        key = buffer.substr(0, i);
        value = buffer.substr(i + 1);

        // Assignment
        converter << value;

        if( key == "id" ) {
          converter >> this->page_.id;

          // Only one page texture of glyphs is supported
          NOM_ASSERT(this->page_.id == 0);

        } else if( key == "file" ) {

          // FIXME: Parsing string sequences with spaces
          std::string string_sequence;
          this->parse_string(value, string_sequence);

          this->page_.filename = string_sequence;

          NOM_ASSERT(this->page_.filename != "");
        }
      } // end while stream not EOF

      // NOM_DUMP(this->page_.id);
      // NOM_DUMP(this->page_.filename);

    } // end if 'page' tag
    else if( buffer == "char" ) {

      int cid = 0;    // char id
      int pages = 0;  // char page

      // Char tag parsing
      while( !stream.eof() ) {

        std::stringstream converter;
        stream >> buffer;
        i = buffer.find('=');
        key = buffer.substr(0, i);
        value = buffer.substr(i + 1);

        // Assignment
        converter << value;

        if( key == "id" ) {
          converter >> cid;
        } else if( key == "x" ) {
          converter >> this->pages_[0].glyphs[cid].bounds.x;
        } else if( key == "y" ) {
          converter >> this->pages_[0].glyphs[cid].bounds.y;
        } else if( key == "width" ) {
          converter >> this->pages_[0].glyphs[cid].bounds.w;
        } else if( key == "height" ) {
          converter >> this->pages_[0].glyphs[cid].bounds.h;
        } else if( key == "xoffset" ) {
          converter >> this->pages_[0].glyphs[cid].offset.x;
        } else if( key == "yoffset" ) {
          converter >> this->pages_[0].glyphs[cid].offset.y;
        } else if( key == "xadvance" ) {
          converter >> this->pages_[0].glyphs[cid].advance;
        } else if( key == "page" ) {
          converter >> pages;

          // Only one page texture of glyphs is supported
          NOM_ASSERT(pages == 0);
        }

      } // end while line not EOF
    } // end if 'char' tag
    else if( buffer == "kerning" ) {

      BMFontKerningPair pair;

      // kerning tag parsing
      while( !stream.eof() ) {

        std::stringstream converter;
        stream >> buffer;
        i = buffer.find('=');
        key = buffer.substr(0, i);
        value = buffer.substr(i + 1);

        // Assignment
        converter << value;

        if( key == "first" ) {
          converter >> pair.first_char_id;
        }

        if( key == "second" ) {
          converter >> pair.second_char_id;
        }

        if( key == "amount" ) {
          converter >> pair.x_offset;
        }

        // Ensure that the struct's field has been filled with a non-default
        // value
        if( pair.x_offset != 0 ) {
          this->kernings_.push_back(pair);
        }

      } // end while stream not EOF
    } // end if 'kerning' tag
  } // end while fp not EOF

  return true;
}

void BMFont::parse_string(const std::string& key, std::string& value)
{
  // Beginning and end positions for the string value
  nom::size_type begin_pos = key.find_first_of("\"");
  nom::size_type end_pos = key.find_last_of("\"");

  value = key.substr(begin_pos+1, (end_pos - begin_pos)-1);

  // NOM_DUMP(begin_pos);
  // NOM_DUMP(end_pos);
}

} // namespace nom
