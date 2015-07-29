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
#ifndef NOMLIB_STDINT_TYPES_HPP
#define NOMLIB_STDINT_TYPES_HPP

#include <cstddef>  // std::size_t
#include <limits>   // min && max types

#include "nomlib/platforms.hpp"

/*
  TODO: This should be replaced by an actual CMake script -- think:
        compile-time check for the necessary feature support for C++11 style
        headers support.

        Look into why GCC doesn't like the inclusion of <cstdint> -- otherwise
        known as stdint.h. MSVCPP2013 & llvm-clang are fine with it).
*/

#ifndef NOM_PLATFORM_LINUX  // To be replace with NOM_COMPILER_FEATURE_NULLPTR
                            // (see above TODO note).
  #include <cstdint>
#else
  #include <sys/types.h>
#endif

// FIXME: The following declaration is necessary in order to avoid a very
// nasty compiling conflict that can happen under Windows anytime the
// windef.h header file is included (commonly from windows.h), due to min and
// max macros being declared there. This is why macros are evil.
//
// http://support.microsoft.com/kb/143208
// http://stackoverflow.com/questions/5004858/stdmin-gives-error
#if defined( NOM_PLATFORM_WINDOWS )
  #undef min
  #undef max
#endif

// Borrowed from /usr/include/MacTypes.h && /usr/include/objc/objc.h:
#ifndef NULL
  #if defined( NOM_COMPILER_FEATURE_NULLPTR )
    #define NULL nullptr
  #else
    #define NULL 0 //__DARWIN_NULL
  #endif
#endif // ! NULL

