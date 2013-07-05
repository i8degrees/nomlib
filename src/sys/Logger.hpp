/******************************************************************************

    Logger class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_LOGGER_HEADERS
#define NOMLIB_LOGGER_HEADERS

#include <iostream>
#include <string>
#include <ctime>

#include <SDL/SDL.h>

#include "nomlib_types.hpp"

namespace nom {

class Logger
{
  public:
    ~Logger ( void );

    /// Logging of info messages
    static void info ( std::string message );
    /// Logging of error messages
    static void err ( std::string file, int32 line, std::string reason = "\0" );

  private:
    Logger ( void );
    static std::string getCurrentTime ( void );
};

} // namespace nom

#endif // NOMLIB_LOGGER_HEADERS defined
