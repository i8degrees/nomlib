/******************************************************************************

    Portable fixed-size types

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_TYPES_HEADERS
#define NOMLIB_TYPES_HEADERS

#include <cstdint>

// 64-bit kernel
#define NOMLIB_64BIT_TYPE

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
#if defined NOMLIB_64BIT_TYPE
  #if defined ( _MSC_VER ) // MSVC++
    typedef signed __int64 int64;
    typedef unsigned __int64 uint64;
  #else
    typedef int64_t int64;
    typedef uint64_t uint64;
  #endif
#endif

// Additional integer type definitions
typedef uint16_t ushort;
typedef unsigned long ulong;

} // namespace nom

// Thanks to SDL for the macro! :-)
// Ensure our defined data types have the right sizes
#define NOMLIB_COMPILE_TIME_ASSERT(name, x) \
  typedef int NOMLIB_dummy_ ## name[(x) * 2 - 1]

NOMLIB_COMPILE_TIME_ASSERT(bit, sizeof(nom::bit) == 1);
NOMLIB_COMPILE_TIME_ASSERT(uint8, sizeof(nom::uint8) == 1);
NOMLIB_COMPILE_TIME_ASSERT(int8, sizeof(nom::int8) == 1);
NOMLIB_COMPILE_TIME_ASSERT(uint16, sizeof(nom::uint16) == 2);
NOMLIB_COMPILE_TIME_ASSERT(int16, sizeof(nom::int16) == 2);
NOMLIB_COMPILE_TIME_ASSERT(uint32, sizeof(nom::uint32) == 4);
NOMLIB_COMPILE_TIME_ASSERT(int32, sizeof(nom::int32) == 4);
NOMLIB_COMPILE_TIME_ASSERT(ushort, sizeof(nom::ushort) == 2);

#if defined NOMLIB_64BIT_TYPE
  NOMLIB_COMPILE_TIME_ASSERT(uint64, sizeof(nom::uint64) == 8);
  NOMLIB_COMPILE_TIME_ASSERT(int64, sizeof(nom::int64) == 8);
  NOMLIB_COMPILE_TIME_ASSERT(ulong, sizeof(nom::ulong) == 8);
#else
  NOMLIB_COMPILE_TIME_ASSERT(ulong, sizeof(nom::ulong) == 4);
#endif

#endif // NOMLIB_TYPES_HEADERS defined
