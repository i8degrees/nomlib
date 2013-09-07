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

// Portable fixed-size data types derive from stdint.h
namespace nom {

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
typedef signed long long int int64;
typedef unsigned long long int uint64;

// Additional integer type definitions
#if defined ( NOM_PLATFORM_ARCH_X86_64 )
  typedef unsigned long ulong;
#else // Blindly assume 32-bit arch
  typedef long long ulong;
#endif

typedef unsigned char uchar;

typedef void* Pixels;


} // namespace nom

/// Ensure our data types have the right sizes using C++11 compile-time asserts.
static_assert ( sizeof ( nom::uint8 ) == 1, "nom::uint8" );
static_assert ( sizeof ( nom::int8 ) == 1, "nom::int8" );

static_assert ( sizeof ( nom::uint16 ) == 2, "nom::uint16" );
static_assert ( sizeof ( nom::int16 ) == 2, "nom::int16" );

static_assert ( sizeof ( nom::uint32 ) == 4, "nom::uint32" );
static_assert ( sizeof ( nom::int32 ) == 4, "nom::int32" );

static_assert ( sizeof ( nom::uint64 ) == 8, "nom::uint64" );
static_assert ( sizeof ( nom::int64 ) == 8, "nom::int64" );

static_assert ( sizeof ( nom::ulong ) == 8, "nom::ulong" );

/// Something is *seriously* wrong if this fails!
static_assert ( sizeof ( nom::uchar ) == 1, "nom::uchar" );

/// \todo FIXME
//#if defined ( NOM_PLATFORM_ARCH_X86_64 )
  //static_assert ( sizeof ( nom::Pixels ) == 8, "nom::Pixels" );
//#else
  //static_assert ( sizeof ( nom::Pixels ) == 4, "nom::Pixels" );
//#endif

#endif // NOMLIB_TYPES_HEADERS defined
