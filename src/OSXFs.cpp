/******************************************************************************
    OSXFS.cpp

  (OSX) Filesystem Interface

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "OSXFS.hpp"

nom::OSXFS::OSXFS ( void )
{
  #ifdef DEBUG_OSXFS_OBJ
    std::cout << "nom::OSXFS::OSXFS (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}

nom::OSXFS::~OSXFS ( void )
{
  #ifdef DEBUG_OSXFS_OBJ
    std::cout << "nom::OSXFS::~OSXFS (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}

// Not sure how else to do this but in straight C
const char* nom::OSXFS::getDirName ( char *path )
{
  // ERR checking

  return dirname ( path );
}

const std::string nom::OSXFS::getWorkingDir ( void )
{
  char path[PATH_MAX];

  getcwd ( path, PATH_MAX );

  // ERR checking

  std::string cwd ( path );

  return cwd;
}

void nom::OSXFS::setWorkingDir ( std::string path )
{
  chdir ( path.c_str() );

  // ERR checking
}
