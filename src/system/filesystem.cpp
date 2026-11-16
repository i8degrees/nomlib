/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or
   other materials provided with the distribution.

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
#include "nomlib/system/filesystem.hpp"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

  #if defined( NOM_PLATFORM_WINDOWS )
  #include <direct.h>
  #include <io.h>
  #include <sys/stat.h>
  #include <sys/types.h>
  #include <windows.h>
  #include <cwchar>
  #else
  #include <unistd.h>
  #include <sys/stat.h>
  #include <sys/types.h>
  #include <dirent.h>
#endif

namespace nom {
namespace filesystem {

namespace detail {

bool is_separator( char c )
{
#if defined( NOM_PLATFORM_WINDOWS )
  return c == '\\' || c == '/';
#else
  return c == '/';
#endif
}

// Returns a pair of (root-name, number of consumed characters).
std::pair<std::string, std::size_t> parse_root_name( const std::string& s )
{
#if defined( NOM_PLATFORM_WINDOWS )
  // Drive letter, e.g. "C:"
  if( s.size() >= 2 &&
      std::isalpha( static_cast<unsigned char>( s[0] ) ) &&
      s[1] == ':' )
  {
    return std::make_pair( s.substr( 0, 2 ), std::size_t(2) );
  }

  // UNC root-name, e.g. "\\server\share"
  if( s.size() >= 2 && s[0] == '\\' && s[1] == '\\' )
  {
    std::size_t end = 2;
    int components_seen = 0;
    std::size_t component_start = 2;

    for(; end < s.size() && components_seen < 2; ++end )
    {
      if( is_separator( s[end] ) )
      {
        if( end > component_start )
        {
          ++components_seen;
          if( components_seen == 2 )
            break;
        }
        component_start = end + 1;
      }
    }

    if( components_seen == 2 )
      return std::make_pair( s.substr( 0, end ), end );
  }
#endif
  (void)s;
  return std::make_pair( std::string(), std::size_t(0) );
}

std::string join_components( const std::vector<std::string>& components,
                              char separator )
{
  std::string result;
  for( std::size_t i = 0; i < components.size(); ++i )
  {
    if( i > 0 ) result.push_back( separator );
    result += components[i];
  }
  return result;
}

// Decode a single UTF-16 code unit sequence starting at i, returning the code
// point and updating i past consumed code units.  Invalid sequences return
// false and consume one code unit.
bool decode_utf16( const std::u16string& u16, std::size_t& i, std::uint32_t& cp )
{
  if( i >= u16.size() ) return false;

  char16_t high = u16[i++];

  // High surrogate (0xD800-0xDBFF)
  if( high >= 0xD800 && high <= 0xDBFF )
  {
    if( i >= u16.size() ) return false;

    char16_t low = u16[i++];
    if( low < 0xDC00 || low > 0xDFFF ) return false;

    cp = 0x10000u +
         ( ( static_cast<std::uint32_t>( high ) - 0xD800u ) << 10 ) +
         ( static_cast<std::uint32_t>( low ) - 0xDC00u );
    return true;
  }

  // Standalone low surrogate is invalid.
  if( high >= 0xDC00 && high <= 0xDFFF ) return false;

  cp = static_cast<std::uint32_t>( high );
  return true;
}

// Append a single Unicode code point to a UTF-8 string.
void encode_utf8( std::string& out, std::uint32_t cp )
{
  if( cp < 0x80 )
  {
    out.push_back( static_cast<char>( cp ) );
  }
  else if( cp < 0x800 )
  {
    out.push_back( static_cast<char>( 0xC0 | ( cp >> 6 ) ) );
    out.push_back( static_cast<char>( 0x80 | ( cp & 0x3F ) ) );
  }
  else if( cp < 0x10000 )
  {
    out.push_back( static_cast<char>( 0xE0 | ( cp >> 12 ) ) );
    out.push_back( static_cast<char>( 0x80 | ( ( cp >> 6 ) & 0x3F ) ) );
    out.push_back( static_cast<char>( 0x80 | ( cp & 0x3F ) ) );
  }
  else
  {
    out.push_back( static_cast<char>( 0xF0 | ( cp >> 18 ) ) );
    out.push_back( static_cast<char>( 0x80 | ( ( cp >> 12 ) & 0x3F ) ) );
    out.push_back( static_cast<char>( 0x80 | ( ( cp >> 6 ) & 0x3F ) ) );
    out.push_back( static_cast<char>( 0x80 | ( cp & 0x3F ) ) );
  }
}

// Decode a single UTF-8 code point starting at i, returning the code point
// and updating i past consumed bytes.  Invalid sequences return false and
// consume one byte.
bool decode_utf8( const std::string& u8, std::size_t& i, std::uint32_t& cp )
{
  if( i >= u8.size() ) return false;

  unsigned char c = static_cast<unsigned char>( u8[i] );
  std::uint32_t code_point = 0;
  std::size_t extra = 0;

  if( ( c & 0x80 ) == 0 )
  {
    code_point = c;
    extra = 0;
  }
  else if( ( c & 0xE0 ) == 0xC0 )
  {
    code_point = c & 0x1F;
    extra = 1;
  }
  else if( ( c & 0xF0 ) == 0xE0 )
  {
    code_point = c & 0x0F;
    extra = 2;
  }
  else if( ( c & 0xF8 ) == 0xF0 )
  {
    code_point = c & 0x07;
    extra = 3;
  }
  else
  {
    // Invalid lead byte.
    ++i;
    return false;
  }

  if( i + extra >= u8.size() )
  {
    ++i;
    return false;
  }

  for( std::size_t j = 1; j <= extra; ++j )
  {
    unsigned char n = static_cast<unsigned char>( u8[i + j] );
    if( ( n & 0xC0 ) != 0x80 )
    {
      ++i;
      return false;
    }
    code_point = ( code_point << 6 ) | ( n & 0x3F );
  }

  // Reject overlong encodings, surrogates and out-of-range values.
  if( ( extra == 0 && c > 0x7F ) ||
      ( extra == 1 && code_point < 0x80 ) ||
      ( extra == 2 && code_point < 0x800 ) ||
      ( extra == 3 && code_point < 0x10000 ) ||
      code_point > 0x10FFFF ||
      ( code_point >= 0xD800 && code_point <= 0xDFFF ) )
  {
    ++i;
    return false;
  }

  cp = code_point;
  i += extra + 1;
  return true;
}

std::string utf16_to_utf8( const std::u16string& u16 )
{
  std::string u8;
  u8.reserve( u16.size() );

  std::size_t i = 0;
  std::uint32_t cp = 0;
  while( decode_utf16( u16, i, cp ) )
  {
    encode_utf8( u8, cp );
  }

  return u8;
}

std::u16string utf8_to_utf16( const std::string& u8 )
{
  std::u16string u16;
  u16.reserve( u8.size() );

  std::size_t i = 0;
  std::uint32_t cp = 0;
  while( decode_utf8( u8, i, cp ) )
  {
    if( cp > 0xFFFF )
    {
      cp -= 0x10000u;
      u16.push_back( static_cast<char16_t>( 0xD800u | ( cp >> 10 ) ) );
      u16.push_back( static_cast<char16_t>( 0xDC00u | ( cp & 0x3FF ) ) );
    }
    else
    {
      u16.push_back( static_cast<char16_t>( cp ) );
    }
  }

  return u16;
}

std::u32string utf8_to_utf32( const std::string& u8 )
{
  std::u32string u32;
  u32.reserve( u8.size() );

  std::size_t i = 0;
  std::uint32_t cp = 0;
  while( decode_utf8( u8, i, cp ) )
  {
    u32.push_back( static_cast<char32_t>( cp ) );
  }

  return u32;
}

std::string utf32_to_utf8( const std::u32string& u32 )
{
  std::string u8;
  u8.reserve( u32.size() * 4 );

  for( char32_t c : u32 )
  {
    encode_utf8( u8, static_cast<std::uint32_t>( c ) );
  }

  return u8;
}

#if defined( NOM_PLATFORM_WINDOWS )
std::wstring utf8_to_wstring( const std::string& u8 )
{
  std::u16string u16 = utf8_to_utf16( u8 );
  return std::wstring( u16.begin(), u16.end() );
}

std::string wstring_to_utf8( const std::wstring& w )
{
  std::u16string u16( w.begin(), w.end() );
  return utf16_to_utf8( u16 );
}
#else
std::wstring utf8_to_wstring( const std::string& u8 )
{
  std::u32string u32 = utf8_to_utf32( u8 );
  return std::wstring( u32.begin(), u32.end() );
}

std::string wstring_to_utf8( const std::wstring& w )
{
  std::u32string u32( w.begin(), w.end() );
  return utf32_to_utf8( u32 );
}
#endif

} // namespace detail

#if defined( NOM_PLATFORM_WINDOWS )
const path::value_type path::preferred_separator = '\\';

namespace {
std::wstring to_wide_path( const std::string& u8 )
{
  return nom::filesystem::detail::utf8_to_wstring( u8 );
}
} // anonymous namespace
#else
const path::value_type path::preferred_separator = '/';
#endif

path::path() :
  dirty_( true ),
  has_root_directory_( false ),
  absolute_( false ),
  has_trailing_separator_( false )
{
  // Nothing to do.
}

path::path( const string_type& source ) : path()
{
  this->parse( source );
}

path::path( const char* source ) : path()
{
  if( source ) this->parse( source );
}

path::path( const u16string_type& source ) : path()
{
  this->parse( detail::utf16_to_utf8( source ) );
}

path::path( const std::wstring& source ) : path()
{
  this->parse( detail::wstring_to_utf8( source ) );
}

path::path( const path& other ) = default;
path::path( path&& other ) noexcept = default;
path::~path() = default;

path& path::operator=( const path& other ) = default;
path& path::operator=( path&& other ) noexcept = default;

path& path::operator=( const string_type& source )
{
  this->parse( source );
  return *this;
}

path& path::operator=( const char* source )
{
  if( source ) this->parse( source );
  else this->clear();
  return *this;
}

path& path::operator=( const u16string_type& source )
{
  this->parse( detail::utf16_to_utf8( source ) );
  return *this;
}

path& path::operator=( const std::wstring& source )
{
  this->parse( detail::wstring_to_utf8( source ) );
  return *this;
}

path& path::assign( const string_type& source )
{
  this->parse( source );
  return *this;
}

path& path::assign( const u16string_type& source )
{
  this->parse( detail::utf16_to_utf8( source ) );
  return *this;
}

path& path::assign( const std::wstring& source )
{
  this->parse( detail::wstring_to_utf8( source ) );
  return *this;
}

path& path::operator/=( const path& other )
{
  if( other.is_absolute() )
  {
    *this = other;
    return *this;
  }

  if( other.has_root_directory() )
  {
    if( this->has_root_name() )
    {
      string_type keep_root = this->root_name_;
      *this = other;
      this->root_name_ = keep_root;
      this->absolute_ = !this->root_name_.empty() && this->has_root_directory_;
      this->invalidate_cache();
    }
    else
    {
      *this = other;
    }
    return *this;
  }

  // Avoid producing "foo//bar" when the left-hand side has a trailing
  // separator (which parse() stores as an empty final component).
  if( this->has_trailing_separator_ &&
      !this->components_.empty() &&
      this->components_.back().empty() )
  {
    this->components_.pop_back();
    this->has_trailing_separator_ = false;
  }

  for( const auto& c : other.components_ )
  {
    this->components_.push_back( c );
  }

  if( other.has_trailing_separator_ )
  {
    this->has_trailing_separator_ = true;
  }

  this->normalize();
  this->invalidate_cache();
  return *this;
}

path& path::operator/=( const string_type& source )
{
  return *this /= path( source );
}

path& path::operator/=( const char* source )
{
  return *this /= path( source ? source : "" );
}

path& path::operator+=( const path& other )
{
  string_type combined = this->native() + other.native();
  this->parse( combined );
  return *this;
}

path& path::operator+=( const string_type& source )
{
  return *this += path( source );
}

path& path::operator+=( const char* source )
{
  return *this += path( source ? source : "" );
}

void path::clear() noexcept
{
  this->root_name_.clear();
  this->has_root_directory_ = false;
  this->absolute_ = false;
  this->components_.clear();
  this->has_trailing_separator_ = false;
  this->native_pathname_.clear();
  this->dirty_ = true;
}

void path::invalidate_cache() const
{
  this->dirty_ = true;
}

void path::rebuild_cache() const
{
  this->native_pathname_.clear();

  if( this->has_root_name() )
  {
    this->native_pathname_ += this->root_name_;
  }

  if( this->has_root_directory_ )
  {
    this->native_pathname_.push_back( this->preferred_separator );
  }

  for( std::size_t i = 0; i < this->components_.size(); ++i )
  {
    if( i > 0 )
    {
      this->native_pathname_.push_back( this->preferred_separator );
    }
    this->native_pathname_ += this->components_[i];
  }

  this->dirty_ = false;
}

const path::string_type& path::native() const
{
  if( this->dirty_ ) this->rebuild_cache();
  return this->native_pathname_;
}

const path::value_type* path::c_str() const
{
  return this->native().c_str();
}

path::string_type path::string() const
{
  return this->native();
}

path::string_type path::generic_string() const
{
  return this->native();
}

path::u8string_type path::u8string() const
{
  return this->native();
}

path::u16string_type path::u16string() const
{
  return detail::utf8_to_utf16( this->native() );
}

std::wstring path::wstring() const
{
  return detail::utf8_to_wstring( this->native() );
}

path::u8string_type path::generic_u8string() const
{
  return this->u8string();
}

path::u16string_type path::generic_u16string() const
{
  return this->u16string();
}

std::wstring path::generic_wstring() const
{
  return this->wstring();
}

void path::normalize()
{
  std::vector<string_type> out;
  out.reserve( this->components_.size() );

  for( const auto& component : this->components_ )
  {
    if( component == "." )
    {
      continue;
    }

    if( component == ".." )
    {
      if( !out.empty() && out.back() != ".." )
      {
        out.pop_back();
      }
      else if( !this->is_absolute() )
      {
        out.push_back( component );
      }
      // An absolute path drops ".." components that would escape the root.
      continue;
    }

    out.push_back( component );
  }

  this->components_ = std::move( out );
}

void path::parse( const string_type& source )
{
  this->clear();

  if( source.empty() )
  {
    return;
  }

#if defined( NOM_PLATFORM_WINDOWS )
  auto root = detail::parse_root_name( source );
  this->root_name_ = root.first;
  std::size_t pos = root.second;
#else
  std::size_t pos = 0;
#endif

  if( pos < source.size() && detail::is_separator( source[pos] ) )
  {
    this->has_root_directory_ = true;
    ++pos;
  }

#if defined( NOM_PLATFORM_WINDOWS )
  this->absolute_ = !this->root_name_.empty() && this->has_root_directory_;
#else
  this->absolute_ = this->has_root_directory_;
#endif

  if( !source.empty() && detail::is_separator( source[source.size()-1] ) )
  {
    this->has_trailing_separator_ = true;
  }

  string_type token;
  for( ; pos < source.size(); ++pos )
  {
    char c = source[pos];
    if( detail::is_separator( c ) )
    {
      if( !token.empty() )
      {
        this->components_.push_back( token );
        token.clear();
      }
    }
    else
    {
      token.push_back( c );
    }
  }

  if( !token.empty() )
  {
    this->components_.push_back( token );
  }

  this->normalize();

  // A trailing directory separator adds an empty filename element so that
  // parent_path() / filename() behave like std::filesystem::path.
  if( this->has_trailing_separator_ && !this->components_.empty() )
  {
    this->components_.push_back( string_type() );
  }
}

path path::root_name() const
{
  if( !this->has_root_name() ) return path();
  return path( this->root_name_ );
}

path path::root_directory() const
{
  if( !this->has_root_directory_ ) return path();
  return path( string_type( 1, this->preferred_separator ) );
}

path path::root_path() const
{
  if( this->root_name_.empty() && !this->has_root_directory_ )
  {
    return path();
  }

  string_type s = this->root_name_;
  if( this->has_root_directory_ )
  {
    s.push_back( this->preferred_separator );
  }
  return path( s );
}

path path::relative_path() const
{
  if( this->components_.empty() ) return path();
  return path( detail::join_components( this->components_,
                                        this->preferred_separator ) );
}

path path::parent_path() const
{
  if( this->components_.empty() )
  {
    return path();
  }

  if( this->components_.size() == 1 )
  {
    if( this->has_root_directory_ || !this->root_name_.empty() )
    {
      return this->root_path();
    }
    return path();
  }

  string_type s = this->root_name_;
  if( this->has_root_directory_ )
  {
    s.push_back( this->preferred_separator );
  }

  for( std::size_t i = 0; i + 1 < this->components_.size(); ++i )
  {
    if( !s.empty() )
    {
      s.push_back( this->preferred_separator );
    }
    s += this->components_[i];
  }

  return path( s );
}

path path::filename() const
{
  if( this->components_.empty() || this->components_.back().empty() )
  {
    return path();
  }
  return path( this->components_.back() );
}

path path::stem() const
{
  string_type f = this->filename().native();
  if( f.empty() ) return path();

  // Special names such as "." and ".." are not extension-bearing.
  if( f == "." || f == ".." )
  {
    return path( f );
  }

  std::size_t pos = f.find_last_of( '.' );
  if( pos == string_type::npos || pos == 0 )
  {
    return path( f );
  }
  return path( f.substr( 0, pos ) );
}

path path::extension() const
{
  string_type f = this->filename().native();
  if( f.empty() ) return path();

  // Special names such as "." and ".." are not extension-bearing.
  if( f == "." || f == ".." )
  {
    return path();
  }

  std::size_t pos = f.find_last_of( '.' );
  if( pos == string_type::npos || pos == 0 )
  {
    return path();
  }
  return path( f.substr( pos ) );
}

bool path::has_root_name() const
{
  return !this->root_name_.empty();
}

bool path::has_root_directory() const
{
  return this->has_root_directory_;
}

bool path::has_root_path() const
{
  return this->has_root_name() || this->has_root_directory_;
}

bool path::has_relative_path() const
{
  return !this->components_.empty();
}

bool path::has_parent_path() const
{
  if( this->components_.size() > 1 ) return true;
  if( this->components_.size() == 1 &&
      ( this->has_root_directory_ || !this->root_name_.empty() ) )
  {
    return true;
  }
  return false;
}

bool path::has_filename() const
{
  return !this->components_.empty() && !this->components_.back().empty();
}

bool path::has_stem() const
{
  return this->has_filename();
}

bool path::has_extension() const
{
  return this->has_filename() && !this->extension().native().empty();
}

bool path::is_absolute() const
{
  return this->absolute_;
}

bool path::is_relative() const
{
  return !this->absolute_;
}

path& path::make_preferred()
{
  const char sep = path::preferred_separator;

  string_type native = this->native();

  for( char& c : native )
  {
    if( detail::is_separator( c ) && c != sep )
    {
      c = sep;
    }
  }

  this->parse( native );
  return *this;
}

std::vector<path> path::path_elements() const
{
  std::vector<path> result;

  if( !this->root_name_.empty() )
  {
    result.push_back( path( this->root_name_ ) );
  }

  if( this->has_root_directory_ )
  {
    result.push_back( path( std::string( 1, path::preferred_separator ) ) );
  }

  for( const auto& component : this->components_ )
  {
    result.push_back( path( component ) );
  }

  return result;
}

path::iterator::iterator() :
  elements_(),
  pos_( 0 )
{
  // End-of-range default iterator.
}

path::iterator::iterator( const path& p, bool end_it ) :
  elements_( p.path_elements() ),
  pos_( end_it ? this->elements_.size() : 0 )
{
  // Nothing to do
}

path::iterator::reference path::iterator::operator*() const
{
  return this->elements_[this->pos_];
}

path::iterator::pointer path::iterator::operator->() const
{
  return &this->elements_[this->pos_];
}

path::iterator& path::iterator::operator++()
{
  ++this->pos_;
  return *this;
}

path::iterator path::iterator::operator++( int )
{
  iterator tmp( *this );
  ++this->pos_;
  return tmp;
}

bool path::iterator::operator==( const iterator& other ) const
{
  const bool this_end = this->pos_ >= this->elements_.size();
  const bool other_end = other.pos_ >= other.elements_.size();

  if( this_end && other_end )
  {
    return true;
  }
  if( this_end != other_end )
  {
    return false;
  }

  // Two iterators into equivalent sequences at the same position are equal.
  if( this->elements_.size() != other.elements_.size() )
  {
    return false;
  }

  for( std::size_t i = 0; i < this->elements_.size(); ++i )
  {
    if( this->elements_[i].native() != other.elements_[i].native() )
    {
      return false;
    }
  }

  return this->pos_ == other.pos_;
}

bool path::iterator::operator!=( const iterator& other ) const
{
  return !this->operator==( other );
}

path::iterator path::begin() const
{
  return path::iterator( *this, false );
}

path::iterator path::end() const
{
  return path::iterator( *this, true );
}

bool path::empty() const
{
  return !this->has_root_name() && !this->has_root_directory_ &&
          this->components_.empty();
}

path operator/( const path& lhs, const path& rhs )
{
  path result( lhs );
  result /= rhs;
  return result;
}

path operator/( const path& lhs, const std::string& rhs )
{
  return lhs / path( rhs );
}

path operator/( const path& lhs, const char* rhs )
{
  return lhs / path( rhs ? rhs : "" );
}

path operator/( const std::string& lhs, const path& rhs )
{
  return path( lhs ) / rhs;
}

path operator+( const path& lhs, const path& rhs )
{
  path result( lhs );
  result += rhs;
  return result;
}

path operator+( const path& lhs, const std::string& rhs )
{
  return lhs + path( rhs );
}

path operator+( const path& lhs, const char* rhs )
{
  return lhs + path( rhs ? rhs : "" );
}

/// \name Directory iteration
/// \{

directory_entry::directory_entry() :
  path_()
{
  // Nothing to do
}

directory_entry::directory_entry( class path p ) :
  path_( std::move( p ) )
{
  // Nothing to do
}

const path& directory_entry::path() const
{
  return this->path_;
}

struct directory_iterator::state
{
#if defined( NOM_PLATFORM_WINDOWS )
  HANDLE handle_ = nullptr;
#else
  DIR* dir_ = nullptr;
#endif
  path dir_path_;
  directory_entry entry_;

