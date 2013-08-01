/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_LOGGER_HEADERS
#define NOMLIB_LOGGER_HEADERS

#include <iostream>
#include <string>
#include <cassert>

#include "nomlib/config.hpp"

#include "nomlib/system/Clock.hpp"

// Pretty print C macros purely for convenience sake
#define NOM_DUMP_VAR(var) \
  ( std::cout << std::endl << #var << ": " << var << std::endl << std::endl )

// nomlib debugging macros
#ifdef NOM_DEBUG

  // If debugging is turned on, we log all warnings, errors & info
  #define NOM_LOG_INFO(message) \
    ( nom::Logger::info ( message ) )

  #define NOM_LOG_ERR(message) \
    ( nom::Logger::err ( __FILE__, __LINE__, message ) )

#else // Do not add any overhead

  #define NOM_LOG_INFO(message)
  #define NOM_LOG_ERR(message)

#endif

#ifdef NOM_DEBUG_ASSERT

  #define NOM_ASSERT(expression) \
    ( assert (expression) )

#else // Do not add any overhead

  #define NOM_ASSERT(expression)

#endif

#ifdef NOM_DEBUG_TRACE

  // If trace debugging is turned on, we show class construction and destruction
  #define NOM_LOG_CLASSINFO \
    ( nom::Logger::class_info ( __func__ ) )

#else // Do not add any overhead

  #define NOM_LOG_CLASSINFO

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
