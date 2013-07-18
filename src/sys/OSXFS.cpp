/******************************************************************************

    (OSX) Filesystem Interface

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "nomlib/sys/OSXFS.hpp"

namespace nom {

OSXFS::OSXFS ( void )
{
}

OSXFS::~OSXFS ( void )
{
}

const std::string OSXFS::getFileType ( const std::string& file )
{
  magic_t cookie;
  std::string extension = "\0";

  cookie = magic_open ( MAGIC_MIME_TYPE );

  if ( cookie == nullptr )
  {
NOMLIB_LOG_ERR ( "Could not initialize magic library." );

    magic_close ( cookie );

    return extension;
  }

  // TODO: use magic_error ( magic_t cookie ) to obtain err?
  if ( magic_load ( cookie, nullptr ) != 0 )
  {
NOMLIB_LOG_ERR ( "Could not read magic database." );

    magic_close ( cookie );

    return extension;
  }

  extension = magic_file ( cookie, file.c_str() );

  magic_close ( cookie );

  return extension;
}
}

int32 OSXFS::getFileSize ( const std::string& file_path )
{
  struct stat file;

  if ( ! stat ( file_path.c_str(), &file ) )
  {
    return file.st_size;
  }

  return -1;
}

bool OSXFS::fileExists ( const std::string& file_path )
{
  struct stat file;

  if ( stat ( file_path.c_str(), &file ) != 0 || !S_ISREG ( file.st_mode ))
    return false;

  return true;
}

const std::string OSXFS::getBaseName ( const std::string& dir_path )
{
  // We must do this string conversion -- from a std::string to a char
  // pointer -- because the c_str method returns a const char pointer, which
  // cannot be used for the dirname function, as this call modifies the contents
  // of the passed variable.

  char path[PATH_MAX];

  std::strcpy ( path, dir_path.c_str() );

  return basename ( path );
}

const std::string OSXFS::getDirName ( const std::string& dir_path )
{
  // We must do this string conversion -- from a std::string to a char
  // pointer -- because the c_str method returns a const char pointer, which
  // cannot be used for the dirname function, as this call modifies the contents
  // of the passed variable.

  char path[PATH_MAX];

  std::strcpy ( path, dir_path.c_str() );

  return dirname ( path );
}

const std::string OSXFS::getWorkingDir ( void )
{
  char path[PATH_MAX];

  getcwd ( path, PATH_MAX );

  std::string cwd ( path );

  return cwd;
}

void OSXFS::setWorkingDir ( const std::string& path )
{
  chdir ( path.c_str() );
}

// FIXME
#if defined NOMLIB_SYSTEM_OSX

const std::string OSXFS::getResourcesPath ( void )
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

#endif


} // namespace nom
