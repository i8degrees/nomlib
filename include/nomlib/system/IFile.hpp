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
#ifndef NOMLIB_IFILE_HEADERS
#define NOMLIB_IFILE_HEADERS

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#include "nomlib/config.hpp"

namespace nom {

/// \brief Abstract interface class for Platform-agnostic file access
class IFile
{
  public:
    IFile ( void ) {}
    virtual ~IFile ( void ) {}

    /// Returns the file extension of the input file path
    ///
    /// \returns a null terminated string on err
    virtual const std::string extension ( const std::string& file ) = 0;

    /// Uses stat(2) to determine input file size (in bytes)
    ///
    /// \returns -1 on err
    virtual int32 size ( const std::string& file_path ) = 0;

    virtual bool is_dir( const std::string& file_path ) = 0;

    virtual bool is_file( const std::string& file_path ) = 0;

    /// Uses stat(2) to determine if the input file exists
    virtual bool exists( const std::string& file_path ) = 0;

    /// \brief Extract the directory portion of a pathname.
    ///
    /// \see nom::UnixFile::path, nom::WinFile::path.
    virtual const std::string path( const std::string& dir_path ) = 0;

    /// getcwd(3) wrapper
    ///
    /// \remarks Returned path is arbitrarily limited to 1024 characters.
    virtual const std::string currentPath ( void ) = 0;

    /// chdir(2) wrapper
    ///
    /// \remarks Setting the path to '.' will change the working directory to
    /// the directory path of the executable.
    virtual bool set_path ( const std::string& path ) = 0;

    /// Return the base file name
    ///
    /// \param filename   Complete file name (including its suffix/extension)
    ///
    /// \returns          File name path with its file extension removed upon
    ///                   success -- unmodified input filename on failure.
    virtual const std::string basename ( const std::string& filename ) = 0;

    virtual std::vector<std::string> read_dir( const std::string& dir_path ) = 0;

    virtual const std::string resource_path( const std::string& identifier = "\0" ) = 0;

    virtual const std::string user_documents_path( void ) = 0;

    virtual const std::string user_app_support_path( void ) = 0;

    virtual const std::string user_home_path( void ) = 0;
    virtual const std::string system_path( void ) = 0;

    virtual bool mkdir( const std::string& path ) = 0;
    virtual bool recursive_mkdir( const std::string& path ) = 0;
    virtual bool rmdir( const std::string& path ) = 0;

    virtual bool mkfile( const std::string& path ) = 0;
};


} // namespace nom

#endif // include guard

/// \class nom::IFile
///
