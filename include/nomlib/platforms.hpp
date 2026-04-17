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
#ifndef NOMLIB_SUPPORTED_PLATFORMS_HPP
#define NOMLIB_SUPPORTED_PLATFORMS_HPP

/// \brief Identification the platform (operating system)
///
/// I have platform detection separated from nomlib/config.hpp specifically so
/// that one may include this header file for use in an external project without
/// worry of polluting the namespace with any unnecessary definitions.

#if defined(_WIN32) || defined(__WIN32__)
  #define NOM_PLATFORM_WINDOWS
// NOTE: The following two macros -- linux and __linux -- are obsolete and are
// not POSIX compliant!
#elif defined(__linux__) || defined(linux) || defined(__linux)
  #define NOM_PLATFORM_LINUX
  #define NOM_PLATFORM_POSIX
#elif defined(__APPLE__) || defined(__MAC__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
  #define NOM_PLATFORM_OSX
  #define NOM_PLATFORM_POSIX // Assume POSIX-compliant Unix
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
      defined(__bsdi__) || defined(__DragonFly__)
  #define NOM_PLATFORM_BSD
  #define NOM_PLATFORM_POSIX
#endif

/// Platform architecture detection; we only check for a 32-bit or 64-bit
/// environment at the moment as this is all that we can test on.
///
/// \remarks The second set of arch checks are for when compiling on Windows;
/// it has only been tested with MSVCPP 2013 RC1.
#if defined( __i386__ ) || defined( _M_IX86 )
  #define NOM_PLATFORM_ARCH_X86
#elif defined( __x86_64__ ) || defined( _M_AMD64 )
  #define NOM_PLATFORM_ARCH_X86_64
#endif

/// \brief Compiler detection
///
/// \todo Add conditional if for GNU GCC
#if defined(_MSC_VER) // Microsoft Visual C++
  #define NOM_COMPILER_MSVCPP
  #define NOM_COMPILER_FEATURE_NULLPTR // See include/nomlib/types.hpp
#else // Assume llvm-clang
  #define NOM_COMPILER_CLANG
  #define NOM_COMPILER_FEATURE_NULLPTR // See include/nomlib/types.hpp
#endif

/// \brief Function names and preferably also its type signature
#if defined ( NOM_COMPILER_MSVCPP ) // MSVC++
  /// \see http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx
  #define NOM_FUNC __FUNCTION__
#else // We assume clang or GNU v2+
  #define NOM_FUNC __PRETTY_FUNCTION__
#endif

#ifdef PATH_MAX
  #undef PATH_MAX
#endif

/// \todo: Apparently determining a safe, portable maximum file path is a bit
/// trickier
/// than meets the eye -- for the time being, I am going to assume the POSIX
/// standard of 256 bytes is sufficient for my needs.
///
/// Linux: #include <linux/limits.h>
/// OSX: #include <sys/syslimits.h>
/// Windows: #include <windows.h> ???
///
/// See also:
///
/// 1. http://stackoverflow.com/questions/833291/is-there-an-equivalent-to-winapis-max-path-under-linux-unix
/// 2.
#define PATH_MAX 256

// Cross-platform macro for obtaining the CPU's Time Stamp Counter (RDTSC)
//
//    See also
// 1. http://handmadehero.com
// 2. http://www.strchr.com/performance_measurements_with_rdtsc
#if defined(NOM_PLATFORM_WINDOWS) && defined(NOM_COMPILER_MSVCPP)
  // Use built-in compiler intrinsic
  #define NOM_RDTSC() __rdtsc();
#else // Assume clang compiler
  #define NOM_RDTSC() nom::rdtsc()
#endif // MS Windows && MSVCPP

namespace nom {

enum Platform
{
  PLATFORM_UNKNOWN = 0,   /// Undetected
  PLATFORM_WINDOWS,       /// Microsoft
  PLATFORM_LINUX,         /// GNU
  PLATFORM_BSD,           /// FreeBSD, NetBSD, OpenBSD, DragonFly, ...
  PLATFORM_OSX,           /// Apple
  NUM_PLATFORMS,          /// Total number of platforms
};

struct PlatformSpec
{
  /// \brief The platform name.
  const char* name = "Unknown";

  /// \brief The platform's environment.
  Platform env = PLATFORM_UNKNOWN;

  /// \brief The number of available CPUs.
  int num_cpus = 1;

  /// \brief The L1 cache size of the CPU (in bytes).
  int cpu_cache_size = 0;

  /// \brief The total available system RAM (in bytes).
  int total_ram = 0;
};

PlatformSpec platform_info();

} // namespace nom

#endif // include guard
