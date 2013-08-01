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
#include <nomlib/system/UnixFile.hpp>

namespace nom {

UnixFile::UnixFile ( void )
{
}

UnixFile::~UnixFile ( void )
{
}

const std::string UnixFile::mime ( const std::string& file )
{
  magic_t cookie;
  std::string extension = "\0";

  cookie = magic_open ( MAGIC_MIME_TYPE );

  if ( cookie == nullptr )
  {
NOM_LOG_ERR ( "Could not initialize magic library." );

    magic_close ( cookie );

    return extension;
  }

  // TODO: use magic_error ( magic_t cookie ) to obtain err?
  if ( magic_load ( cookie, nullptr ) != 0 )
  {
NOM_LOG_ERR ( "Could not read magic database." );

    magic_close ( cookie );

    return extension;
  }

  extension = magic_file ( cookie, file.c_str() );

  magic_close ( cookie );

  return extension;
}

const std::string UnixFile::extension ( const std::string& file )
{
  std::string extension = "\0";

  if ( file.find_last_of ( "." ) != std::string::npos )
    extension = file.substr ( file.find_last_of (".") + 1 ); // match found

   // if we reach this point, return false (a null terminated string)
  return extension;
}

int32 UnixFile::size ( const std::string& file_path )
{
  struct stat file;

  if ( ! stat ( file_path.c_str(), &file ) )
  {
    return file.st_size;
  }

  return -1;
}

bool UnixFile::exists ( const std::string& file_path )
{
  struct stat file;

  if ( stat ( file_path.c_str(), &file ) != 0 || !S_ISREG ( file.st_mode ))
    return false;

  return true;
}

const std::string UnixFile::path ( const std::string& dir_path )
{
  // We must do this string conversion -- from a std::string to a char
  // pointer -- because the c_str method returns a const char pointer, which
  // cannot be used for the dirname function, as this call modifies the contents
  // of the passed variable.

  char path[PATH_MAX];

  std::strcpy ( path, dir_path.c_str() );

  return dirname ( path );
}

const std::string UnixFile::currentPath ( void )
{
  char path[PATH_MAX];

  getcwd ( path, PATH_MAX );

  std::string cwd ( path );

  return cwd;
}

void UnixFile::setPath ( const std::string& path )
{
  chdir ( path.c_str() );
}


} // namespace nom
