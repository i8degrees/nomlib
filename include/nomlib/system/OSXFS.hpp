/******************************************************************************

    (OSX) Filesystem Interface

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_OSXFS_HEADERS
#define NOMLIB_OSXFS_HEADERS

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>

#include <magic.h>

#include "nomlib/config.hpp"
#include "nomlib/system/Logger.hpp"

#if defined NOM_PLATFORM_OSX
  #include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef PATH_MAX
  #define PATH_MAX 1024
#endif

namespace nom {

/// \brief Platform-agnostic file system interface
class OSXFS
{
  public:
    OSXFS ( void );
    ~OSXFS ( void );

    /// Obtain the input file's mime type; uses libmagic to determine said type
    ///
    /// Returns a string in the same format as the following terminal command:
    ///
    /// $ file -b --mime-type <file>
    ///
    /// Returns a null terminated string on err
    const std::string getFileType ( const std::string& file );

    /// Returns the file extension of the input file path
    ///
    /// Returns a null terminated string on err
    ///
    const std::string getFileExtension ( const std::string& file );

    /// Uses stat(2) to determine input file size (in bytes)
    ///
    /// -1 on err
    int32 getFileSize ( const std::string& file_path );

    /// Uses stat(2) to determine if the input file exists
    ///
    bool fileExists ( const std::string& file_path );

    /// basename(3) wrapper
    ///
    /// dir_path is arbitrarily limited to 1024 characters.
    const std::string getBaseName ( const std::string& dir_path );

    /// dirname(3) wrapper
    ///
    /// dir_path is arbitrarily limited to 1024 characters.
    const std::string getDirName ( const std::string& dir_path );

    /// getcwd(3) wrapper
    ///
    /// Returned path is arbitrarily limited to 1024 characters.
    const std::string getWorkingDir ( void );

    /// chdir(2) wrapper
    void setWorkingDir ( const std::string& path );

    /// Returns the path to the Resources folder (OSX Application Bundle)
    const std::string getResourcesPath ( void );
};


} // namespace nom

#endif // NOMLIB_OSXFS_HEADERS defined

/// \class nom::OSXFS
///
/// \todo ERR checking & research PATH_MAX macro #define
