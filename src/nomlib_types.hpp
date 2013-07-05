/******************************************************************************

    Portable fixed-size types

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_TYPES_HEADERS
#define NOMLIB_TYPES_HEADERS

#include <cstdint>

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
#if defined ( _MSC_VER ) // MSVC++
  typedef signed __int64 int64;
  typedef unsigned __int64 uint64;
#else
  typedef int64_t int64;
  typedef uint64_t uint64;
#endif

// Additional integer type definitions
typedef uint16_t ushort;
typedef unsigned long ulong;

} // namespace nom

#endif // NOMLIB_TYPES_HEADERS defined
