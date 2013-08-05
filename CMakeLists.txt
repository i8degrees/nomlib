cmake_minimum_required ( VERSION 2.6 )

# CMake Environment
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" )
set ( CMAKE_TEMPLATE_PATH ${CMAKE_TEMPLATE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates" )

# CMake options
option ( VERBOSE "Enable verbose makefile" off )
option ( CMAKE_INSTALL_PREFIX "Install project in a custom location" ${CMAKE_CURRENT_BINARY_DIR} )

if ( VERBOSE )
  set ( CMAKE_VERBOSE_MAKEFILE true )
endif ( VERBOSE )

# Set the install prefix to the current build folder if this option has not been
# set.
if ( NOT CMAKE_INSTALL_PREFIX )
  set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" )
endif ( NOT CMAKE_INSTALL_PREFIX )

set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 1 )
set ( PROJECT_VERSION_PATCH 0 )

# We only support building nomlib as a dynamic library at the moment
set ( BUILD_SHARED_LIBS on )

# Project Options

option ( DEBUG "Build with all debugging features" off )
option ( DEBUG_ASSERT "Build with run-time assertions enabled" off )
option ( DEBUG_TRACE "Build with trace logging of object construction & destruction" off )
option ( EXAMPLES "Build nomlib usage examples" off )
option ( DOCS "Generate HTML based API documentation -- requires Doxygen" off )

project ( nomlib ) # $PROJECT_NAME

# Platform detection
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/platform.cmake" )

# Utility macro helpers
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros.cmake" )

set ( SRC_DIR "${PROJECT_SOURCE_DIR}/src" ) # nomlib/src
set ( NOMLIB_HEADERS_DIR "include" ) # include/nomlib
set ( EXAMPLES_SRC_DIR "${PROJECT_SOURCE_DIR}/examples" ) # nomlib/examples
set ( NOMLIB_RESOURCES_DIR "Resources" ) # folder containing library data (ex.: icon, translations, etc.)
# Files used with documentation generation
set ( PROJECT_DOXYGEN_DIR "${PROJECT_SOURCE_DIR}/${NOMLIB_RESOURCES_DIR}/doxygen" )

# Information used in the generation of the packaged Info.plist
set ( BUNDLE_TEMPLATE_PLIST "${CMAKE_TEMPLATE_PATH}/Info.plist.in" )
set ( BUNDLE_NAME "nomlib" )
set ( BUNDLE_ICON "nomlib.icns" )
set ( BUNDLE_IDENTIFIER "org.i8degrees.${PROJECT_NAME}" )

# Install paths
set ( INSTALL_LIB_PATH "lib" )
set ( INSTALL_RESOURCES_PATH "share/${PROJECT_NAME}" )
set ( INSTALL_HEADERS_PATH "include" )
set ( INSTALL_DOCS_PATH "$share/doc/${PROJECT_NAME}" )
set ( INSTALL_GENERATED_DOCS_PATH "${INSTALL_DOCS_PATH}" )
set ( INSTALL_MAN_PAGES_PATH "share/man" )

if ( PLATFORM_OSX )
  option ( FRAMEWORK "Build OSX Framework instead of dylib" on )
  option ( UNIVERSAL "Build as an OSX Universal Library" off )
endif ( PLATFORM_OSX )

if ( DEBUG )
  set ( CMAKE_BUILD_TYPE "Debug" )
  set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNOM_DEBUG" )
  message ( STATUS "Building ${PROJECT_NAME} with debugging." )
else () # Build with optimizations for maximum speed and minimal size
  set ( CMAKE_BUILD_TYPE "Release" )
  message ( STATUS "Building ${PROJECT_NAME} with high speed, low drag!" )
endif ( DEBUG )

if ( DEBUG_ASSERT )
  add_definitions ( "-DNOM_DEBUG_ASSERT" )
  message ( STATUS "Building ${PROJECT_NAME} with run-time assertions." )
endif ( DEBUG_ASSERT )

if ( DEBUG_TRACE )
  # We intentionally do not set CMAKE_BUILD_TYPE to "Debug" or "Release" here so
  # we can enable this feature in either target mode to maximize debugging
  # capabilities.
  add_definitions ( "-DNOM_DEBUG_TRACE" )
  message ( STATUS "Building ${PROJECT_NAME} with class object call tracing." )
endif ( DEBUG_TRACE )

# I was able to shave off close to 50% time compiling nomlib by leaving
# this option off by default.
#
# PPC is not officially supported because I have no means whatsoever of
# testing such a package.
if ( UNIVERSAL )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( STATUS "Building an OSX Universal Library" )
endif ( UNIVERSAL )

if ( FRAMEWORK )
  message ( STATUS "Using OSX Framework bundle profile." )
else ( NOT FRAMEWORK )
  message ( STATUS "Using POSIX build profile." )
endif ( FRAMEWORK )

# Platform specific configuration
if ( PLATFORM_LINUX ) # Tested on Ubuntu v12.04-LTS

  # Clang is not supported on Linux due to libc++ not being distributed by
  # default yet
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )
elseif ( PLATFORM_OSX )

  # libc++ requires OSX v10.7+
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++" )

  if ( CMAKE_GENERATOR STREQUAL "Xcode" ) # Available only on OSX, I think
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
  endif ( CMAKE_GENERATOR STREQUAL "Xcode" )
