/******************************************************************************

    Logger class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_LOGGER_HEADERS
#define NOMLIB_LOGGER_HEADERS

#include <iostream>
#include <string>
#include <cassert>

#include "config.hpp"
#include "Clock.hpp"

// Pretty print C macros
#ifdef NOMLIB_DEBUG
  // If debugging is turned on, we log all warnings, errors & info
  #define NOMLIB_LOG(message) \
    ( nom::Logger::info ( message ) )
  #define NOMLIB_LOG_ERR(message) \
    ( nom::Logger::err ( __FILE__, __LINE__, message ) )
  #define NOMLIB_ASSERT(expression) \
    ( assert (expression) )
  #define NOMLIB_DUMP_VAR(var) \
  ( std::cout << std::endl << #var << ": " << var << std::endl << std::endl )
#else // We do not add any overhead
  #define NOMLIB_LOG(message)
  #define NOMLIB_LOG_ERR(message)
  #define NOMLIB_ASSERT(expression)
  #define NOMLIB_DUMP_VAR(var)
#endif

#ifdef NOMLIB_DEBUG_ALL
  // If all debugging is turned on, we show class construction and destruction
  #define NOMLIB_LOG_INFO \
    ( nom::Logger::info ( __func__ ) )
#else // We do not add any overhead
  #define NOMLIB_LOG_INFO
#endif

namespace nom {

class Logger
{
  public:
    ~Logger ( void );

    /// Logging of info messages
    static void info ( const std::string& message );
    /// Logging of error messages
    static void err ( const std::string& file, uint32 line,
                      const std::string& reason = "\0"
                    );

  private:
    Logger ( void );
};


} // namespace nom

#endif // NOMLIB_LOGGER_HEADERS defined
