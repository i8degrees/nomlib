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

void Logger::info ( const std::string& message )
{
  std::cout << "NOMLIB_LOG_INFO at " << Logger::getCurrentTime() << message << std::endl << std::endl;
}

void Logger::err ( const std::string& file, uint32 line, const std::string& reason )
{
  std::cout << "NOMLIB_LOG_ERR at " << Logger::getCurrentTime() << "In file " << file << ":" << line << std::endl << "Reason: " << reason << std::endl << std::endl;
}

std::string Logger::getCurrentTime ( void )
{
  time_t timer = time ( nullptr );

  return ctime ( &timer );
}

} // namespace nom
