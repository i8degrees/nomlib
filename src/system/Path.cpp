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
#include "nomlib/system/Path.hpp"

namespace nom {

void Path::init( void )
{
  #if defined( NOM_PLATFORM_WINDOWS )
    path_separator = "\\";
  #elif defined( NOM_PLATFORM_POSIX )
    path_separator = "/";
  #else // Hope that we are running on a POSIX-compliant platform!
    path_separator = "/";
    #pragma message ( "nomlib cannot detect the platform it is being compiled on; assuming POSIX-compliant path delimiters..." )
  #endif
}

Path::Path( void )
{
  // NOM_LOG_TRACE( NOM ):

  this->init();
}

Path::~Path( void )
{
  // NOM_LOG_TRACE( NOM ):
}

Path::Path( const std::string& p ) :
  pathname( p )
{
  // NOM_LOG_TRACE( NOM ):

  this->init();
}

const Path::value_type& Path::native ( void ) const
{
  return this->path_separator;
}

const std::string& Path::path ( void ) const
{
  return this->pathname;
}

Path& Path::operator =( const Path& rhs )
{
  this->path_separator = rhs.native();
  this->pathname = rhs.path();

  return *this;
}

std::string Path::prepend( const std::string& path ) const
{
  if( this->path().substr( this->path().size() - 1, 1 ) == this->native() )
  {
    return this->path() + path;
  }
  else
  {
    return this->path() + this->native() + path;
  }
}

} // namespace nom
