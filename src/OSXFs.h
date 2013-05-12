/******************************************************************************
    OSXFs.h

    (OSX) Filesystem Interface

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_OSXFS_HEADERS
#define GAMELIB_OSXFS_HEADERS

#include <iostream>
#include <string>
#include <unistd.h>
#include <libgen.h>

#include "gamelib.h"

class OSXFs
{
  public:
    OSXFs ( void );
    ~OSXFs ( void );

    static const char *getDirName ( char *path ); // dirname(3) wrapper
    static const std::string getWorkingDir ( void ); // getcwd(3) wrapper
    static void setWorkingDir ( std::string path ); // chdir(2) wrapper

  private:
    // ...
};

#endif // GAMELIB_OSXFS_HEADERS defined
