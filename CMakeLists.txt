cmake_minimum_required( VERSION 2.8.12 )

# Enable the use of MACOSX_RPATH by default; this effectively allows plug 'n'
# play functionality, so to speak -- the resulting shared library files can
# simply be copied over into the end-user's application bundle or framework
# bundle. No mucking around with install_name_tool.
#
# (Minimum required CMake: v2.8.12)
#
#   See also:
#
# cmake --help-policy cmp0042
# http://www.kitware.com/blog/home/post/510
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif(POLICY CMP0042)

# CMake Environment

# Opt out of using CMake v3.0 PROJECT_VERSION variables management for the
# project.
# http://www.cmake.org/cmake/help/v3.0/command/project.html#command:project
if( POLICY CMP0048 )
  cmake_policy( SET CMP0048 OLD )
endif( POLICY CMP0048 )

# set ( CMAKE_VERBOSE_MAKEFILE OFF CACHE PATH "Verbose Makefile" )
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" )
set ( CMAKE_TEMPLATE_PATH ${CMAKE_TEMPLATE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates" )

set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 11 )
set ( PROJECT_VERSION_PATCH 0 )

# Project Options
option ( DEBUG "Build with all debugging features" off )
option ( DEBUG_ASSERT "Build with run-time assertions enabled" off )
option ( EXAMPLES "Build nomlib usage examples" off )
option( NOM_BUILD_TESTS "Build unit tests" off )
set( NOM_INSTALL_GENERATED_DOCS off )

option( NOM_BUILD_CORE_UNIT "Engine core" ON )
option( NOM_BUILD_MATH_UNIT "Math utilities" ON )
option( NOM_BUILD_FILE_UNIT "Filesystem access" ON )
option( NOM_BUILD_AUDIO_UNIT "Audio subsystem" ON )
option( NOM_BUILD_GRAPHICS_UNIT "Graphics core" ON )
option( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT "Build with scale2x & hqx algorithms" ON )
option( NOM_BUILD_SYSTEM_UNIT "System unit" ON )
option( NOM_BUILD_PTREE_UNIT "Generic container for serialization" ON )
option( NOM_BUILD_SERIALIZERS_UNIT "Serialization for JSON, XML, HTML" ON )
option( NOM_BUILD_GUI_UNIT "GUI subsystem" ON )

project( nomlib ) # Sets PROJECT_NAME variable for us

# Platform detection
include ( "${CMAKE_SOURCE_DIR}/cmake/platform.cmake" )

if ( PLATFORM_WINDOWS )
  # Building nomlib on Windows is only supported as a static library for the
  # moment
  set ( BUILD_SHARED_LIBS off )
else ( NOT PLATFORM_WINDOWS )
  # We only support building nomlib as a dynamic library at the moment
  set ( BUILD_SHARED_LIBS on )
endif ( PLATFORM_WINDOWS )

if ( DEBUG )
  # set ( CMAKE_VERBOSE_MAKEFILE ON )
  set ( CMAKE_BUILD_TYPE "Debug" )

  if ( NOT PLATFORM_WINDOWS )
    # TODO: Enable -Wextra
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D NOM_DEBUG -Wall -Wunused -O0" ) #-D_GLIBCXX_DEBUG

    # Measure the time it takes per source file to build, so we can try
    # speeding up slow builds!
    # set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ftime-report" )
  else ( PLATFORM_WINDOWS )
    # TODO: split these options up like we have for other platforms
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D NOM_DEBUG /D NOM_DEBUG_ASSERT" )
  endif( NOT PLATFORM_WINDOWS )

  message ( STATUS "Building ${PROJECT_NAME} with debugging." )
  message ( STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}" )
else () # Build with optimizations for maximum speed and minimal size
  set ( CMAKE_BUILD_TYPE "Release" )
  message ( STATUS "Building ${PROJECT_NAME} with high speed, low drag!" )
  message ( STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}" )
endif ( DEBUG )

if ( DEBUG_ASSERT )
  add_definitions ( "-DNOM_DEBUG_ASSERT" ) # Enable NOM_ASSERT macros
  message ( STATUS "Run-time assertions are ON." )
else ( NOT DEBUG_ASSERT )
  add_definitions ( "-DNDEBUG" ) # Disable NOM_ASSERT macros
  message ( STATUS "Run-time assertions are OFF." )
endif ( DEBUG_ASSERT )

if( BUILD_SHARED_LIBS )
  set( LIBRARY_OUTPUT_TYPE "SHARED" )
else( NOT BUILD_SHARED_LIBS )
  set( LIBRARY_OUTPUT_TYPE "STATIC" )
endif( BUILD_SHARED_LIBS )

# Utility macro helpers
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros.cmake" )

# TODO: Rename to NOMLIB_SRC_DIR..?
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src" )
set( NOMLIB_RESOURCES_DIR "${PROJECT_SOURCE_DIR}/Resources" )

# TODO: Rename to NOMLIB_INCLUDE_ROOT_DIR..?
set( INC_ROOT_DIR "${PROJECT_SOURCE_DIR}/include" )

# TODO: Rename to NOMLIB_INCLUDE_DIR..?
set( INC_DIR "${INC_ROOT_DIR}/nomlib" )

set( NOMLIB_DEPS_DIR "${PROJECT_SOURCE_DIR}/third-party" )

# Relative path from project root to nomlib demonstration examples
set ( EXAMPLES_SRC_DIR "${PROJECT_SOURCE_DIR}/examples" )

# Resources path for engine examples
set( NOM_EXAMPLES_RESOURCES_DIR "${NOMLIB_RESOURCES_DIR}/examples" )

# Unit testing resources path
set( NOM_TESTS_RESOURCES_DIR "${NOMLIB_RESOURCES_DIR}/tests" )

# Additional flags to pass add_executable -- used when library examples are
# built
set ( EXECUTABLE_FLAGS "" )

# Files used with documentation generation
set ( PROJECT_DOXYGEN_DIR "${NOMLIB_RESOURCES_DIR}/doxygen" )

# CMAKE_SYSTEM_PREFIX_PATH is searched to find libraries when the find_package
# command is used.
message ( STATUS "Library Search Prefix: ${CMAKE_SYSTEM_PREFIX_PATH}" )

# Installation prefix path set for our project
message ( STATUS "Installation Prefix: ${CMAKE_INSTALL_PREFIX}" )

# Doxygen configuration
include ( "${PROJECT_SOURCE_DIR}/cmake/doxygen.cmake" )

# Third-party dependencies

if ( PLATFORM_WINDOWS )
  if(NOT DEFINED ENV{SDL2DIR})
    set(ENV{SDL2DIR} "${NOMLIB_DEPS_DIR}/windows/SDL2")
  endif(NOT DEFINED ENV{SDL2DIR})

  if(NOT DEFINED ENV{SDL2IMAGEDIR})
    set(ENV{SDL2IMAGEDIR} "${NOMLIB_DEPS_DIR}/windows/SDL2_image")
  endif(NOT DEFINED ENV{SDL2IMAGEDIR})

  if(NOT DEFINED ENV{SDL2TTFDIR})
    set(ENV{SDL2TTFDIR} "${NOMLIB_DEPS_DIR}/windows/SDL2_ttf")
  endif(NOT DEFINED ENV{SDL2TTFDIR})

  if(NOT DEFINED ENV{OPENALDIR})
    set(ENV{OPENALDIR} "${NOMLIB_DEPS_DIR}/windows/OpenAL")
  endif(NOT DEFINED ENV{OPENALDIR})

  if(NOT DEFINED ENV{LIBSNDFILEDIR})
    set(ENV{LIBSNDFILEDIR} "${NOMLIB_DEPS_DIR}/windows/libsndfile")
  endif(NOT DEFINED ENV{LIBSNDFILEDIR})

  if(NOT DEFINED ENV{LIBROCKETDIR})
    set(ENV{LIBROCKETDIR} "${NOMLIB_DEPS_DIR}/windows/libRocket")
  endif(NOT DEFINED ENV{LIBROCKETDIR})

  # Redistribution paths must be absolute, a directory and end with a trailing
  # slash character (these are fed to CMake's install command). Only filenames
  # with the extension of '.dll' and '.lib' (case-insensitive) will be copied.

  set( SDL2_REDIST_DIR "${NOMLIB_DEPS_DIR}/windows/SDL2/lib/${PLATFORM_ARCH}/" )
  set( SDL2_IMAGE_REDIST_DIR "${NOMLIB_DEPS_DIR}/windows/SDL2_image/lib/${PLATFORM_ARCH}/" )
  set( SDL2_TTF_REDIST_DIR "${NOMLIB_DEPS_DIR}/windows/SDL2_ttf/lib/${PLATFORM_ARCH}/" )

  # The CMake-distributed FindOpenAL.cmake doesn't recognize lib/x86, lib/x64,
  # so in order to avoid creating our own, we break from our norm and conform
  # to libs/Win32, libs/Win64
  if( PLATFORM_ARCH STREQUAL "x86" )
    set( OPENAL_REDIST_DIR "${NOMLIB_DEPS_DIR}/windows/OpenAL/libs/Win32/" )
  elseif( PLATFORM_ARCH STREQUAL "x64" )
    set( OPENAL_REDIST_DIR "${NOMLIB_DEPS_DIR}/windows/OpenAL/libs/Win64/" )
  endif( PLATFORM_ARCH STREQUAL "x86" )

  set( LIBSNDFILE_REDIST_DIR "${NOMLIB_DEPS_DIR}/windows/libsndfile/lib/${PLATFORM_ARCH}/" )

  # FreeType2
  set( LIBROCKET_EXT_REDIST_DIR "${NOMLIB_DEPS_DIR}/windows/libRocket/bin/${PLATFORM_ARCH}/" )

  set( LIBROCKET_REDIST_DIR "${NOMLIB_DEPS_DIR}/windows/libRocket/lib/${PLATFORM_ARCH}/" )

  set( MSVCPP_REDIST_DIR "${NOMLIB_DEPS_DIR}/windows/msvcpp2013/${PLATFORM_ARCH}/" )

elseif ( PLATFORM_OSX )

  if(NOT DEFINED ENV{SDL2DIR})
    set(ENV{SDL2DIR} "${NOMLIB_DEPS_DIR}/osx")
  endif(NOT DEFINED ENV{SDL2DIR})

  if(NOT DEFINED ENV{SDL2IMAGEDIR})
    set(ENV{SDL2IMAGEDIR} "${NOMLIB_DEPS_DIR}/osx")
  endif(NOT DEFINED ENV{SDL2IMAGEDIR})

  if(NOT DEFINED ENV{SDL2TTFDIR})
    set(ENV{SDL2TTFDIR} "${NOMLIB_DEPS_DIR}/osx")
  endif(NOT DEFINED ENV{SDL2TTFDIR})

  if(NOT DEFINED ENV{OPENALDIR})
    # Use platform-distributed headers for OpenAL; should automatically use the
    # proper platform SDK version as configured by CMAKE_OSX_SYSROOT
    set(ENV{OPENALDIR} "/System/Library/Frameworks/OpenAL.framework")
  endif(NOT DEFINED ENV{OPENALDIR})

  if(NOT DEFINED ENV{LIBSNDFILEDIR})
    set(ENV{LIBSNDFILEDIR} "${NOMLIB_DEPS_DIR}/osx")
  endif(NOT DEFINED ENV{LIBSNDFILEDIR})

  if(NOT DEFINED ENV{LIBROCKETDIR})
    set(ENV{LIBROCKETDIR} "${NOMLIB_DEPS_DIR}/osx/librocket")
  endif(NOT DEFINED ENV{LIBROCKETDIR})

  # FIXME: This file needs its RPATH modified for redistribution; it is **not**
  # being used as the run-time library for executables yet. We are using brew's
  # version installed under /usr/local
  set(  LIBROCKET_EXT_REDIST_FILES
        "${NOMLIB_DEPS_DIR}/osx/librocket/bin/libfreetype.6.dylib" )

elseif ( PLATFORM_LINUX )
  # TODO
endif ( PLATFORM_WINDOWS )

# List of our source code to be compiled
add_subdirectory( ${SRC_DIR} )

# Installation phase

# Bundle libraries we depend on in the appropriate modules
if( PLATFORM_OSX AND FRAMEWORK )

  if( NOM_BUILD_CORE_UNIT )
    install(  DIRECTORY ${SDL2_LIBRARY}
              DESTINATION "${PROJECT_NAME}-core.framework/Frameworks"
              PATTERN ".*" EXCLUDE
    )
  endif( NOM_BUILD_CORE_UNIT )

  if( NOM_BUILD_SYSTEM_UNIT )
    install(  DIRECTORY ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY}
              DESTINATION "${PROJECT_NAME}-system.framework/Frameworks"
              PATTERN ".*" EXCLUDE
    )
  endif( NOM_BUILD_SYSTEM_UNIT )

  if( NOM_BUILD_AUDIO_UNIT )
    install(  DIRECTORY ${LIBSNDFILE_LIBRARY}
              DESTINATION "${PROJECT_NAME}-audio.framework/Frameworks"
              PATTERN ".*" EXCLUDE
    )
  endif( NOM_BUILD_AUDIO_UNIT )

  if( NOM_BUILD_GUI_UNIT )
    # dylibs
    install(  FILES ${LIBROCKET_CORE_LIBRARY} ${LIBROCKET_CONTROLS_LIBRARY}
              ${LIBROCKET_DEBUGGER_LIBRARY}
              DESTINATION "${PROJECT_NAME}-gui.framework/Libraries"
    )
  endif( NOM_BUILD_GUI_UNIT )

  # Install nomlib's header files in a stub framework, nomlib.framework
  #
  # NOTE: The trailing slash is necessary, to prevent the last directory
  # component -- 'nomlib' -- from being copied. This keeps the nomlib root
  # namespace preserved, so we can include headers the same way across
  # platforms, i.e.: <nomlib/config.hpp>.
  install(  DIRECTORY "${INC_DIR}/"
            DESTINATION "${PROJECT_NAME}.framework/Headers"
            PATTERN ".*" EXCLUDE
  )

  # nomlib's icon
  install(  FILES
            "${NOMLIB_RESOURCES_DIR}/nomlib.icns"
            DESTINATION "${PROJECT_NAME}.framework/Resources"
            OPTIONAL
  )

  # Install nomlib's CMake find module; this should end up in a spot that is
  # automatically searched by find_package.
  install(  DIRECTORY
            "${NOMLIB_RESOURCES_DIR}/CMake"
            DESTINATION "${PROJECT_NAME}.framework/Resources"
            PATTERN ".*" EXCLUDE
  )

  # Install software license & general project information
  install(  FILES
            ${PROJECT_SOURCE_DIR}/README.md
            ${PROJECT_SOURCE_DIR}/LICENSE.md
            DESTINATION "${PROJECT_NAME}.framework/Resources"
  )

  # Install the generated documentation files
  # if( NOM_INSTALL_GENERATED_DOCS )
  #   install(  DIRECTORY ${PROJECT_BINARY_DIR}/docs/html
  #             DESTINATION "${PROJECT_NAME}.framework/Resources/Documentation"
  #             PATTERN ".*" EXCLUDE
  #   )
  # endif( NOM_INSTALL_GENERATED_DOCS )

