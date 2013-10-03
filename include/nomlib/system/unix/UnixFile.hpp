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
#ifndef NOMLIB_UNIX_FILE_HEADERS
#define NOMLIB_UNIX_FILE_HEADERS

#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>

#include "nomlib/config.hpp"
#include "nomlib/resources.hpp"
#include "nomlib/system/IFile.hpp"

/*
#if defined ( FRAMEWORK ) && defined ( NOM_PLATFORM_OSX )
  #include "nomlib/system/osx/ResourcePath.hpp"
#endif
*/

namespace nom {

/// \brief Platform-specific interface for file based access in Unix
class UnixFile: public IFile
{
  public:
    UnixFile ( void );
    ~UnixFile ( void );

    /// Returns the file extension of the input file path
    ///
    /// Returns a null terminated string on err
    ///
    const std::string extension ( const std::string& file );

    /// Uses stat(2) to determine input file size (in bytes)
    ///
    /// -1 on err
    int32 size ( const std::string& file_path );

    /// Uses stat(2) to determine if the input file exists
    ///
    bool exists ( const std::string& file_path );

    /// dirname(3) wrapper
    ///
    /// Extract the directory portion of a pathname
    ///
    /// dir_path is arbitrarily limited to the POSIX standard of 256 characters
    /// for maximum cross-platform portability.
    const std::string path ( const std::string& dir_path );

    /// getcwd(3) wrapper
    ///
    /// Return path is arbitrarily limited to the POSIX standard of 256
    /// characters for maximum cross-platform portability.
    const std::string currentPath ( void );

    /// chdir(2) wrapper
    void setPath ( const std::string& path );
};


} // namespace nom

#endif // include guard

/// \class nom::UnixFile
///
