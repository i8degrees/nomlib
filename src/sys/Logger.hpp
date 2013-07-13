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

#include "types.hpp"

#include "audio/AL/OpenAL.hpp"

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

    /// OpenAL general error handling
    static void al_check_err ( const std::string& file, nom::uint32 line );

  private:
    Logger ( void );
    static std::string getCurrentTime ( void );
};


} // namespace nom

#endif // NOMLIB_LOGGER_HEADERS defined
