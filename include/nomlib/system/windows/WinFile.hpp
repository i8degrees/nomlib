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
#ifndef NOMLIB_WINDOWS_FILE_HEADERS
#define NOMLIB_WINDOWS_FILE_HEADERS

#include <string>
#include <cstring>

#include <stdlib.h>
#include <errno.h>
#include <direct.h>

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

    /// Stub
    const std::string extension ( const std::string& file );

    /// Stub
    int32 size ( const std::string& file_path );

    /// Stub
    bool exists ( const std::string& file_path );

    /// Emulates POSIX dirname() function -- see 'man 3 dirname'.
    /// \todo We might consider using this in the UnixFile 
    /// implementation as well, in order to ensure consistency?
    ///
    /// \todo This should probably be renamed to dirname ...
    const std::string path ( const std::string& dir_path );

    /// Stub
    const std::string currentPath ( void );

    void setPath ( const std::string& path );
};


} // namespace nom

#endif // include guard

/// \class nom::WinFile
///