  ~state()
  {
    this->close();
  }

  void close() noexcept
  {
#if defined( NOM_PLATFORM_WINDOWS )
    if( this->handle_ != nullptr )
    {
      ::FindClose( this->handle_ );
      this->handle_ = nullptr;
    }
#else
    if( this->dir_ != nullptr )
    {
      ::closedir( this->dir_ );
      this->dir_ = nullptr;
    }
#endif
  }

#if defined( NOM_PLATFORM_WINDOWS )
  void open( const path& p )
  {
    this->dir_path_ = p;
    std::wstring pattern = to_wide_path( p.native() ) + L"\\*";
    WIN32_FIND_DATAW data;
    this->handle_ = ::FindFirstFileW( pattern.c_str(), &data );
    if( this->handle_ == INVALID_HANDLE_VALUE )
    {
      this->handle_ = nullptr;
      this->entry_ = directory_entry();
      return;
    }

    if( std::wcscmp( data.cFileName, L"." ) == 0 ||
        std::wcscmp( data.cFileName, L".." ) == 0 )
    {
      this->advance();
    }
    else
    {
      this->entry_ = directory_entry( this->dir_path_ / narrow_name(data.cFileName) );
    }
  }

  static std::string narrow_name( const wchar_t* wname )
  {
    int len = ::WideCharToMultiByte( CP_UTF8, 0, wname, -1,
                                     nullptr, 0, nullptr, nullptr );
    std::string name;
    if( len > 0 )
    {
      name.resize( static_cast<std::size_t>( len - 1 ) );
      ::WideCharToMultiByte( CP_UTF8, 0, wname, -1, &name[0], len,
                             nullptr, nullptr );
    }
    return name;
  }