else () # Not Linux nor OSX
  # Use whatever CMake gathers for us and hope for the best!
endif ( PLATFORM_LINUX )

if ( DOCS )
  include ( "${PROJECT_SOURCE_DIR}/cmake/doxygen.cmake" )
endif ( DOCS )

# List of our source code to be compiled
include ( "${SRC_DIR}/CMakeLists.txt" )

# Project Header Files
include_directories ( ${NOMLIB_HEADERS_DIR} )

# Required Dependencies
#
# We gather up all of the library & header search paths in two separate
# variables -- EXT_HEADERS & EXTLIBS -- so we can then make a minimal number
# of calls to include_directories & linking of libraries.
#
find_package ( SDL REQUIRED )
find_package ( SDL_image REQUIRED )
find_package ( SDL_ttf REQUIRED )

find_package ( OpenAL REQUIRED )
find_package ( LibSndFile REQUIRED )

find_package ( LibMagic REQUIRED )

# LibMagic requires zlib when statically linking
if ( PLATFORM_OSX )
  find_package ( ZLIB REQUIRED )
endif ( PLATFORM_OSX )

# We need GTK to display dialog message boxes if we are not building a debug
# enabled library.
if ( NOT DEFINED DEBUG AND PLATFORM_LINUX )
  find_package ( GTK )
endif ( NOT DEFINED DEBUG AND PLATFORM_LINUX )

if ( SDL_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL_LIBRARY} )
endif ( SDL_FOUND )

if ( SDL_IMAGE_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL_IMAGE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDLIMAGE_LIBRARY} )
endif ( SDL_IMAGE_FOUND )

if ( SDL_TTF_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL_TTF_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDLTTF_LIBRARY} )
endif ( SDL_TTF_FOUND )

if ( OPENAL_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${OPENAL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${OPENAL_LIBRARY} )
endif ( OPENAL_FOUND )

if ( LIBSNDFILE_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBSNDFILE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBSNDFILE_LIBRARY} )
endif ( LIBSNDFILE_FOUND )

if ( LIBMAGIC_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBMAGIC_INCLUD_DIR} )

  # Statically linking in OS X frees us from the chore of worrying about the
  # runtime search paths issue, which we never was able to figure out how to
  # do so with this library ...
  #
  # See also cmake/modules/FindLibMagic.cmake

  # We must link zlib into our library because the burden falls on us when
  # statically linking a library to ours.
  if ( PLATFORM_OSX )
    if ( ZLIB_FOUND )
      set ( EXTLIBS ${EXTLIBS} ${LIBMAGIC_LIBRARY} ${ZLIB_LIBRARY} )
    endif ( ZLIB_FOUND )
  # Otherwise we do not need to link zlib with libmagic as we will be linking
  # libmagic as a dynamic library -- the burden then falls on libmagic for
  # finding zlib, not us!
  else ( NOT PLATFORM_OSX )
    set ( EXTLIBS ${EXTLIBS} ${LIBMAGIC_LIBRARY} )
  endif ( PLATFORM_OSX )

endif ( LIBMAGIC_FOUND )

