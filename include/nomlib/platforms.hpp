/******************************************************************************

    Platform detection

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_PLATFORMS_HEADERS
#define NOMLIB_PLATFORMS_HEADERS

/// \brief Identification the platform (operating system)
///
/// I have platform detection separated from nomlib/config.hpp specifically so
/// that one may include this header file for use in an external project without
/// worry of polluting the namespace with any unnecessary definitions.

#if defined ( _WIN32) || defined ( __WIN32__ )
  #define NOM_PLATFORM_WINDOWS
#elif defined ( linux ) || defined ( __linux )
  #define NOM_PLATFORM_LINUX
#elif defined ( __APPLE__ ) || defined ( MACOSX ) || defined ( macintosh ) || defined ( Macintosh )
  #define NOM_PLATFORM_OSX
#endif

// Function names and preferably also its type signature
#if defined ( _MSC_VER ) // MSVC++
  // TODO: Presumably the same as GNU's __PRETTY_FUNCTION__ ?
  //
  // SOURCE: http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx
  #define __func__ __FUNCSIG__
#else // We assume GNU v2+

  // The type signature is nice because this shows if the function calling type
  // is a virtual or not and even what arguments the function has
  #define __func__ __PRETTY_FUNCTION__
#endif

#endif // include guard
