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
#ifndef NOMLIB_TYPES_HEADERS
#define NOMLIB_TYPES_HEADERS

#include <cstdint>

// Portable fixed-size data types

#ifdef PATH_MAX
  #undef PATH_MAX
#endif

// Apparently determining a safe, portable maximum file path is a bit trickier
// than meets the eye -- for the time being, I am going to assume the POSIX
// standard of 256 bytes is sufficient for my needs. I have my doubts as to this
// working out well for us if nomlib ever sees the light of day on the Windows
// platform, but alas!
//
// See http://stackoverflow.com/questions/833291/is-there-an-equivalent-to-winapis-max-path-under-linux-unix
#define PATH_MAX 256

// 64-bit kernel
#define NOMLIB_64BIT

// Derives from stdint.h
namespace nom {

// 1-bit integer types
typedef bool bit;

// 8-bit integer types
typedef int8_t int8;
typedef uint8_t uint8;

// 16-bit integer types
typedef int16_t int16;
typedef uint16_t uint16;

// 32-bit integer types
typedef int32_t int32;
typedef uint32_t uint32;

// 64-bit integer types
#if defined NOMLIB_64BIT

  #if defined ( _MSC_VER ) // MSVC++
    typedef signed __int64 int64;
    typedef unsigned __int64 uint64;
  #else
    typedef int64_t int64;
    typedef uint64_t uint64;
  #endif

#else
  // We are compiling on a 32-bit system (we may not have int64 types defined)
  typedef signed long long int64;
  typedef unsigned long long uint64;

#endif // NOMLIB_64BIT defined

// Additional integer type definitions
typedef unsigned long ulong;

} // namespace nom

// Ensure our defined data types have the right sizes
// Thanks to SDL for the macro! :-)
#define NOMLIB_COMPILE_TIME_ASSERT(name, x) \
  typedef int NOMLIB_dummy_ ## name[(x) * 2 - 1]

NOMLIB_COMPILE_TIME_ASSERT(bit, sizeof(nom::bit) == 1);
NOMLIB_COMPILE_TIME_ASSERT(uint8, sizeof(nom::uint8) == 1);
NOMLIB_COMPILE_TIME_ASSERT(int8, sizeof(nom::int8) == 1);
NOMLIB_COMPILE_TIME_ASSERT(uint16, sizeof(nom::uint16) == 2);
NOMLIB_COMPILE_TIME_ASSERT(int16, sizeof(nom::int16) == 2);
NOMLIB_COMPILE_TIME_ASSERT(uint32, sizeof(nom::uint32) == 4);
NOMLIB_COMPILE_TIME_ASSERT(int32, sizeof(nom::int32) == 4);
NOMLIB_COMPILE_TIME_ASSERT(uint64, sizeof(nom::uint64) == 8);
NOMLIB_COMPILE_TIME_ASSERT(int64, sizeof(nom::int64) == 8);

#endif // NOMLIB_TYPES_HEADERS defined
