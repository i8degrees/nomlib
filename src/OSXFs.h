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

#include "gamelib.h"

class OSXFs
{
  public:
    OSXFs ( void );
    ~OSXFs ( void );

    static const std::string getWorkingDir ( void );

  private:
    // ...
};

#endif // GAMELIB_OSXFS_HEADERS defined
