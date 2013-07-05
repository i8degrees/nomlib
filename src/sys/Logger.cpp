/******************************************************************************

    Logger Class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Logger.hpp"

namespace nom {

Logger::Logger ( void )
{
  // Nothing to initialize
}

Logger::~Logger ( void )
{
  // Nothing to clean up
}

void Logger::info ( std::string message )
{
  std::string current_time = getCurrentTime();
  std::cout << "NOMLIB_LOG_INFO at " << current_time << message << std::endl << std::endl;
}

void Logger::err ( std::string file, int32 line, std::string reason )
{
  std::string current_time = getCurrentTime();
  std::cout << "NOMLIB_ERR at " << current_time << "In file " << file << ":" << line << std::endl << "Reason: " << reason << std::endl << std::endl;
}

std::string Logger::getCurrentTime ( void )
{
  time_t timer = time ( nullptr );

  return ctime ( &timer );
}

} // namespace nom
