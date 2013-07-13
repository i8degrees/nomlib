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

void Logger::al_check_err ( const std::string& file, uint32 line )
{
  ALenum error_code = alGetError();

  if ( error_code != AL_NO_ERROR )
  {
    std::string error, description;

    switch ( error_code )
    {
      default:
      {
        error = "Unknown";
        description = "An unknown error has occurred.";
      }
      break;

      case AL_INVALID_NAME:
      {
        error = "AL_INVALID_NAME";
        description = "Invalid name (identifier).";
      }
      break;

      case AL_INVALID_ENUM:
      {
        error = "AL_INVALID_ENUM";
        description = "Invalid enumeration (attribute).";
      }
      break;

      case AL_INVALID_VALUE:
      {
        error = "AL_INVALID_VALUE";
        description = "Invalid value.";
      }
      break;

      case AL_INVALID_OPERATION:
      {
        error = "AL_INVALID_OPERATION";
        description = "Requested operation is not valid.";
      }
      break;

      case AL_OUT_OF_MEMORY:
      {
        error = "AL_OUT_OF_MEMORY";
        description = "Requested operation resulted in OpenAL running out of memory";
      }
      break;
    } // end switch

  std::cout << "NOMLIB_LOG_ERR at " << getCurrentTime() << "In file " << file << ":" << line << std::endl << "Error: " << error << ", " << description << std::endl << std::endl;

  } // end if AL_NO_ERROR
}

} // namespace nom