  void advance()
  {
    if( this->handle_ == nullptr )
    {
      this->entry_ = directory_entry();
      return;
    }

    WIN32_FIND_DATAW data;
    while( ::FindNextFileW( this->handle_, &data ) != 0 )
    {
      if( std::wcscmp( data.cFileName, L"." ) == 0 ||
          std::wcscmp( data.cFileName, L".." ) == 0 )
      {
        continue;
      }

      this->entry_ = directory_entry( this->dir_path_ / narrow_name(data.cFileName) );
      return;
    }

    this->close();
    this->entry_ = directory_entry();
  }
#else // POSIX
  void open( const path& p )
  {
    this->dir_path_ = p;
    this->dir_ = ::opendir( p.c_str() );
    this->advance();
  }

  void advance()
  {
    if( this->dir_ == nullptr )
    {
      this->entry_ = directory_entry();
      return;
    }

    struct dirent* item = nullptr;
    while( ( item = ::readdir( this->dir_ ) ) != nullptr )
    {
      if( std::strcmp( item->d_name, "." ) == 0 ||
          std::strcmp( item->d_name, ".." ) == 0 )
      {
        continue;
      }

      this->entry_ = directory_entry( this->dir_path_ / item->d_name );
      return;
    }

    this->close();
    this->entry_ = directory_entry();
  }
#endif
};

directory_iterator::directory_iterator()
{
  // Nothing to do: state_ is empty, indicating the end iterator.
}

directory_iterator::directory_iterator( const class path& p ) :
  state_( std::make_shared<state>() )
{
  this->state_->open( p );
}

directory_iterator::~directory_iterator()
{
  // Nothing to do: shared_ptr handles cleanup of the underlying directory.
}

const directory_entry& directory_iterator::operator*() const
{
  return this->state_->entry_;
}

const directory_entry* directory_iterator::operator->() const
{
  return &this->state_->entry_;
}

directory_iterator& directory_iterator::operator++()
{
  this->state_->advance();
  return *this;
}

bool directory_iterator::operator==( const directory_iterator& other ) const
{
  const bool this_end = ( this->state_ == nullptr ) ||
                        this->state_->entry_.path().empty();
  const bool other_end = ( other.state_ == nullptr ) ||
                         other.state_->entry_.path().empty();

  if( this_end && other_end )
  {
    return true;
  }
  if( this_end != other_end )
  {
    return false;
  }

  // Two non-end iterators are equal only if they share the same directory
  // state (i.e. they were copied from the same source iterator).
  return this->state_ == other.state_;
}

bool directory_iterator::operator!=( const directory_iterator& other ) const
{
  return !this->operator==( other );
}

directory_iterator begin( directory_iterator it )
{
  return it;
}

directory_iterator end( const directory_iterator& )
{
  return directory_iterator();
}

/// \}

/// \name Operational functions
/// \{

bool exists( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_WINDOWS )
  std::wstring wp = to_wide_path( p.native() );
  struct _stat st;
  if( ::_wstat( wp.c_str(), &st ) != 0 ) return false;
#else
  struct stat st;
  if( ::stat( p.c_str(), &st ) != 0 ) return false;
#endif
  return true;
}

bool is_directory( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_WINDOWS )
  std::wstring wp = to_wide_path( p.native() );
  struct _stat st;
  if( ::_wstat( wp.c_str(), &st ) != 0 ) return false;
  return ( st.st_mode & _S_IFDIR ) != 0;
#else
  struct stat st;
  if( ::stat( p.c_str(), &st ) != 0 ) return false;
  return S_ISDIR( st.st_mode );
#endif
}

