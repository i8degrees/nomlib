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
#ifndef NOMLIB_WINDOWS_FILE_HEADERS
#define NOMLIB_WINDOWS_FILE_HEADERS

#include <string>
#include <cstring>
#include <vector>

#include <stdlib.h>
#include <errno.h>
#include <direct.h>

// FIXME:
// #include <windows.h>
// #include <tchar.h>
// #include <strsafe.h>

#include "nomlib/config.hpp"
#include "nomlib/system/Path.hpp"
#include "nomlib/system/IFile.hpp"

namespace nom {

/// \brief Platform-specific interface for file based access in Windows
class WinFile: public IFile
{
  public:
    WinFile ( void );
    ~WinFile ( void );

    /// Implements nom::IFile::extension
    ///
    /// \remarks See nom::IFile::extension
    const std::string extension ( const std::string& file );

    /// Implements nom::IFile::size
    ///
    /// \remarks See nom::IFile::size
    int32 size( const std::string& file_path );

    /// \brief Test for the existence of a directory.
    ///
    /// \todo Verify working functionality.
    bool is_dir( const std::string& file_path );

    /// Implements nom::IFile::exists
    ///
    /// \remarks See nom::IFile::exists.
    ///
    /// \note http://stackoverflow.com/questions/18320486/how-to-check-if-file-exists-in-c-in-a-portable-way
    /// \note http://msdn.microsoft.com/en-us/library/windows/desktop/aa365522(v=vs.85).aspx
    bool exists( const std::string& file_path );

    /// Implements nom::IFile::path
    ///
    /// Emulates POSIX dirname() function; see man (3) dirname.
    ///
    /// \todo This should probably be renamed to dirname?
    ///
    /// Implements nom::IFile::path
    const std::string path ( const std::string& dir_path );

    /// Implements nom::IFile::currentPath
    ///
    /// \remarks See nom::IFile::currentPath
    const std::string currentPath ( void );

    /// Implements nom::IFile::set_path
    ///
    /// \remarks See nom::IFile::set_path
    bool set_path ( const std::string& path );

    /// Implements nom::IFile::basename
    ///
    /// \remarks See nom::IFile::basename
    const std::string basename ( const std::string& filename );

    /// \brief Get a list of file entries within a directory.
    ///
    /// \remarks The directory entries '.' and '..' are removed from the file
    /// listing.
    ///
    /// \todo Implement optional directory recursion.
    ///
    /// \note http://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx
    std::vector<std::string> read_dir( const std::string& dir_path );
};


} // namespace nom

#endif // include guard

/// \class nom::WinFile
///