// Portable fixed-size data types derive from stdint.h
namespace nom {

/// \brief Signed 8-bit integer.
typedef int8_t int8;

/// \brief Unsigned 8-bit integer.
typedef uint8_t uint8;

/// \brief Signed 16-bit integer.
typedef int16_t int16;

/// \brief Unsigned 16-bit integer.
typedef uint16_t uint16;

/// \brief Signed 32-bit integer.
typedef int32_t int32;

/// \brief Unsigned 16-bit integer.
typedef uint32_t uint32;

/// \brief 32-bit IEEE floating-point value.
typedef float real32;

/// \brief 64-bit IEEE floating-point value.
typedef double real64;

/// \brief 64-bit integer types
/// \note As per **/usr/include/MacTypes.h**:
///
/// "The MS Visual C/C++ compiler uses __int64 instead of long long".
#if defined( NOM_COMPILER_MSVCPP ) && defined( NOM_PLATFORM_ARCH_X86 )
  typedef signed __int64 int64;
  typedef unsigned __int64 uint64;
#else // Blindly assume a 64-bit architecture
  typedef int64_t int64;    //typedef signed long long int int64;
  typedef uint64_t uint64;  //typedef unsigned long long int uint64;
#endif

// Additional integer type definitions

/// \brief Unsigned 8-bit character.
typedef unsigned char uchar;

/// \brief Variable-size (platform-defined) signed integer.
/// \deprecated This will be removed in a future version; use int type instead.
typedef signed int sint;

/// \brief Variable-size (platform-defined) unsigned integer.
typedef unsigned int uint;

typedef std::size_t size_type;

typedef intptr_t* int_ptr;
typedef uintptr_t* uint_ptr;

/// \deprecated This will be removed in a future version; use int_ptr type
/// instead.
typedef sint* sint_ptr;

typedef int32_t* int32_ptr;
typedef uint32_t* uint32_ptr;

typedef void* void_ptr;

typedef unsigned long ulong;

// Numerical min, max values for commonly used data types
//
// http://en.cppreference.com/w/cpp/types/numeric_limits

const char NOM_CHAR_MIN = std::numeric_limits<char>::lowest();
const char NOM_CHAR_MAX = std::numeric_limits<char>::max();
const uchar NOM_UCHAR_MIN = std::numeric_limits<uchar>::lowest();
const uchar NOM_UCHAR_MAX = std::numeric_limits<uchar>::max();

const int8 NOM_INT8_MIN = std::numeric_limits<int8>::lowest();
const int8 NOM_INT8_MAX = std::numeric_limits<int8>::max();
const uint8 NOM_UINT8_MIN = std::numeric_limits<uint8>::lowest();
const uint8 NOM_UINT8_MAX = std::numeric_limits<uint8>::max();

const int16 NOM_INT16_MIN = std::numeric_limits<int16>::lowest();
const int16 NOM_INT16_MAX = std::numeric_limits<int16>::max();
const uint16 NOM_UINT16_MIN = std::numeric_limits<uint16>::lowest();
const uint16 NOM_UINT16_MAX = std::numeric_limits<uint16>::max();

const int NOM_INT_MIN = std::numeric_limits<int>::lowest();
const int NOM_INT_MAX = std::numeric_limits<int>::max();
const uint NOM_UINT_MIN = std::numeric_limits<uint>::lowest();
const uint NOM_UINT_MAX = std::numeric_limits<uint>::max();

const int32 NOM_INT32_MIN = std::numeric_limits<int32>::lowest();
const int32 NOM_INT32_MAX = std::numeric_limits<int32>::max();
const uint32 NOM_UINT32_MIN = std::numeric_limits<uint32>::lowest();
const uint32 NOM_UINT32_MAX = std::numeric_limits<uint32>::max();

const int64 NOM_INT64_MIN = std::numeric_limits<int64>::lowest();
const int64 NOM_INT64_MAX = std::numeric_limits<int64>::max();
const uint64 NOM_UINT64_MIN = std::numeric_limits<uint64>::lowest();
const uint64 NOM_UINT64_MAX = std::numeric_limits<uint64>::max();

const size_type NOM_SIZE_TYPE_MIN = std::numeric_limits<size_type>::lowest();
const size_type NOM_SIZE_TYPE_MAX = std::numeric_limits<size_type>::max();

const real32 NOM_REAL32_MIN = std::numeric_limits<real32>::lowest();
const real32 NOM_REAL32_MAX = std::numeric_limits<real32>::max();

const real64 NOM_REAL64_MIN = std::numeric_limits<real64>::lowest();
const real64 NOM_REAL64_MAX = std::numeric_limits<real64>::max();

/// \brief An integer indicating that there is no match, an error or NULL.
static const int npos = -1;

/// \brief The default standard point size for fonts.
///
/// \remarks Used in default initialization of nom::Text, nom::UIWidget, etc.
static const int DEFAULT_FONT_SIZE = 12;

/// \brief Alignment types.
///
/// \remarks See also, nom::Anchor enumeration.
///
/// \note Borrowed from [maelstrom's screenlib](https://hg.libsdl.org/Maelstrom/)
enum Alignment: uint32
{
  NONE = 0x0,
  X_LEFT = 0x01,
  X_CENTER = 0x02,
  X_RIGHT = 0x4,

  /// \remarks Horizontal
  X_MASK = ( X_LEFT | X_CENTER | X_RIGHT ),

  Y_TOP = 0x10,
  Y_CENTER = 0x20,
  Y_BOTTOM = 0x40,

  /// \remarks Vertical
  Y_MASK = ( Y_TOP | Y_CENTER | Y_BOTTOM )
};

/// \note Borrowed from [maelstrom's screenlib](https://hg.libsdl.org/Maelstrom/)
enum Anchor: uint32
{
  None = NONE,                          // 0
  Left = X_LEFT,                        // 1
  Center = X_CENTER,                    // 2
  Right = X_RIGHT,                      // 4

  TopLeft = Y_TOP | X_LEFT,             // Hex: 0x11, Dec: 17
  TopCenter = Y_TOP | X_CENTER,         // Hex: 0x12, Dec: 18
  TopRight = Y_TOP | X_RIGHT,           // Hex: 0x14, Dec: 20

  MiddleLeft = Y_CENTER | X_LEFT,       // Hex: 0x21, Dec: 33
  MiddleCenter = Y_CENTER | X_CENTER,   // Hex: 0x22, Dec: 34
  MiddleRight = Y_CENTER | X_RIGHT,     // Hex: 0x24, Dec: 36