# POSIX install layout scheme is the default installation scheme. This is
# applicable to OS X when FRAMEWORK=off.
elseif( NOM_PLATFORM_POSIX OR PLATFORM_WINDOWS )

  # Install nomlib's header files
  install(  DIRECTORY ${INC_DIR}
            DESTINATION "include" # i.e.: /usr/local/include/nomlib/
            PATTERN ".*" EXCLUDE
  )

  # Install nomlib's icon
  install(  FILES
            "${NOMLIB_RESOURCES_DIR}/nomlib.icns"
            DESTINATION "share/${PROJECT_NAME}"
            OPTIONAL
  )

  # Install nomlib's cmake module; this should be installed somewhere that is
  # automatically searched by find_package.
  #
  # See also (installation prefixes for a package):
  # http://www.cmake.org/cmake/help/v3.0/command/find_package.html?highlight=find_package.
  if( PLATFORM_WINDOWS )
    # i.e.: C:\Program Files (x86)\nomlib\CMake
    install(  DIRECTORY
              "${NOMLIB_RESOURCES_DIR}/CMake/"
              DESTINATION
              "CMake"
              PATTERN ".*" EXCLUDE
            )
  else( NOT PLATFORM_WINDOWS )
    # i.e.: /usr/local/share/nomlib/CMake
    install(  DIRECTORY
              "${NOMLIB_RESOURCES_DIR}/CMake"
              DESTINATION
              "share/${PROJECT_NAME}/CMake"
              PATTERN ".*" EXCLUDE
            )
  endif( PLATFORM_WINDOWS )

  # Install software license & general project information
  install(  FILES
            ${PROJECT_SOURCE_DIR}/README.md
            ${PROJECT_SOURCE_DIR}/LICENSE.md
            DESTINATION "share/doc/${PROJECT_NAME}"
          )

  # Install the generated documentation files
  if( NOM_INSTALL_GENERATED_DOCS )
    install(  DIRECTORY ${PROJECT_BINARY_DIR}/docs/html
              DESTINATION "share/doc/${PROJECT_NAME}"
              PATTERN ".*" EXCLUDE
    )
  endif( NOM_INSTALL_GENERATED_DOCS )

