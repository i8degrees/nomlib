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
#ifndef NOMLIB_PATH_HEADERS
#define NOMLIB_PATH_HEADERS

#include <iostream>
#include <string>
#include <cstring>

#include "nomlib/config.hpp"

namespace nom {

/// Platform-dependent data typedef used for defining the preferred file
/// path separator.
typedef std::string value_type;

/// \brief Platform-independent interface for cross-platform handling of
/// pathnames
class Path
{
  public:
    /// Default constructor -- pathname is initialized as a null-terminated
    /// string.
    Path ( void );

    /// Initialize instance with an existing pathname
    Path ( const std::string& p );

    /// Default destructor
    ~Path ( void );

    /// Obtain native path (directory) separator; the value returned is
    /// dependent upon the platform and is determined at compile-time, not
    /// run-time.
    const value_type& native ( void ) const;

    /// Obtain assigned pathname
    const value_type& path ( void ) const;

    /// Assignment operator
    const Path& operator= ( const Path& p );

  private:
    value_type path_separator;
    std::string pathname;
};


} // namespace nom

#endif // include guard

/// \class nom::Path
///
/// Inspired by Boost::Filesystem
