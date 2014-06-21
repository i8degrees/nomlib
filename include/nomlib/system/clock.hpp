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
#include <ctime>
#include <locale>

#include <algorithm>  // std::max

#include "nomlib/config.hpp"

namespace nom {

const nom::size TIME_STRING_SIZE = 26;

/// \brief Get the current date & time.
///
/// \returns A std::string containing the formatted date and time stamp on
/// success, or a null-terminated string on err.
///
/// \param format Conversion specifiers that are compatible with std::strftime.
///
/// \note This method is platform-specific.
///
/// \see http://en.cppreference.com/w/cpp/chrono/c/strftime
/// \see http://msdn.microsoft.com/en-us/library/fe06s4ak(v=vs.71).aspx
const std::string time( const std::string& format );

/// \brief Get the current date & time (logger friendly).
///
/// \returns A std::string containing the ISO 8601 date and time on success, or
/// a null-terminated string on err.
///
/// \code
/// 2014/06/02 02:52:42
/// \endcode
const std::string time( void );

/// \brief Get the current date and time (file name friendly).
///
/// \returns A std::string containing the ISO 8601 date and time on success, or
/// a null-terminated string on err.
///
/// \code
/// 2014-06-19_13-25-21
/// \endcode
std::string timestamp( void );

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
