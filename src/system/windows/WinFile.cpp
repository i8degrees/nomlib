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
#include "nomlib/system/windows/WinFile.hpp"

// Private headers (third-party libs)
#include <windows.h>
#include <shlobj.h>
#include <tchar.h>
#include <strsafe.h>

// Private headers
#include "nomlib/system/Path.hpp"

namespace nom {

WinFile::WinFile( void )
{
  // NOM_LOG_TRACE( NOM );
}

WinFile::~WinFile( void )
{
  // NOM_LOG_TRACE( NOM );
}

const std::string WinFile::extension ( const std::string& file )
{
  std::string extension = "\0";

  if ( file.find_last_of ( "." ) != std::string::npos ) // match found
  {
    extension = file.substr ( file.find_last_of (".") + 1 );
  }

  // if we reach this point, return false (a null terminated string)
  return extension;
}

int32 WinFile::size( const std::string& file_path )
{
  NOM_STUBBED( NOM );

  return 0;
}

bool WinFile::is_dir( const std::string& file_path )
{
  DWORD attr = 0;

  attr = GetFileAttributes( file_path.c_str() );

  if( attr == FILE_ATTRIBUTE_DIRECTORY )
  {
    return true;
  }

  return false;
}

bool WinFile::exists( const std::string& file_path )
{
  DWORD attr = 0;

  attr = GetFileAttributes( file_path.c_str() );

  if( attr != INVALID_FILE_ATTRIBUTES )
  {
    return true;
  }

  return false;
}

const std::string WinFile::path( const std::string& dir_path )
{
  Path p; // Just to be safe, we'll let nom::Path determine our path separator!

  int32 pos = dir_path.find_last_of( p.native(), PATH_MAX );

  // If no matches are found, this means the file path given is actually a base
  // file name path, without any directory path at all.
  // See also 'man 3 basename'
  if( pos == std::string::npos ) return ".";

  // A match was found -- return only the directory path leading up to the
  // file name path, without a a trailing path separator.
  return dir_path.substr( 0, pos );
}

const std::string WinFile::currentPath ( void )
{
  char* buffer = nullptr;
  std::string pwd;

  if( (buffer = _getcwd( nullptr, 0 ) ) == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not get the current working directory path." );
    return pwd; // NULL
  }
  else
  {
    pwd = buffer;
    free( buffer );
  }

  return pwd;
}

bool WinFile::set_path ( const std::string& path )
{
  if ( _chdir ( path.c_str() ) != 0 )
  {
NOM_LOG_ERR ( NOM, "Unknown error on attempt to change working directory to: " + path );
    return false;
  }

  return true;
}

const std::string WinFile::basename ( const std::string& filename )
{
  nom::size pos = 0;
  pos = filename.find_last_of ( ".", PATH_MAX );

  // If no matches are found, return the input filename back to the end-user.
  if ( pos == std::string::npos ) return filename; // Failure

  // Bingo! A match was found -- return everything leading up to --
  // the last '.' character found (excluding the dot).
  return filename.substr ( 0, pos );
}

std::vector<std::string> WinFile::read_dir( const std::string& dir_path )
{
  WIN32_FIND_DATA ffd;
  // LARGE_INTEGER filesize;
  TCHAR szDir[PATH_MAX];
  size_t length_of_arg;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  DWORD dwError = 0;
  std::string entry;              // A file entry within a directory
  std::vector<std::string> files; // Files found within directory

  // Check that the input path plus 3 is not longer than PATH_MAX.
  // Three characters are for the "\*" plus NULL appended below.
  StringCchLength( dir_path.c_str(), PATH_MAX, &length_of_arg );

  if( length_of_arg > ( PATH_MAX - 3 ) )
  {
    NOM_LOG_ERR( NOM, "Directory path is too long." );
    return files;
  }

  // Prepare string for use with FindFile functions.  First, copy the
  // string to a buffer, then append '\*' to the directory name.
  StringCchCopy( szDir, PATH_MAX, dir_path.c_str() );
  StringCchCat( szDir, PATH_MAX, TEXT("\/*") );

  // Find the first file in the directory.
  hFind = FindFirstFile( szDir, &ffd );

  if( INVALID_HANDLE_VALUE == hFind )
  {
    NOM_LOG_ERR( NOM, "Could not read file in directory." );
    FindClose( hFind );
    return files;
  }

  // List all the files in the directory with some info about them.
  do
  {
    if( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
    {
      // TODO: Handle directory case

      // files.push_back( std::string( ffd.cFileName ) );
    }
    else
    {
      // filesize.LowPart = ffd.nFileSizeLow;
      // filesize.HighPart = ffd.nFileSizeHigh;

      entry = ffd.cFileName;

      if( entry != "." && entry != ".." )
      {
        files.push_back( entry );
      }
    }
  }

  while( FindNextFile( hFind, &ffd ) != 0 );

  dwError = GetLastError();
  if( dwError != ERROR_NO_MORE_FILES )
  {
    NOM_LOG_ERR( NOM, "File listing is not EOF." );
  }

  FindClose( hFind );

  return files;
}

const std::string WinFile::resource_path( const std::string& identifier )
{
  Path p; // Platform-agnostic path separator
  DWORD ret = 0;
  CHAR szPath[PATH_MAX];
  std::string pwd( "\0" );

  ret = GetModuleFileName( nullptr, szPath, PATH_MAX );

  // The parent working directory string will include the executable's file
  // name (including extension).
  if( ret == ERROR_INSUFFICIENT_BUFFER )
  {
    NOM_LOG_ERR( NOM, "Could not return resource path: insufficient PATH_MAX buffer: " + PATH_MAX );
    return pwd; // NULL
  }

  // Sanitize the complete file path and return just the directory path portion
  pwd = this->path( szPath );

  // In case of err, assume / hope that we are in the parent directory leading
  // to the Resources directory, with a closing slash.
  if( pwd == "." )
  {
    return p.native() + "Resources";
  }

  // Assumed success!
  return pwd += p.native() + "Resources";
}

const std::string WinFile::user_documents_path( void )
{
  CHAR path[PATH_MAX];

  HRESULT result =  SHGetFolderPath(  nullptr,
                                      CSIDL_PERSONAL,
                                      nullptr,
                                      SHGFP_TYPE_CURRENT,
                                      path );

  if ( result != S_OK ) return "\0";

  return path;
}

const std::string WinFile::user_app_support_path( void )
{
  CHAR path[PATH_MAX];

  HRESULT result =  SHGetFolderPath(  nullptr,
                                      CSIDL_LOCAL_APPDATA,
                                      nullptr,
                                      SHGFP_TYPE_CURRENT,
                                      path );

  if ( result != S_OK ) return "\0";

  return path;
}

const std::string WinFile::user_home_path( void )
{
  CHAR path[PATH_MAX];

  HRESULT result =  SHGetFolderPath (
                                      nullptr,
                                      CSIDL_PROFILE,
                                      nullptr,
                                      SHGFP_TYPE_CURRENT,
                                      path
                                    );

  if( result != S_OK )
  {
    // Err
    return "\0";
  }

  // Success!
  return path;
}

const std::string WinFile::system_path( void )
{
  CHAR path[PATH_MAX];

  HRESULT result =  SHGetFolderPath (
                                      nullptr,
                                      CSIDL_WINDOWS,
                                      nullptr,
                                      SHGFP_TYPE_CURRENT,
                                      path
                                    );

  if( result != S_OK )
  {
    // Err
    return "\0";
  }

  // Success!
  return path;
}

} // namespace nom
