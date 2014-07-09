/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/system/log.hpp"

// Private headers (third-party libs)
#include "SDL.h"

namespace nom {

// void log ( void* userdata, int category, SDL_LogPriority prio, const char* message );
// void log_err ( const std::string& identifier, const std::string& message );
// void log_trace ( const std::string& identifier );

void log ( void* userdata, int category, SDL_LogPriority prio, const char* message )
{
/*
  std::cout
  << "NOM_LOG at "
  << nom::timestamp()
  << std::endl
  << "Reason: "
  << std::string(message)
  << std::endl
  << "Priority: "
  << prio
  << std::endl
  << "Category: "
  << category
  << std::endl;
*/
  /*
  std::cout
  << identifier
  << "_LOG_ERR at "
  << nom::timestamp(),
  << "In file "
  << __FILE__
  << ":"
  << __LINE__
  << std::endl
  << "Reason: "
  << message
  << std::endl
  << std::endl;
  */
}

void log_err ( const std::string& identifier, const std::string& message )
{
  //
}

void log_trace ( const std::string& identifier )
{
  /*
  std::cout
  << identifier
  << "_LOG_TRACE at "
  << nom::timestamp(),
  << __func__
  << std::endl
  << std::endl;
  */
}

} // namespace nom
