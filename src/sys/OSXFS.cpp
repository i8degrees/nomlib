/******************************************************************************

    (OSX) Filesystem Interface

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "OSXFS.hpp"

nom::OSXFS::OSXFS ( void )
{
NOMLIB_LOG_INFO;
}

nom::OSXFS::~OSXFS ( void )
{
NOMLIB_LOG_INFO;
}

nom::int32 nom::OSXFS::getFileSize ( const std::string& file_path )
{
  struct stat file;

  if ( ! stat ( file_path.c_str(), &file ) )
  {
    return file.st_size;
  }

  return -1;
}

bool nom::OSXFS::fileExists ( const std::string& file_path )
{
  struct stat file;

  if ( stat ( file_path.c_str(), &file ) != 0 || !S_ISREG ( file.st_mode ))
    return false;

  return true;
}

const std::string nom::OSXFS::getBaseName ( const std::string& dir_path )
{
  // We must do this string conversion -- from a std::string to a char
  // pointer -- because the c_str method returns a const char pointer, which
  // cannot be used for the dirname function, as this call modifies the contents
  // of the passed variable.

  char path[PATH_MAX];

  std::strcpy ( path, dir_path.c_str() );

  return basename ( path );
}

const std::string nom::OSXFS::getDirName ( const std::string& dir_path )
{
  // We must do this string conversion -- from a std::string to a char
  // pointer -- because the c_str method returns a const char pointer, which
  // cannot be used for the dirname function, as this call modifies the contents
  // of the passed variable.

  char path[PATH_MAX];

  std::strcpy ( path, dir_path.c_str() );

  return dirname ( path );
}

const std::string nom::OSXFS::getWorkingDir ( void )
{
  char path[PATH_MAX];

  getcwd ( path, PATH_MAX );

  std::string cwd ( path );

  return cwd;
}

void nom::OSXFS::setWorkingDir ( const std::string& path )
{
  chdir ( path.c_str() );
}

const std::string nom::OSXFS::getResourcesPath ( void )
{
  char resources_path[ PATH_MAX ];
  CFBundleRef mainBundle = CFBundleGetMainBundle();
  CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL ( mainBundle );

  if ( ! CFURLGetFileSystemRepresentation ( resourcesURL, true, ( uint8* ) resources_path, PATH_MAX ) )
  {
NOMLIB_LOG_ERR ( "Could not obtain the bundle's Resources path." );

    CFRelease ( resourcesURL );

    return getWorkingDir();
  }

  CFRelease ( resourcesURL );

  return resources_path;
}