  BottomLeft = Y_BOTTOM | X_LEFT,       // Hex: 0x41, Dec: 65
  BottomCenter = Y_BOTTOM | X_CENTER,   // Hex: 0x42, Dec: 66
  BottomRight = Y_BOTTOM | X_RIGHT      // Hex: 0x44, Dec: 68
};

const nom::size_type NOM_BYTE = 1024;

inline
nom::size_type kilobyte(nom::size_type bytes)
{
  nom::size_type result = (bytes * NOM_BYTE);

  return result;
}

inline
nom::size_type megabyte(nom::size_type bytes)
{
  nom::size_type result = ( kilobyte(bytes) * NOM_BYTE);

  return result;
}

inline
nom::size_type gigabyte(nom::size_type bytes)
{
  nom::size_type result = ( megabyte(bytes) * NOM_BYTE );

  return result;
}

inline
nom::size_type terabyte(nom::size_type bytes)
{
  nom::size_type result = ( gigabyte(bytes) * NOM_BYTE );

  return result;
}

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

static_assert ( sizeof ( nom::real32 ) == 4, "nom::real32" );
static_assert ( sizeof ( nom::real64 ) == 8, "nom::real64" );

static_assert ( sizeof ( nom::uchar ) == 1, "nom::uchar" );

// Blindly assumes we are on either a 64-bit or 32-bit platform.
// TODO: Relocate this def to run-time (cmake gen)
#if defined(NOM_PLATFORM_ARCH_X86_64)
  static_assert( sizeof ( nom::ulong ) == 8, "nom::ulong" );
  static_assert( sizeof ( nom::size_type ) == 8, "nom::size_type" );
#else // #elif defined( NOM_PLATFORM_ARCH_X86_86 )
  static_assert( sizeof ( nom::ulong ) == 4, "nom::ulong" );
  static_assert( sizeof ( nom::size_type ) == 4, "nom::size_type" );
#endif

// Blindly assumes we are on either a 64-bit or 32-bit platform.
#if defined(NOM_PLATFORM_ARCH_X86_64)
  static_assert( sizeof(nom::int_ptr) == 8, "nom::int_ptr" );
  static_assert( sizeof(nom::uint_ptr) == 8, "nom::uint_ptr" );
  static_assert( sizeof ( nom::int32_ptr ) == ( sizeof(long) ), "nom::int32_ptr" );
  static_assert( sizeof ( nom::uint32_ptr ) == ( sizeof(nom::ulong) ), "nom::uint32_ptr" );
#else // #elif defined(NOM_PLATFORM_ARCH_X86)
  static_assert( sizeof(nom::int_ptr) == 4, "nom::int_ptr" );
  static_assert( sizeof(nom::uint_ptr) == 4, "nom::uint_ptr" );
  static_assert( sizeof( nom::int32_ptr ) == ( sizeof( long ) ), "nom::int32_ptr" );
  static_assert( sizeof( nom::uint32_ptr ) == ( sizeof( nom::ulong ) ), "nom::uint32_ptr" );
#endif

/// Additional type definitions

const nom::sint NOM_EXIT_FAILURE = 1; // EXIT_FAILURE from cstdlib headers
const nom::sint NOM_EXIT_SUCCESS = 0; // EXIT_SUCCESS from cstdlib headers

//#if defined(HAVE_SDL2)
const nom::sint SDL_SUCCESS = 0; // Non-error return value for SDL2 API
//#endif

#define NOM_KILOBYTES(bytes) kilobyte(bytes)
#define NOM_MEGABYTES(bytes) megabyte(bytes)
#define NOM_GIGABYTES(bytes) gigabyte(bytes)
#define NOM_TERABYTES(bytes) terabytes(bytes)

// Configuration variables for the engine

#define NOM_EVENT_QUEUE_STATISTICS "NOM_EVENT_QUEUE_STATISTICS"

#endif // include guard defined