endif()

if ( PLATFORM_LINUX )
  install(CODE "MESSAGE(\"Post-install: sudo ldconfig.\")") # FIXME (prettify)
endif ( PLATFORM_LINUX )

# nomlib examples configuration
if( EXAMPLES )
  add_subdirectory("examples")
endif( EXAMPLES )

# NOTE: we *MUST* enable testing from the top-level build script -- that's this
# file -- or else 'make test' breaks. (Dated 2014-04-05)
if( NOM_BUILD_TESTS )
  enable_testing()
  add_subdirectory("tests")
endif( NOM_BUILD_TESTS )

# Make a version file containing the current version from git
#
# Source: http://brianmilco.blogspot.com/2012/11/cmake-automatically-use-git-tags-as.html
#
# NOTE: This has been removed from the project due to wrong version output from
# `git describe` when using our git branching strategy -- also known as Vincent
# Driessen’s "Git branching model".
#
# In short, the problem is introduced during the "Finishing a release branch"
# section of the model, when we create our release tag. The algorithm
# `git describe` uses to determine the "nearest" tag is, I believe, the root
# cause of the problem. It's especially worth mentioning that the problem only
# occurs on non-HEAD branches -- i.e.: development branches, such as dev or
# feature/<name>.
#
# Full problem description: http://www.xerxesb.com/2010/git-describe-and-the-tale-of-the-wrong-commits/
#
# include( GetGitRevisionDescription )
# git_describe( GIT_REVISION )
# string( REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${GIT_REVISION}" )

