/******************************************************************************
    OSXFs.cpp

    (OSX) Filesystem Interface

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "OSXFs.h"

OSXFs::OSXFs ( void )
{
  #ifdef DEBUG_OSXFS_OBJ
    std::cout << "OSXFs::OSXFs (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}

OSXFs::~OSXFs ( void )
{
  #ifdef DEBUG_OSXFS_OBJ
    std::cout << "OSXFs::~OSXFs (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}

const std::string OSXFs::dirName ( std::string path )
{
  // Stub
}

const std::string OSXFs::getWorkingDir ( void )
{
  char path[PATH_MAX];

  getcwd ( path, PATH_MAX );

  // ERR checking

  std::string cwd ( path );

  return cwd;
}

void OSXFs::setWorkingDir ( std::string path )
{
  // Stub
}
