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

#include "nomlib/config.hpp"

#include "nomlib/sys/Clock.hpp"

// Pretty print C macros
#ifdef NOMLIB_DEBUG

  // If debugging is turned on, we log all warnings, errors & info
  #define NOM_LOG_INFO(message) \
    ( nom::Logger::info ( message ) )
  #define NOM_LOG_ERR(message) \
    ( nom::Logger::err ( __FILE__, __LINE__, message ) )
  #define NOM_ASSERT(expression) \
    ( assert (expression) )
  #define NOM_DUMP_VAR(var) \
  ( std::cout << std::endl << #var << ": " << var << std::endl << std::endl )

#else // Do not add any overhead

  #define NOM_LOG_INFO(message)
  #define NOM_LOG_ERR(message)
  #define NOM_ASSERT(expression)
  #define NOM_DUMP_VAR(var)

#endif

#ifdef NOMLIB_DEBUG_TRACE

  // If trace debugging is turned on, we show class construction and destruction
  #define NOM_LOG_CLASSINFO \
    ( nom::Logger::class_info ( __func__ ) )

#else // Do not add any overhead

  #define NOM_LOG_CLASSINFO

#endif

#ifdef NOMLIB_RELEASE_WITH_ASSERT

  #undef NOM_ASSERT
  #define NOM_ASSERT(expression) \
    ( assert (expression) )

#endif

namespace nom {

class Logger
{
  public:
    ~Logger ( void );

    /// Logging of class info
    static void class_info ( const std::string& message );

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