# Auto-generate a build revision file containing the last recorded git revision
# number (top of branch).
include(GetGitRevisionNumber)
# git_rev_number(GIT_REVISION) # Full SHA
git_rev_number(GIT_REVISION "--short")
set(GIT_REVISION "${GIT_REVISION}")

configure_file( ${SRC_DIR}/revision.cpp.in ${SRC_DIR}/revision.cpp )

# Uninstall target support; 'make uninstall'
configure_file  (
                  "${CMAKE_TEMPLATE_PATH}/uninstall.cmake.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
                  IMMEDIATE @ONLY
                )

add_custom_target ( uninstall COMMAND ${CMAKE_COMMAND} -P
                    ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
                  )

# TODO: Windows platform support
if ( NOT PLATFORM_WINDOWS )
  # CPack configuration
  include ( "${PROJECT_SOURCE_DIR}/cmake/CPackConfig.cmake" )

  include ( InstallRequiredSystemLibraries )

  # Platform specific generator presets
  if ( PLATFORM_OSX )
    set ( CPACK_GENERATOR "DragNDrop" )
  elseif ( PLATFORM_LINUX )
    set ( CPACK_GENERATOR "DEB" )
  elseif ( PLATFORM_UNKNOWN )
    set ( CPACK_GENERATOR "ZIP" )
  endif ( PLATFORM_OSX )

  include ( CPack )
endif ( NOT PLATFORM_WINDOWS )
