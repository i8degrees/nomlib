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
  std::cout << "NOMLIB_LOG_INFO at " << getCurrentTime() << message << std::endl << std::endl;
}

void Logger::err ( const std::string& file, uint32 line, const std::string& reason )
{
  std::cout << "NOMLIB_LOG_ERR at " << getCurrentTime() << "In file " << file << ":" << line << std::endl << "Reason: " << reason << std::endl << std::endl;
}

} // namespace nom
