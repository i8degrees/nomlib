/******************************************************************************

    (OSX) Filesystem Interface

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_OSXFS_HEADERS
#define NOMLIB_OSXFS_HEADERS

#include <iostream>
#include <string>
#include <unistd.h>
#include <libgen.h>
#include <CoreFoundation/CoreFoundation.h>

#include "config.hpp"

#ifndef PATH_MAX
  #define PATH_MAX 1024
#endif

namespace nom {

/// \brief Platform-agnostic file system interface
class OSXFS
{
  public:
    ~OSXFS ( void );

    /// basename(3) wrapper
    ///
    /// dir_path is arbitrarily limited to 1024 characters.
    static const std::string getBaseName ( const std::string& dir_path );

    /// dirname(3) wrapper
    ///
    /// dir_path is arbitrarily limited to 1024 characters.
    static const std::string getDirName ( const std::string& dir_path );

    /// getcwd(3) wrapper
    ///
    /// Returned path is arbitrarily limited to 1024 characters.
    static const std::string getWorkingDir ( void );

    /// chdir(2) wrapper
    static void setWorkingDir ( const std::string& path );

    /// Returns the path to the Resources folder (OSX Application Bundle)
    /// \todo TEST ME
    static const std::string getResourcesPath ( void );

  private:
    OSXFS ( void );
};


} // namespace nom

#endif // NOMLIB_OSXFS_HEADERS defined

/// \class nom::OSXFS
///
/// \todo ERR checking & research PATH_MAX macro #define