bool is_regular_file( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_WINDOWS )
  std::wstring wp = to_wide_path( p.native() );
  struct _stat st;
  if( ::_wstat( wp.c_str(), &st ) != 0 ) return false;
  return ( st.st_mode & _S_IFREG ) != 0;
#else
  struct stat st;
  if( ::stat( p.c_str(), &st ) != 0 ) return false;
  return S_ISREG( st.st_mode );
#endif
}

bool is_symlink( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_POSIX )
  struct stat st;
  if( ::lstat( p.c_str(), &st ) != 0 ) return false;
  return S_ISLNK( st.st_mode );
#else
  return false;
#endif
}

bool exists_symlink( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_POSIX )
  struct stat st;
  return ::lstat( p.c_str(), &st ) == 0;
#else
  return exists( p );
#endif
}

bool is_symlink_directory( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_POSIX )
  struct stat st_link;
  if( ::lstat( p.c_str(), &st_link ) != 0 ) return false;
  if( !S_ISLNK( st_link.st_mode ) ) return false;

  struct stat st_target;
  if( ::stat( p.c_str(), &st_target ) != 0 ) return false;
  return S_ISDIR( st_target.st_mode );
#else
  return false;
#endif
}

bool is_symlink_regular_file( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_POSIX )
  struct stat st_link;
  if( ::lstat( p.c_str(), &st_link ) != 0 ) return false;
  if( !S_ISLNK( st_link.st_mode ) ) return false;

  struct stat st_target;
  if( ::stat( p.c_str(), &st_target ) != 0 ) return false;
  return S_ISREG( st_target.st_mode );
