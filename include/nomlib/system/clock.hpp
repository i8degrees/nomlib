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
#ifndef NOMLIB_SYSTEM_CLOCK_HPP
#define NOMLIB_SYSTEM_CLOCK_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>

#include "SDL.h" // Used for ticks & sleep implementations

#include "nomlib/config.hpp"

namespace nom {

/// \brief Buffer input size; used in Windows-dependent time implementation --
/// see **system/clock.cpp**. Although this is only used in the Windows code,
/// I expect the buffer size to be the same ( ~1 +/- ) across all supported
/// platforms.
const nom::size TIME_STRING_SIZE = 26;

/// \brief Get the current date & time.
///
/// \returns On success, a time string with the following formatting:
///
/// \code
/// Wed Jan 02 02:03:55 1980\n\0
/// \endcode
///
/// On error: null-terminated (empty) string.
///
/// \remarks  The string is expected to be null-terminated with a newline
///           carriage.
///
/// \todo Move Windows OS supporting code from **system/clock.cpp** to
/// **system/windows.cpp**.
const std::string time( void );

/// SDL helper function
///
/// Wrapper for SDL_GetTicks.
uint32 ticks( void );
const std::string ticks_as_string( void );

/// SDL_Delay wrapper
///
/// Values below 10 milliseconds are clamped to 10
void sleep( uint32 milliseconds );

} // namespace nom

#endif // include guard defined