if ( PLATFORM_LINUX AND GTK_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${GTK_INCLUD_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${GTK_LIBRARY} )
endif ( PLATFORM_LINUX AND GTK_FOUND )

# Now that we have collected all the header file search paths we need, we
# include them all here for the compiler to be made aware of
include_directories ( ${EXT_HEADERS} )

add_library ( ${PROJECT_NAME} SHARED ${NOMLIB_SOURCE} )

target_link_libraries  ( ${PROJECT_NAME} ${EXTLIBS} )

set_target_properties ( ${PROJECT_NAME} # makefile target
                        PROPERTIES SOVERSION
                        "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
                      )

if ( PLATFORM_OSX )

  set ( INSTALL_NAME_PATH "@rpath" )

  # For distribution in user's frameworks / OSX App Bundle
  set_target_properties ( ${PROJECT_NAME} PROPERTIES
                          BUILD_WITH_INSTALL_RPATH 1
                          INSTALL_NAME_DIR
                          "${INSTALL_NAME_PATH}"
                        )

  if ( FRAMEWORK )

    set ( INSTALL_LIB_PATH "${PROJECT_NAME}.framework" )

    set ( INSTALL_ROOT_PATH "${INSTALL_LIB_PATH}" )

    set ( INSTALL_RESOURCES_PATH "${INSTALL_ROOT_PATH}" )
    set ( INSTALL_HEADERS_PATH "${INSTALL_ROOT_PATH}/Headers" )

    set ( INSTALL_DOCS_PATH "${INSTALL_RESOURCES_PATH}/Resources" )
    set ( INSTALL_GENERATED_DOCS_PATH "${INSTALL_DOCS_PATH}/Documentation" )

    set ( INSTALL_BINARY_PATH "${INSTALL_ROOT_PATH}/Versions/A/${PROJECT_NAME}" )

    # * All paths must *NOT* include closing backslash character
    set ( INSTALL_EXT_FRAMEWORKS_PATH "${INSTALL_ROOT_PATH}/Frameworks" )
    set ( INSTALL_EXT_LIBRARIES_PATH "${INSTALL_ROOT_PATH}/Libraries" )

    # For distribution in user's frameworks / OSX App Bundle
    set_target_properties ( ${PROJECT_NAME} PROPERTIES
                            BUILD_WITH_INSTALL_RPATH 1
                            INSTALL_NAME_DIR
                            "${INSTALL_NAME_PATH}"
                          )

    # Frameworks distribution
    set_target_properties ( ${PROJECT_NAME} PROPERTIES
                            FRAMEWORK
                            TRUE
                            MACOSX_FRAMEWORK_INFO_PLIST
                            "${BUNDLE_TEMPLATE_PLIST}"
                            MACOSX_FRAMEWORK_NAME
                            "${BUNDLE_NAME}"
                            MACOSX_FRAMEWORK_ICON_FILE
                            "${BUNDLE_ICON}"
                            MACOSX_FRAMEWORK_BUNDLE_VERSION
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
                            MACOSX_FRAMEWORK_SHORT_VERSION_STRING
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                            MACOSX_FRAMEWORK_IDENTIFIER
                            "${BUNDLE_IDENTIFIER}"
                          )

    # Install framework library file
    install ( TARGETS ${PROJECT_NAME} LIBRARY FRAMEWORK DESTINATION "${CMAKE_INSTALL_PREFIX}" )

    # Bundle external dependencies within our application bundle for distribution
    #
    # * All paths must *NOT* include closing backslash character
    #
    # SDL frameworks are all from the official distribution, found on the
    # official web site at https://www.libsdl.org.
    #
    # SDL.framework v1.2.15
    # SDL_image.framework v1.2.12
    # SDL_ttf.framework v2.0.11

    # sndfile.framework v1.0.25
    #
    # See README text within extlibs folder in root of this git repository
    # for packaging details of libsndfile.

    # The SDL_LIBRARY variable for OS X is appended with the "-framework Cocoa"
    # linkage flag, so we have to sanitize this before hand for use in locating
    # and bundling this dependency.
    string ( REPLACE "-framework Cocoa" "" SDL_REAL_LIBRARY ${SDL_LIBRARY} )

    # Bundled frameworks we depend on
    install ( DIRECTORY
              "${SDL_REAL_LIBRARY}" # SDL.framework
              "${SDLIMAGE_LIBRARY}" # SDL_image.framework
              "${SDLTTF_LIBRARY}" # SDL_ttf.framework
              "${LIBSNDFILE_LIBRARY}" # sndfile.framework
              DESTINATION "${INSTALL_EXT_FRAMEWORKS_PATH}"
            )

    # Add all the necessary runtime search paths for locating the installed
    # dependencies we bundled.
    set ( RUNTIME_PATH
          "@loader_path/../Frameworks"
          # SDL_image.framework & SDL_ttf.framework each have their own bundled
          # dependencies and therefore:
          "@loader_path/../../Frameworks"
        )

    install_rpath ( "${RUNTIME_PATH}" "${INSTALL_BINARY_PATH}" )

  endif ( FRAMEWORK )

endif ( PLATFORM_OSX )

if ( NOT FRAMEWORK )
  # Install library in POSIX standard location by default
  install ( TARGETS ${PROJECT_NAME} LIBRARY
            DESTINATION "${INSTALL_LIB_PATH}"
          )
endif ( NOT FRAMEWORK )

# Install nomlib header files
install ( DIRECTORY ${NOMLIB_HEADERS_DIR}/${PROJECT_NAME}
          DESTINATION "${INSTALL_HEADERS_PATH}"
          PATTERN ".*" EXCLUDE # do not include dotfiles
        )

# Install nomlib Resource files
install ( FILES
          "${NOMLIB_RESOURCES_DIR}/nomlib.icns" # Icon
          "${NOMLIB_RESOURCES_DIR}/magic.mgc" # libmagic mime db
          DESTINATION "${INSTALL_RESOURCES_PATH}/${NOMLIB_RESOURCES_DIR}"
          OPTIONAL
        )

# Install software license & general project information
install ( FILES
          ${PROJECT_SOURCE_DIR}/README.md
          ${PROJECT_SOURCE_DIR}/LICENSE.md
          ${PROJECT_SOURCE_DIR}/TODO.md
          DESTINATION "${INSTALL_DOCS_PATH}"
        )

if ( DOCS )
  # Install generated HTML documentation
  install ( DIRECTORY ${PROJECT_BINARY_DIR}/docs/html
            DESTINATION "${INSTALL_GENERATED_DOCS_PATH}"
            PATTERN ".*" EXCLUDE # do not include dotfiles
          )

  if ( NOT FRAMEWORK )

    # Install generated man page documentation
    install ( DIRECTORY ${PROJECT_BINARY_DIR}/docs/man/man3
              DESTINATION "${INSTALL_MAN_PAGES_PATH}"
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )
    endif ( NOT FRAMEWORK )
endif ( DOCS )

if ( PLATFORM_LINUX )
  install(CODE "MESSAGE(\"Post-install: sudo ldconfig.\")") # FIXME (prettify)
endif ( PLATFORM_LINUX )

# nomlib examples configuration
if ( EXAMPLES )

  set ( EXECUTABLE_OUTPUT_PATH "${EXAMPLES_SRC_DIR}" )

  set ( NOMLIB_EXAMPLES_AUDIO
        ${EXAMPLES_SRC_DIR}/audio/audio.cpp
      )

  set ( NOMLIB_EXAMPLES_APP
        ${EXAMPLES_SRC_DIR}/app/app.cpp
      )

  set ( NOMLIB_EXAMPLES_FONTS
      ${EXAMPLES_SRC_DIR}/fonts/font.cpp
      )

  link_libraries ( ${PROJECT_NAME} ${SDL_LIBRARY} "SDLmain" )
  add_executable ( audio/audio ${NOMLIB_EXAMPLES_AUDIO} )
  add_executable ( app/app ${NOMLIB_EXAMPLES_APP} )
  add_executable ( fonts/font ${NOMLIB_EXAMPLES_FONTS} )

endif ( EXAMPLES )

# Auto-generated at build-time header files
configure_file  ( "${CMAKE_TEMPLATE_PATH}/version.hpp.in"
                  "${PROJECT_SOURCE_DIR}/${NOMLIB_HEADERS_DIR}/${PROJECT_NAME}/version.hpp"
                )

# Uninstall target support (always available)
configure_file  ( "${CMAKE_TEMPLATE_PATH}/uninstall.cmake.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
                  IMMEDIATE @ONLY
                )

add_custom_target ( uninstall COMMAND ${CMAKE_COMMAND} -P
                    ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
                  )