#else
  return false;
#endif
}

std::uintmax_t file_size_symlink( const path& p )
{
#if defined( NOM_PLATFORM_POSIX )
  struct stat st;
  if( ::lstat( p.c_str(), &st ) != 0 ) return 0;
  if( !S_ISLNK( st.st_mode ) ) return 0;
  return static_cast<std::uintmax_t>( st.st_size );
#else
  return 0;
#endif
}

bool create_directory( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_WINDOWS )
  std::wstring wp = to_wide_path( p.native() );
  if( CreateDirectoryW( wp.c_str(), nullptr ) != 0 ) return true;
#else
  if( ::mkdir( p.c_str(), 0755 ) == 0 ) return true;
#endif
  return false;
}

bool create_directories( const path& p )
{
  if( p.empty() ) return false;
  if( exists( p ) ) return false;

  path parent = p.parent_path();
  if( !parent.empty() && !exists( parent ) )
  {
    create_directories( parent );
  }

  return create_directory( p );
}

bool remove( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_WINDOWS )
  std::wstring wp = to_wide_path( p.native() );
  DWORD attrs = ::GetFileAttributesW( wp.c_str() );
  if( attrs == INVALID_FILE_ATTRIBUTES )
  {
    return false;
  }

  if( attrs & FILE_ATTRIBUTE_DIRECTORY )
  {
    return ::RemoveDirectoryW( wp.c_str() ) != 0;
  }
  return ::DeleteFileW( wp.c_str() ) != 0;
