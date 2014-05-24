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
#ifndef NOM_SYSTEM_WINDOWS_WIN_FILE_HPP
#define NOM_SYSTEM_WINDOWS_WIN_FILE_HPP

#include <string>
#include <cstring>
#include <vector>

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
    /// \see http://stackoverflow.com/questions/18320486/how-to-check-if-file-exists-in-c-in-a-portable-way
    /// \see http://msdn.microsoft.com/en-us/library/windows/desktop/aa365522(v=vs.85).aspx
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
    ///
    /// \see http://msdn.microsoft.com/ru-ru/library/ms235450.aspx
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
    /// \see http://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx
    std::vector<std::string> read_dir( const std::string& dir_path );

    /// \brief Get the path to the Resources folder.
    ///
    /// \param identifier Not implemented; reserved for future use.
    ///
    /// \returns On success, returns the current working directory of the
    /// executable with '\Resources" appended onto the end. On failure to: a)
    /// interpret the path, using ::path, '\Resources' will be returned; b) out
    /// of buffer space (see platforms.hpp, PATH_MAX preprocessor define), a
    /// null-terminated string will be returned.
    ///
    /// \see http://msdn.microsoft.com/en-us/library/ms683197%28v=vs.85%29.aspx
    const std::string resource_path( const std::string& identifier = "\0" );

    /// Obtain the path to the logged in user's Documents folder
    ///
    ///     $HOME/Documents
    ///
    /// \remark This is a standard folder that may be used for saving user data
    ///
    /// \note This function requires Windows Vista or above
    ///
    /// \todo Support for when the user has changed the default save folder; see http://stackoverflow.com/a/12607759 and http://stackoverflow.com/questions/19553311/windows-8-how-to-read-the-user-documents-folder-path-programmatically-using-c
    const std::string user_documents_path( void );

    /// Obtain the path to the logged in user's (local) Application Data folder
    ///
    ///     $HOME/AppData/Local
    ///
    /// \remark This is a standard folder that may be used for saving user data
    ///
    /// \note This function requires Windows Vista or above
    ///
    /// \todo Support for when the user has changed the default save folder; see http://stackoverflow.com/a/12607759 and http://stackoverflow.com/questions/19553311/windows-8-how-to-read-the-user-documents-folder-path-programmatically-using-c
    const std::string user_app_support_path( void );
};


} // namespace nom

#endif // include guard

/// \class nom::WinFile
/// \ingroup system
