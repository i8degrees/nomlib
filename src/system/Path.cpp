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
#include "nomlib/system/Path.hpp"

namespace nom {

// GCC v4.6.3 -- as per Ubuntu x64 v12.04 "Precise" -- has not (yet?)
// implemented non-static data member initializers, so... we have relocated
// these compile-time macros here for the time being.
//
//                        [*] -_- [*]
//
#if defined (NOM_PLATFORM_WINDOWS)
  value_type Path::path_separator = "\\";
#else // Assume POSIX-compliant platform -- home sweet home!
  value_type Path::path_separator = "/";
#endif

Path::Path ( void ) : pathname ( "\0" ) {}

Path::Path ( const std::string& p ) : pathname ( p ) {}

Path::~Path ( void ) {}

const value_type& Path::native ( void ) const
{
  return this->path_separator;
}

const value_type& Path::path ( void ) const
{
  return this->pathname;
}

const Path& Path::operator= ( const Path& p )
{
  this->pathname = p.pathname;

  return *this;
}


} // namespace nom
