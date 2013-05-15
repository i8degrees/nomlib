/******************************************************************************
    Logger.cpp

  File I/O Logging

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "Logger.h"

Logger::Logger ( void )
{
  #ifdef DEBUG_LOGGER_OBJ
    std::cout << "Logger::Logger (): Hello, world!" << "\n" << std::endl;
  #endif
}

Logger::~Logger ( void )
{
  #ifdef DEBUG_LOGGER_OBJ
    std::cout << "Logger::~Logger (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( fp.is_open() )
    fp.close();
}

std::vector<int> Logger::Read ( std::string filename )
{
  std::vector<int> val;
  unsigned int value;
  val.clear();

  fp.open( filename );

  if ( fp.is_open() )
  {
    while ( fp.good() )
    {
      fp >> value;
      val.push_back ( value );
    }
    fp.close();
  }
  else
  {
    std::cout << "Logger::~Logger (): ERR at " << filename << std::endl;
  }
  return val;
}

void Logger::List ( std::vector<int> values )
{
  unsigned int idx = 0;

  std::cout << "\nLogger List:\n";
  for ( idx = 0; idx < values.size() - 1; idx++ )
  {
    std::cout << "I: " << values[idx] << "\n";
  }
}