#else
  struct stat st;
  if( ::lstat( p.c_str(), &st ) != 0 )
  {
    return false;
  }

  if( S_ISDIR( st.st_mode ) )
  {
    return ::rmdir( p.c_str() ) == 0;
  }
  return ::remove( p.c_str() ) == 0;
#endif
}

std::uintmax_t file_size( const path& p )
{
#if defined( NOM_PLATFORM_WINDOWS )
  std::wstring wp = to_wide_path( p.native() );
  struct __stat64 st;
  if( ::_wstat64( wp.c_str(), &st ) != 0 ) return 0;
  return static_cast<std::uintmax_t>( st.st_size );
#else
  struct stat st;
  if( ::stat( p.c_str(), &st ) != 0 ) return 0;
  return static_cast<std::uintmax_t>( st.st_size );
#endif
}

path current_path()
{
#if defined( NOM_PLATFORM_WINDOWS )
  wchar_t buffer[4096];
  if( ::_wgetcwd( buffer, sizeof(buffer) / sizeof(buffer[0]) ) == nullptr )
  {
    throw filesystem_error( "could not obtain current working directory",
                              path(), std::error_code(errno, std::system_category()) );
  }
  return path( std::wstring( buffer ) );
#else
  char buffer[4096];
  if( ::getcwd( buffer, sizeof(buffer) ) == nullptr )
  {
    throw filesystem_error( "could not obtain current working directory",
                              path(), std::error_code(errno, std::system_category()) );
  }
  return path( buffer );
#endif
}

