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
#include <utility>
#include <vector>

#if defined( NOM_PLATFORM_WINDOWS )
  #include <direct.h>
  #include <io.h>
  #include <sys/stat.h>
  #include <sys/types.h>
  #include <windows.h>
#else
  #include <unistd.h>
  #include <sys/stat.h>
  #include <sys/types.h>
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

} // namespace detail

#if defined( NOM_PLATFORM_WINDOWS )
const path::value_type path::preferred_separator = '\\';
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

path& path::assign( const string_type& source )
{
  this->parse( source );
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
      this->invalidate_cache();
    }
    else
    {
      *this = other;
    }
    return *this;
  }

  for( const auto& c : other.components_ )
  {
    this->components_.push_back( c );
  }

  if( other.has_trailing_separator_ )
  {
    this->has_trailing_separator_ = true;
  }

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

/// \name Operational functions
/// \{

bool exists( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_WINDOWS )
  struct _stat st;
  if( ::_stat( p.c_str(), &st ) != 0 ) return false;
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
  struct _stat st;
  if( ::_stat( p.c_str(), &st ) != 0 ) return false;
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
  struct _stat st;
  if( ::_stat( p.c_str(), &st ) != 0 ) return false;
  return ( st.st_mode & _S_IFREG ) != 0;
#else
  struct stat st;
  if( ::stat( p.c_str(), &st ) != 0 ) return false;
  return S_ISREG( st.st_mode );
#endif
}

bool create_directory( const path& p )
{
  if( p.empty() ) return false;

#if defined( NOM_PLATFORM_WINDOWS )
  if( CreateDirectoryA( p.c_str(), nullptr ) != 0 ) return true;
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
  if( is_directory( p ) )
  {
    return RemoveDirectoryA( p.c_str() ) != 0;
  }
  return DeleteFileA( p.c_str() ) != 0;
#else
  if( is_directory( p ) )
  {
    return ::rmdir( p.c_str() ) == 0;
  }
  return std::remove( p.c_str() ) == 0;
#endif
}

std::uintmax_t file_size( const path& p )
{
#if defined( NOM_PLATFORM_WINDOWS )
  struct _stat st;
  if( ::_stat( p.c_str(), &st ) != 0 ) return 0;
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
  char buffer[4096];
  if( ::_getcwd( buffer, sizeof(buffer) ) == nullptr ) return path();
  return path( buffer );
#else
  char buffer[4096];
  if( ::getcwd( buffer, sizeof(buffer) ) == nullptr ) return path();
  return path( buffer );
#endif
}

void current_path( const path& p )
{
#if defined( NOM_PLATFORM_WINDOWS )
  ::_chdir( p.c_str() );
#else
  ::chdir( p.c_str() );
#endif
}

/// \}

} // namespace filesystem
} // namespace nom
