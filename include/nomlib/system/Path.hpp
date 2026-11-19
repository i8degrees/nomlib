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
#ifndef NOMLIB_PATH_HEADERS
#define NOMLIB_PATH_HEADERS

#include <iostream>
#include <string>
#include <cstring>

#include "export.hpp"
#include "nomlib/config.hpp"
#include "nomlib/system/filesystem.hpp"

namespace nom {

/// \brief Platform-agnostic handling of directory paths.
///
/// This class is now a thin wrapper around nom::filesystem::path, preserving
/// the historical nomlib API while gaining proper path decomposition.
class NOM_EXPORT Path
{
  public:
    /// Platform-dependent data typedef used for defining the preferred file
    /// path separator.
    typedef std::string value_type;

    /// \brief Initialize the platform's native path delimiter.
    ///
    /// \remarks Kept for backward compatibility; the separator is provided by
    /// nom::filesystem::path::preferred_separator.
    void init( void );

    /// \brief Default constructor.
    Path ( void );

    /// \brief Destructor.
    ~Path ( void );

    /// \brief Construct a complete object.
    ///
    /// \param p The relative or absolute path to a file or directory.
    Path ( const std::string& p );

    /// Obtain native path (directory) separator for the current platform.
    const value_type& native ( void ) const;

    /// Obtain assigned pathname.
    const std::string& path ( void ) const;

    /// Assignment operator.
    Path& operator =( const Path& rhs );

    /// \brief Prefix the stored path to the given path, inserting a separator
    /// when needed.
    std::string prepend( const std::string& path ) const;

  private:
    filesystem::path p_;
};


} // namespace nom

#endif // include guard

/// \class nom::Path
/// \ingroup system
///
/// Inspired by Boost::Filesystem
///
/// \see http://www.boost.org/doc/libs/1_55_0/libs/filesystem/doc/reference.html#class-path
///