void current_path( const path& p )
{
#if defined( NOM_PLATFORM_WINDOWS )
  std::wstring wp = to_wide_path( p.native() );
  if( ::_wchdir( wp.c_str() ) != 0 )
  {
    throw filesystem_error( "could not change current working directory",
                              p, std::error_code(errno, std::system_category()) );
  }
#else
  if( ::chdir( p.c_str() ) != 0 )
  {
    throw filesystem_error( "could not change current working directory",
                              p, std::error_code(errno, std::system_category()) );
  }
#endif
}

filesystem_error::filesystem_error( const std::string& what_arg, const path& p1,
                                      std::error_code ec )
  : std::system_error( ec, what_arg ),
    path1_(p1)
{
  // Empty
}

filesystem_error::filesystem_error( const std::string& what_arg, const path& p1,
                                      const path& p2, std::error_code ec )
  : std::system_error( ec, what_arg ),
    path1_(p1),
    path2_(p2)
{
  // Empty
}

const path& filesystem_error::path1() const noexcept
{
  return this->path1_;
}

const path& filesystem_error::path2() const noexcept
{
  return this->path2_;
}

const char* filesystem_error::what() const noexcept
{
  if( this->what_.empty() == true )
  {
    this->what_ = std::system_error::what();

    if( this->path1_.empty() == false )
    {
      this->what_ += " [" + this->path1_.native() + "]";
    }

    if( this->path2_.empty() == false )
    {
      this->what_ += " [" + this->path2_.native() + "]";
    }
  }

  return this->what_.c_str();
}

/// \}

} // namespace filesystem
} // namespace nom
