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
#include "nomlib/system/unix/UnixFile.hpp"

// Private headers (third-party libs)
#include <unistd.h>
//#include <libgen.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>

#if defined( NOM_PLATFORM_OSX )
  #include <CoreServices/CoreServices.h>
  #include <CoreFoundation/CoreFoundation.h>
#endif

// Private headers
#include "nomlib/system/Path.hpp"

namespace nom {

UnixFile::UnixFile( void )
{
  // NOM_LOG_TRACE( NOM );
}

UnixFile::~UnixFile( void )
{
  // NOM_LOG_TRACE( NOM );
}

const std::string UnixFile::extension ( const std::string& file )
{
  std::string extension = "\0";

  if ( file.find_last_of ( "." ) != std::string::npos ) // match found
  {
    extension = file.substr ( file.find_last_of (".") + 1 );
  }

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

bool UnixFile::is_dir( const std::string& file_path )
{
  struct stat fp;

  if( stat( file_path.c_str(), &fp ) == 0 )
  {
    if( fp.st_mode & S_IFDIR )
    {
      return true;
    }
  }

  return false;
}

bool UnixFile::is_file( const std::string& file_path )
{
  struct stat fp;

  if( stat( file_path.c_str(), &fp ) == 0 && ( S_ISREG( fp.st_mode ) ) )
  {
    return true;
  }

  return false;
}

bool UnixFile::exists( const std::string& file_path )
{
  return( this->is_dir( file_path ) || this->is_file( file_path ) );
}

// const std::string UnixFile::path ( const std::string& dir_path )
// {
//   // We must do this string conversion -- from a std::string to a char
//   // pointer -- because the c_str method returns a const char pointer, which
//   // cannot be used for the dirname function, as this call modifies the contents
//   // of the passed variable.

//   char path[PATH_MAX];

//   std::strcpy ( path, dir_path.c_str() );

//   return dirname ( path );
// }

const std::string UnixFile::path ( const std::string& dir_path )
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

std::string UnixFile::currentPath( void )
{
  char path[PATH_MAX];

  if ( getcwd ( path, PATH_MAX ) == nullptr )
  {
    NOM_LOG_ERR ( NOM, "Unknown error on attempt to obtain current working directory." );
    return "\0";
  }

  std::string cwd ( path );

  return cwd;
}

bool UnixFile::set_path ( const std::string& path )
{
  if ( chdir ( path.c_str() ) != 0 )
  {
NOM_LOG_ERR ( NOM, "Unknown error on attempt to change working directory to: " + path );
    return false;
  }

  return true;
}

const std::string UnixFile::basename ( const std::string& filename )
{
  nom::size_type pos = 0;
  pos = filename.find_last_of ( ".", PATH_MAX );

  // If no matches are found, return the input filename back to the end-user.
  if ( pos == std::string::npos ) return filename; // Failure

  // Bingo! A match was found -- return everything leading up to --
  // the last '.' character found (excluding the dot).
  return filename.substr ( 0, pos );
}

std::vector<std::string> UnixFile::read_dir( const std::string& dir_path )
{
  DIR *dp = nullptr;
  dirent *ep = nullptr;
  bool eof = false;               // Tracks the end of the directory
  std::string entry;              // A file entry within a directory
  std::vector<std::string> files; // Files found within directory

  dp = opendir( dir_path.c_str() );

  if( dp != nullptr )
  {
    while( eof == false )
    {
      ep = readdir( dp );

      if( ep != nullptr )
      {
        entry = ep->d_name;

        // TODO: Handle directory case
        // if( this->is_dir( dir_path + "/" + entry ) == true )
        // {
        // }

        // Sanitize the file entries
        if( entry != "." && entry != ".." )
        {
          files.push_back( entry );
        }
        // NOM_DUMP(entry);
      }
      else
      {
        eof = true;
      }
    }

    closedir( dp );
  }
  else
  {
    NOM_LOG_ERR( NOM, "Could not read directory path: " + dir_path );
  }

  return files;
}

const std::string UnixFile::resource_path( const std::string& identifier )
{
  return "\0";
/*
  char resources_path [ PATH_MAX ]; // file-system path
  CFBundleRef bundle; // bundle type reference

  // Look for a bundle using its identifier if string passed is not null
  // terminated
  if ( identifier != "\0" )
  {
    CFStringRef identifier_ref; // Apple's string type

    identifier_ref = CFStringCreateWithCString  ( nullptr, identifier.c_str(),
                                                  strlen ( identifier.c_str() )
                                                );

    bundle = CFBundleGetBundleWithIdentifier ( identifier_ref );
  }
  else // Assume that we are looking for the top-level bundle's Resources path
  {
    bundle = CFBundleGetMainBundle();
  }

  CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL ( bundle );

  if ( ! CFURLGetFileSystemRepresentation ( resourcesURL, true, ( uint8* ) resources_path, PATH_MAX ) )
  {
    NOM_LOG_ERR ( NOM, "Could not obtain the bundle's Resources path." );

    CFRelease ( resourcesURL );

    return "\0";
  }

  CFRelease ( resourcesURL );

  return resources_path;
  */
}

//#pragma clang diagnostic push
//#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

const std::string UnixFile::user_documents_path( void )
{
  return "/home/jeff/Documents";
/*
  FSRef ref;
  OSType folderType = kDocumentsFolderType;
  char path[PATH_MAX];

  FSFindFolder ( kUserDomain, folderType, kCreateFolder, &ref );

  FSRefMakePath ( &ref, (uint8*) &path, PATH_MAX );

  return std::string ( path );
  */
}

const std::string UnixFile::user_app_support_path( void )
{
  return "/home/jeff/.config";
  /*
  FSRef ref;
  OSType folderType = kApplicationSupportFolderType;
  char path[PATH_MAX];

  FSFindFolder ( kUserDomain, folderType, kCreateFolder, &ref );

  FSRefMakePath ( &ref, (uint8*) &path, PATH_MAX );

  return std::string ( path );
  */
}

const std::string UnixFile::user_home_path( void )
{
  return "/home/jeff";
  /*
  char path[PATH_MAX];
  FSRef ref;
  OSType folderType = kCurrentUserFolderType;

  FSFindFolder( kUserDomain, folderType, kCreateFolder, &ref );

  FSRefMakePath( &ref, ( uint8* ) &path, PATH_MAX );

  return path;*/
}

const std::string UnixFile::system_path( void )
{
  return "\0";
  /*
  char path[PATH_MAX];
  FSRef ref;
  OSType folderType = kSystemFolderType;

  FSFindFolder( kUserDomain, folderType, kCreateFolder, &ref );

  FSRefMakePath( &ref, ( uint8* ) &path, PATH_MAX );

  return path;
*/
}

// const std::string UnixFile::system_library_path( void )
// {
//   Path p( this->system_path() );

//   return p.prepend( "Library" );
// }

// const std::string UnixFile::system_fonts_path( void )
// {
//   Path p( this->system_library_path() );

//   return p.prepend( "Fonts" );
// }

//#pragma clang diagnostic pop

bool UnixFile::mkdir( const std::string& path )
{
  int ret = 0;
  mode_t perms = 0755;

  if( this->exists( path ) == false )
  {
    ret = ::mkdir( path.c_str(), perms );

    if( ret == 0 )
    {
      // Success!
      return true;
    }
  }
  else if( this->exists( path ) == true )
  {
    NOM_LOG_ERR( NOM, "Could not create directory -- file or directory path exists: ", path );
    return false;
  }

  // Unknown err
  return false;
}

bool UnixFile::recursive_mkdir( const std::string& path )
{
  nom::size_type pos = std::string::npos;

  Path p;
  std::string delimiter = p.native();

  if( this->exists( path ) == false )
  {
    pos = path.find_last_of( p.native() );

    if( pos != std::string::npos )
    {
      this->recursive_mkdir( path.substr( 0, pos ) );
    }

    if( this->mkdir( path.c_str() ) == true )
    {
      return true;
    }
  }
  else if( this->exists( path ) == true )
  {
    return false;
  }

  // Unknown err
  return false;
}

bool UnixFile::rmdir( const std::string& path )
{
  int ret = 0;

  ret = ::rmdir( path.c_str() );

  if( ret == 0 )
  {
    // Successful directory removal
    return true;
  }

  // Err (path not found or access denied at existing path?)
  return false;
}

bool UnixFile::mkfile( const std::string& path )
{
  std::ofstream fp;

  fp.open( path );

  if( fp.is_open() && fp.good() )
  {
    fp << "";

    // Success!
    return true;
  }

  // Err (not defined)
  return false;
}

std::string UnixFile::env( const std::string& path )
{
  char* value = nullptr;

  value = getenv( path.c_str() );

  if( value != nullptr )
  {
    return value;
  }

  // Err
  return "\0";
}

} // namespace nom
