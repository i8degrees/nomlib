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
#ifndef NOMLIB_FILE_HEADERS
#define NOMLIB_FILE_HEADERS

#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#include <nomlib/config.hpp>
#include <nomlib/system/Logger.hpp>
#include <nomlib/system/IFile.hpp>

#if defined (NOM_PLATFORM_OSX) || defined (NOM_PLATFORM_LINUX)

  #include <nomlib/system/UnixFile.hpp>

#elif defined (NOM_PLATFORM_WINDOWS)

  #include <nomlib/system/WinFile.hpp>

#endif

namespace nom {

/// \brief Platform-agnostic file system interface
class File
{
  public:
    File ( void );
    ~File ( void );

    /// Obtain the input file's mime type; uses libmagic to determine said type
    ///
    /// Returns a string in the same format as the following terminal command:
    ///
    /// $ file -b --mime-type <file>
    ///
    /// Returns a null terminated string on err
    const std::string mime ( const std::string& file );

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
    /// dir_path is arbitrarily limited to 1024 characters.
    const std::string path ( const std::string& dir_path );

    /// getcwd(3) wrapper
    ///
    /// Returned path is arbitrarily limited to 1024 characters.
    const std::string currentPath ( void );

    /// chdir(2) wrapper
    void setPath ( const std::string& path );

  private:
    std::shared_ptr<IFile> file;
};


} // namespace nom

#endif // include guard

/// \class nom::File
///
