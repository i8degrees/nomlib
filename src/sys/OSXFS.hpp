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

#include "config.hpp"

#ifndef PATH_MAX
  #define PATH_MAX 1024 // TODO: research this further, no idea if 1024 is legit
#endif

namespace nom
{
  class OSXFS
  {
    public:
      OSXFS ( void );
      ~OSXFS ( void );

      static const char* getDirName ( char* path ); // dirname(3) wrapper
      static const std::string getWorkingDir ( void ); // getcwd(3) wrapper
      static void setWorkingDir ( std::string path ); // chdir(2) wrapper

    private:
      // ...
  };
}

#endif // NOMLIB_OSXFS_HEADERS defined
