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
#include "nomlib/system/clock.hpp"

namespace nom {

const std::string time( void )
{
  time_t timer;

  // Broken: Fix MSVCPP compile warning that suggests using ctime_s; the
  // "secure" variant of MS time functions.
  #if defined( NOM_PLATFORM_WINDOWS )
    char current_time[TIME_STRING_SIZE] = "\0"; // null-terminated
    errno_t err;
    timer = std::time( NULL );
    err = ctime_s( current_time, TIME_STRING_SIZE , &timer );

    // Return a null-terminated string on err
    if ( err != 0 )
    {
      // Error is likely due to incomplete implementation --
      // are we compiling on an unsupported platform?
      NOM_LOG_ERR( NOM, "Could not obtain current time & date" );
      return std::string( current_time );
      //return std::string("\0");
    }
    else
    {
      return std::string( current_time );
    }
  #else // Assume POSIX Unix variant
    timer = std::time( NULL );
    return ctime( &timer );
  #endif
}

uint32 ticks( void )
{
  return SDL_GetTicks();
}

const std::string ticks_as_string( void )
{
  return std::to_string( SDL_GetTicks() );
}

void sleep( uint32 milliseconds )
{
  SDL_Delay ( std::max ( milliseconds, static_cast<uint32> ( 10 ) ) );
}

} // namespace nom
