cmake_minimum_required ( VERSION 2.6 )

# TODO: Use CMake v2.8.12 OSX RPath features
# http://www.kitware.com/blog/home/post/510
# cmake_minimum_required ( VERSION 2.8.12 )

# CMake Environment

# Opt out of using CMake v3.0 PROJECT_VERSION variables management for the
# project.
# http://www.cmake.org/cmake/help/v3.0/command/project.html#command:project
if( POLICY CMP0048 )
  cmake_policy( SET CMP0048 OLD )
endif( POLICY CMP0048 )

set ( CMAKE_VERBOSE_MAKEFILE OFF CACHE PATH "Verbose Makefile" )
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" )
set ( CMAKE_TEMPLATE_PATH ${CMAKE_TEMPLATE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates" )

# Set the install prefix to the current build folder if the user has not chosen
# their own prefix path.
if ( NOT CMAKE_INSTALL_PREFIX )
  set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Install path prefix" )
endif ( NOT CMAKE_INSTALL_PREFIX )

set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 9 )
set ( PROJECT_VERSION_PATCH 1 )

# Project Options
option ( DEBUG "Build with all debugging features" off )
option ( DEBUG_ASSERT "Build with run-time assertions enabled" off )
option ( EXAMPLES "Build nomlib usage examples" off )
option( NOM_BUILD_TESTS "Build unit tests" off )

# Copy the generated documentation to the final distribution image; the logic
# differs depending on the type of "image" we are building (OS X framework
# bundle, source tar-ball and so forth).
set( NOM_INSTALL_GENERATED_DOCS off )

option( NOM_BUILD_CORE_UNIT "Engine core" true )

# TODO: Consider moving math unit into core?
option( NOM_BUILD_MATH_UNIT "Math utilities" true )

# Dependencies: OpenAL
option( NOM_BUILD_AUDIO_UNIT "Audio subsystem" TRUE )

# Dependencies: SDL2, SDL2_IMAGE, SDL2_TTF
option( NOM_BUILD_GRAPHICS_UNIT "Graphics core" true )

# TODO: Shorten this build var
# Dependencies: scale2x & hq2x (included in source tree)
option( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT "Include scale2x & hqx algorithms" TRUE )

# TODO: Break unit up further
option( NOM_BUILD_SYSTEM_UNIT "System unit" true )

option( NOM_BUILD_PTREE_UNIT "Generic container for serialization" true )
option( NOM_BUILD_SERIALIZERS_UNIT "Serialization core for JSON, XML, HTML" true )

# Dependencies: jsoncpp (included in source tree)
option( NOM_BUILD_JSON_UNIT "JSON I/O" true )

# Dependencies: RapidXml (included in source tree)
option( NOM_BUILD_XML_UNIT "XML I/O" FALSE )

option( NOM_BUILD_GUI_UNIT "GUI subsystem" TRUE )

# Collection of internal templates used in the creation of new functionality
# such as classes or examples / tests.
#
#     See also,
# Resources/SharedSupport/ExamplesTemplate.cpp
# Resources/SharedSupport/Template.cpp
# Resources/SharedSupport/Template.hpp
# Resources/SharedSupport/UnitTestTemplate.cpp
option( NOM_BUILD_TEMPLATES_UNIT true )

project( nomlib ) # Sets PROJECT_NAME variable for us

# Platform detection
include ( "${CMAKE_SOURCE_DIR}/cmake/platform.cmake" )

# Determine the required build unit dependencies based on the end-user's
# choice of the given CMake options
include( "${PROJECT_SOURCE_DIR}/cmake/BuildTreeDeps.cmake" )

if ( PLATFORM_WINDOWS )
  # Building nomlib on Windows is only supported as a static library for the
  # moment
  set ( BUILD_SHARED_LIBS off )
else ( NOT PLATFORM_WINDOWS )
  # We only support building nomlib as a dynamic library at the moment
  set ( BUILD_SHARED_LIBS on )
endif ( PLATFORM_WINDOWS )

if ( DEBUG )
  set ( CMAKE_VERBOSE_MAKEFILE ON )
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

# Utility macro helpers
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros.cmake" )

# Absolute path from project root to compiled source files
set ( SRC_DIR "${PROJECT_SOURCE_DIR}/src" )

# Relative path from project root containing project assets
#
# FIXME: Prepend ${PROJECT_SOURCE_DIR} to make this an absolute path
set ( NOMLIB_RESOURCES_DIR "Resources" )

# Unit tests root (absolute path)
set( NOM_TESTS_DIR "${PROJECT_SOURCE_DIR}/tests" )

# Unit tests source files root (absolute path)
set( NOM_TESTS_SOURCE_DIR "${NOM_TESTS_DIR}/src" )

# Unit tests header files root (absolute path)
set( NOM_TESTS_HEADERS_DIR "${NOM_TESTS_DIR}/include" )

# Relative path from project root containing SharedSupport directory
set ( NOMLIB_SHARED_SUPPORT_DIR "${NOMLIB_RESOURCES_DIR}/SharedSupport" )

set ( NOMLIB_HEADERS_ROOT "include" )

# Relative path from project root to nomlib's header files root
set ( NOMLIB_HEADERS_DIR "include/nomlib" )

# Relative path from project root to the external dependencies directory
set ( EXTERNAL_DEPS_DIR "third-party" )

# Absolute directory path to nomlib's common third-party dependency sources.
#
# NOTE: Contents of the path typically include header files and should be
# suitable for installation in third-party projects; we make a copy of the given
# path within the OS X Framework we build, for other projects (i.e.: TTcards) to
# be able to include from, purely for the sake of convenience.
set ( NOMLIB_THIRD_PARTY_INCLUDE_DIR
      "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/common"
    )

# Relative path from project root to nomlib demonstration examples
set ( EXAMPLES_SRC_DIR "${PROJECT_SOURCE_DIR}/examples" )

# Resources path for library examples
set( NOM_EXAMPLES_RESOURCES_DIR "${PROJECT_SOURCE_DIR}/${NOMLIB_RESOURCES_DIR}/examples" )

# Unit testing resources path
set( NOM_TESTS_RESOURCES_DIR "${PROJECT_SOURCE_DIR}/${NOMLIB_RESOURCES_DIR}/tests" )

# Distributed typographic fonts.
set ( NOMLIB_FONTS
      "${PROJECT_SOURCE_DIR}/Resources/fonts"
    )

# Additional flags to pass add_executable -- used when library examples are
# built
set ( EXECUTABLE_FLAGS "" )

# Files used with documentation generation
set ( PROJECT_DOXYGEN_DIR "${PROJECT_SOURCE_DIR}/${NOMLIB_RESOURCES_DIR}/doxygen" )

# OS X framework bundle variables used in generating the Info.plist
set ( BUNDLE_TEMPLATE_PLIST "${CMAKE_TEMPLATE_PATH}/Info.plist.in" )
set ( BUNDLE_NAME "nomlib" )
set ( BUNDLE_ICON "nomlib.icns" )
# TODO: Rename identifier prefix to net.i8degrees
set ( BUNDLE_IDENTIFIER "org.i8degrees.${PROJECT_NAME}" )

# Installation paths; CMAKE_INSTALL_PREFIX will be appended to these.
# Depending on the install profile, these paths are updated; such as when we
# are building an OS X framework bundle.
set ( INSTALL_LIB_PATH "lib" )
set ( INSTALL_RESOURCES_PATH "share/${PROJECT_NAME}" )
set ( INSTALL_HEADERS_PATH "include" )
set ( INSTALL_DOCS_PATH "share/doc/${PROJECT_NAME}" )
set ( INSTALL_GENERATED_DOCS_PATH "${INSTALL_DOCS_PATH}" )
set ( INSTALL_MAN_PAGES_PATH "share/man" )

# CMAKE_SYSTEM_PREFIX_PATH is searched to find libraries when the find_package
# command is used.
message ( STATUS "Library Search Prefix: ${CMAKE_SYSTEM_PREFIX_PATH}" )

# Installation prefix path set for our project
message ( STATUS "Installation Prefix: ${CMAKE_INSTALL_PREFIX}" )

# Doxygen configuration
include ( "${PROJECT_SOURCE_DIR}/cmake/doxygen.cmake" )

# Third-party dependencies
#
# TODO: Use find_package instead of manually setting up the environment
# TODO: we should be checking for the existence of these directories beforehand!
#
# We gather up all of the library & header search paths in two separate
# variables -- EXT_HEADERS & EXTLIBS -- so we can then make a minimal number
# of calls to include_directories & linking of libraries.
#

# Common dependencies (shared between all platforms)

# libRocket dependency
find_package( OpenGL REQUIRED )
if( OPENGL_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${OPENGL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${OPENGL_LIBRARY} )
endif( OPENGL_FOUND )

# This might be a short-term kludge; we are linking statically (by default) so
# that we are able to start using both nomlib and libRocket in TTcards with a
# minimum of packaging fuss for the project (statically linking here lets us
# remove libRocket from the linking of TTcards, and just requires the header
# files).
#
# I'm also not sure that it makes sense to abstract libRocket's higher-level
# APIs...
#
# This only affects builds on OS X. Windows builds always links statically.
set( NOM_USE_STATIC_LIBROCKET FALSE )
set( NOM_USE_LIBROCKET_LUA FALSE )

if ( PLATFORM_WINDOWS )
  # Header and library paths -- *INCLUDE_DIR and *LIBRARY_DIR, respectively --
  # *MUST* include trailing backslash for this platform!
  #
  # *LIBRARY_DIR variables are used to ease the install of run-time dependencies.
  # You should have only DLL and LIB file types in the library directories, in
  # order for our file filtering to weed out the proper ones during installation.

  # SDL2 dependency
  set ( SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2/include/" )
  set ( SDL2_LIBRARY
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2/lib/${PLATFORM_ARCH}/SDL2.lib"
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2/lib/${PLATFORM_ARCH}/SDL2main.lib"
      )
  set ( SDL2_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2/lib/${PLATFORM_ARCH}/" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_LIBRARY} )

  # SDL2_image dependency
  set ( SDL2_IMAGE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_image/include/" )
  set ( SDL2_IMAGE_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_image/lib/${PLATFORM_ARCH}/SDL2_image.lib" )
  set ( SDL2_IMAGE_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_image/lib/${PLATFORM_ARCH}/" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_IMAGE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_IMAGE_LIBRARY} )

  # SDL2_ttf dependency
  set ( SDL2_TTF_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_ttf/include/" )
  set ( SDL2_TTF_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_ttf/lib/${PLATFORM_ARCH}/SDL2_ttf.lib" )
  set ( SDL2_TTF_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_ttf/lib/${PLATFORM_ARCH}/" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_TTF_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_TTF_LIBRARY} )

  # OpenAL dependency
  set ( OPENAL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/OpenAL/include/" )
  set ( OPENAL_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/OpenAL/lib/${PLATFORM_ARCH}/OpenAL32.lib" )
  set ( OPENAL_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/OpenAL/lib/${PLATFORM_ARCH}/" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${OPENAL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${OPENAL_LIBRARY} )

  # libsndfile dependency
  set ( LIBSNDFILE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libsndfile/include/" )
  set ( LIBSNDFILE_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libsndfile/lib/${PLATFORM_ARCH}/" )
  set ( LIBSNDFILE_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libsndfile/lib/${PLATFORM_ARCH}/libsndfile-1.lib" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBSNDFILE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBSNDFILE_LIBRARY} )

  # Google Test dependency; only used if building with NOM_BUILD_TESTS
  set ( GTEST_ROOT
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/gtest"
      )

  # Paths must be: a) absolute; b) directory; c) end with a slash
  set ( MSVCPP_LIBRARY_REDIST "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/msvcpp2013/${PLATFORM_ARCH}/" )

  # libRocket dependency
  set ( GLEW_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/glew/include/GL/" )
  # Paths must be: a) absolute; b) directory; c) end with a slash
  set ( GLEW_REDIST_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/glew/bin/${PLATFORM_ARCH}/" )
  set ( GLEW_LIBRARY
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/glew/lib/${PLATFORM_ARCH}/glew32.lib"
      )
  set ( EXT_HEADERS ${EXT_HEADERS} "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/glew/include/GL" )
  set ( EXTLIBS ${EXTLIBS} ${GLEW_LIBRARY} )

  # libRocket dependency (GUI)
  set ( LIBROCKET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/include" )
  # Paths must be: a) absolute; b) directory; c) end with a slash
  set ( LIBROCKET_REDIST_DIRS
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/lib/${PLATFORM_ARCH}/;${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/bin/${PLATFORM_ARCH}/" )
  set ( LIBROCKET_LIBRARY
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/lib/${PLATFORM_ARCH}/RocketControls.lib"
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/lib/${PLATFORM_ARCH}/RocketCore.lib"
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/lib/${PLATFORM_ARCH}/RocketDebugger.lib"
      )
  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBROCKET_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBROCKET_LIBRARY} )

  # TODO: Group third-party dependency libraries for executables together

elseif ( PLATFORM_OSX )

  # SDL2 dependency
  set ( SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2.framework/Headers" )
  set ( SDL2_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2.framework" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_LIBRARY} )

  # SDL2_image dependency
  set ( SDL2_IMAGE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2_image.framework/Headers" )
  set ( SDL2_IMAGE_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2_image.framework" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_IMAGE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_IMAGE_LIBRARY} )

  # SDL2_ttf dependency
  set ( SDL2_TTF_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2_ttf.framework/Headers" )
  set ( SDL2_TTF_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2_ttf.framework" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_TTF_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_TTF_LIBRARY} )

  # OpenAL dependency
  set ( OPENAL_INCLUDE_DIR "/System/Library/Frameworks/OpenAL.framework" )
  set ( OPENAL_LIBRARY "/System/Library/Frameworks/OpenAL.framework" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${OPENAL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${OPENAL_LIBRARY} )

  # libsndfile dependency
  set ( LIBSNDFILE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/sndfile.framework/Headers" )
  set ( LIBSNDFILE_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/sndfile.framework" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBSNDFILE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBSNDFILE_LIBRARY} )

  # Google Test dependency; only used if building with NOM_BUILD_TESTS
  set ( GTEST_ROOT
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/gtest"
      )

  # libRocket dependency (FreeType2)
  #
  # FIXME: This file needs its RPATH modified; it is **not** being used as the
  # library for executables yet. We are using brew's version installed at
  # /usr/local
  set ( FREETYPE_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/bin/libfreetype.6.dylib" )
  set ( EXTLIBS ${EXTLIBS} ${FREETYPE_LIBRARY} )

  # libRocket dependency
  if( NOM_USE_LIBROCKET_LUA )
    find_package( Lua REQUIRED )

    if( LUA_FOUND )
      set ( EXT_HEADERS ${EXT_HEADERS} ${LUA_INCLUDE_DIR} )
      set ( EXTLIBS ${EXTLIBS} ${LUA_LIBRARY} )
    endif( LUA_FOUND )
  endif( NOM_USE_LIBROCKET_LUA )

  # libRocket dependency (GUI)

  set ( LIBROCKET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/include" )

  if( NOM_USE_STATIC_LIBROCKET )
    # Note that these are built without debugging (Release) because the
    # resulting file size was insane otherwise (+158MB).
    set ( LIBROCKET_LIBRARY
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketControls.a"
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketCore.a"
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketDebugger.a"
        )

    if( NOM_USE_LIBROCKET_LUA )
      set ( LIBROCKET_LIBRARY
            ${LIBROCKET_LIBRARY}
            "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketControlsLua.a"
            "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketCoreLua.a"
          )
    endif( NOM_USE_LIBROCKET_LUA )

  else( NOT NOM_USE_STATIC_LIBROCKET )
    # Debug libraries
    set ( LIBROCKET_LIBRARY
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketControls.1.dylib"
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketCore.1.dylib"
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketDebugger.1.dylib"
        )

    # NOTE: Lua bindings for the dynamic library build has not been tested!
    if( NOM_USE_LIBROCKET_LUA )
      set ( LIBROCKET_LIBRARY
            ${LIBROCKET_LIBRARY}
            "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketControlsLua.1.dylib"
            "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketCoreLua.1.dylib"
          )
    endif( NOM_USE_LIBROCKET_LUA )

  endif( NOM_USE_STATIC_LIBROCKET )

  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBROCKET_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBROCKET_LIBRARY} )

  find_library ( COCOA_LIBRARY Cocoa REQUIRED )
  mark_as_advanced ( COCOA_LIBRARY )
  set ( EXTLIBS ${EXTLIBS} ${COCOA_LIBRARY} )
  message( STATUS "Found Cocoa: ${COCOA_LIBRARY}")

elseif ( PLATFORM_LINUX )
  # TODO
endif ( PLATFORM_WINDOWS )

# RapidXml dependency; only used when building with the NOM_BUILD_XML option.
set ( RAPIDXML_INCLUDE_DIR "${NOMLIB_THIRD_PARTY_INCLUDE_DIR}/rapidxml" )
set ( EXT_HEADERS ${EXT_HEADERS} ${RAPIDXML_INCLUDE_DIR} )

# JsonCpp dependency; only used when building with the NOM_BUILD_JSON option.
set ( JSONCPP_SOURCE "${NOMLIB_THIRD_PARTY_INCLUDE_DIR}/jsoncpp/jsoncpp.cpp" )
set ( JSONCPP_INCLUDE_DIR "${NOMLIB_THIRD_PARTY_INCLUDE_DIR}/jsoncpp" )
set ( EXT_HEADERS ${EXT_HEADERS} ${JSONCPP_INCLUDE_DIR} )

# TCLAP dependency (headers only)
set( TCLAP_INCLUDE_DIR "${NOMLIB_THIRD_PARTY_INCLUDE_DIR}/tclap" )
set( EXT_HEADERS ${EXT_HEADERS} ${TCLAP_INCLUDE_DIR} )

message ( STATUS "SDL2 headers: ${SDL2_INCLUDE_DIR}" )
message ( STATUS "SDL2 library: ${SDL2_LIBRARY}" )

message ( STATUS "SDL2_image headers: ${SDL2_IMAGE_INCLUDE_DIR}" )
message ( STATUS "SDL2_image library: ${SDL2_IMAGE_LIBRARY}" )

message ( STATUS "SDL2_ttf headers: ${SDL2_TTF_INCLUDE_DIR}" )
message ( STATUS "SDL2_ttf library: ${SDL2_TTF_LIBRARY}" )

message ( STATUS "OpenAL headers: ${OPENAL_INCLUDE_DIR}" )
message ( STATUS "OpenAL library: ${OPENAL_LIBRARY}" )

message ( STATUS "libsndfile headers: ${LIBSNDFILE_INCLUDE_DIR}" )
message ( STATUS "libsndfile library: ${LIBSNDFILE_LIBRARY}" )

if( NOT EXISTS ${LIBROCKET_INCLUDE_DIR} )
  message ( FATAL_ERROR "libRocket headers *NOT* found: ${LIBROCKET_INCLUDE_DIR}" )
else( EXISTS ${LIBROCKET_INCLUDE_DIR} )
  message ( STATUS "libRocket headers: ${LIBROCKET_INCLUDE_DIR}" )
  # message ( STATUS "libRocket library: ${LIBROCKET_LIBRARY}" )
endif( NOT EXISTS ${LIBROCKET_INCLUDE_DIR} )

# TODO: Check for existence of #{LIBROCKET_LIBRARY} (multiple dylib files)

message ( STATUS "Found RapidXml: ${RAPIDXML_INCLUDE_DIR}" )

if( NOT EXISTS ${JSONCPP_INCLUDE_DIR} )
  message ( FATAL_ERROR "JsonCpp headers *NOT* found: ${JSONCPP_INCLUDE_DIR}" )
  message ( FATAL_ERROR "JsonCpp source *NOT* found: ${JSONCPP_SOURCE}" )
endif( NOT EXISTS ${JSONCPP_INCLUDE_DIR} )

if( NOT EXISTS ${TCLAP_INCLUDE_DIR} )
  message( FATAL_ERROR "TCLAP headers not found: ${TCLAP_INCLUDE_DIR}" )
else( EXISTS ${TCLAP_INCLUDE_DIR} )
  message( STATUS "TCLAP headers found: ${TCLAP_INCLUDE_DIR}" )
endif( NOT EXISTS ${TCLAP_INCLUDE_DIR} )

# External header files inclusion
include_directories ( ${EXT_HEADERS} )

# List of our source code to be compiled
include ( "${SRC_DIR}/CMakeLists.txt" )

# Project source inclusions (header files).
include_directories (
                      # nomlib's internal header files
                      "${NOMLIB_HEADERS_ROOT}"
                       # Third-party header files; RapidXml, JsonCpp
                      "${NOMLIB_THIRD_PARTY_INCLUDE_DIR}"
                    )

if ( BUILD_SHARED_LIBS )
  add_library ( ${PROJECT_NAME} SHARED ${NOMLIB_SOURCE} ${NOMLIB_HEADERS} )
else ( NOT BUILD_SHARED_LIBS )
  add_library ( ${PROJECT_NAME} STATIC ${NOMLIB_SOURCE} ${NOMLIB_HEADERS} )
endif ( BUILD_SHARED_LIBS )

target_link_libraries  ( ${PROJECT_NAME} ${EXTLIBS} )

set_target_properties ( ${PROJECT_NAME} # makefile target
                        PROPERTIES SOVERSION
                        "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
                        DEBUG_POSTFIX "-d"
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

    set ( INSTALL_LIB_PATH "${BUNDLE_NAME}.framework" )

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
    install ( TARGETS ${PROJECT_NAME}
              LIBRARY FRAMEWORK
              DESTINATION "${CMAKE_INSTALL_PREFIX}"
            )

    # Bundle external dependencies within our application bundle for distribution
    #
    # All paths must *NOT* include closing backslash character
    #
    # SDL frameworks are all from the official distribution, found on the
    # official web site at https://www.libsdl.org.
    #
    # SDL2.framework
    # SDL2_image.framework
    # SDL2_ttf.framework
    # sndfile.framework v1.0.25
    #
    # See README text within extlibs folder in root of this git repository
    # for packaging details of sndfile.

    # Bundled frameworks we depend on
    install ( DIRECTORY
              "${SDL2_LIBRARY}" # SDL2.framework
              "${SDL2_IMAGE_LIBRARY}" # SDL2_image.framework
              "${SDL2_TTF_LIBRARY}" # SDL2_ttf.framework
              "${LIBSNDFILE_LIBRARY}" # sndfile.framework
              DESTINATION "${INSTALL_EXT_FRAMEWORKS_PATH}"
              PATTERN ".*" EXCLUDE # do not include those pesky dotfiles
            )

    if( NOT NOM_USE_STATIC_LIBROCKET )
      # Bundle dynamic libraries we depend on
      #
      # TODO: Use "Libraries" directory for dynamic libraries.
      install ( FILES
                ${LIBROCKET_LIBRARY}
                DESTINATION "${INSTALL_EXT_FRAMEWORKS_PATH}"
              )
    endif( NOT NOM_USE_STATIC_LIBROCKET )

    # Add all the necessary runtime search paths for locating the installed
    # dependencies we bundled.
    set ( RUNTIME_PATH
          "@loader_path/../Frameworks"
          # SDL2_image.framework & SDL2_ttf.framework each have their own bundled
          # dependencies and therefore things get a bit crazier:
          "@loader_path/../../Frameworks"
        )

    # Let Xcode take care of things for us if user is building from Xcode
    # project files
    if ( NOT CMAKE_GENERATOR STREQUAL "Xcode" )
      add_rpath ( "${RUNTIME_PATH}" "${INSTALL_BINARY_PATH}" )
      install_name_rpath ( "@rpath/sndfile.framework/Versions/A/sndfile" "${LIBSNDFILE_LIBRARY}/Versions/A/sndfile" )
    endif ( NOT CMAKE_GENERATOR STREQUAL "Xcode" )

    # FIXME: This is a really lazy workaround to get things up and running for
    # travis-ci builds.
    if( CMAKE_GENERATOR STREQUAL "Xcode" )
      set( FONTS_DEST_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}" )
    else( CMAKE_GENERATOR NOT STREQUAL "Xcode" )
      set( FONTS_DEST_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" )
    endif( CMAKE_GENERATOR STREQUAL "Xcode" )

    # Install nomlib's default fonts
    #
    # FIXME: We need to be building the framework bundle locally -- within
    # cmake's build dir -- and then after *everything* is built, copy said
    # bundle over to the destination prefix.
    install (
              DIRECTORY
              "${NOMLIB_FONTS}"
              # DESTINATION "${INSTALL_ROOT_PATH}/Resources"
              DESTINATION "${FONTS_DEST_PREFIX}/${PROJECT_NAME}.framework/Resources"
              OPTIONAL
              PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
            )

  endif ( FRAMEWORK )

  # ...PLATFORM_OSX...

elseif ( PLATFORM_LINUX ) # POSIX install layout
  # TODO: set paths, etc. like we do above with OSX & Windows platforms

  # Install binaries to our platform-dependent paths we've set
  install ( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${INSTALL_LIB_PATH}" )
elseif ( PLATFORM_WINDOWS )
  # CMAKE_INSTALL_PREFIX will be appended here for us when we issue the install
  # command.
  #set ( INSTALL_LIB_PATH "lib" )

  # TODO:
  #set ( INSTALL_RESOURCES_PATH "${INSTALL_APP_PATH}" )
  #set ( INSTALL_DOCS_PATH "share/doc/${PROJECT_NAME}" )
  #set ( INSTALL_DOCS_HELP_PATH "${INSTALL_DOCS_PATH}" )
  #set ( INSTALL_GENERATED_DOCS_PATH "share/doc/${PROJECT_NAME}" )

  #if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    #set ( LIBRARY_NAME "${PROJECT_NAME}-d" )
  #else ( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )
    #set ( LIBRARY_NAME "${PROJECT_NAME}" )
  #endif ( CMAKE_BUILD_TYPE STREQUAL "Debug" )

  # This is a workaround hack
  #install ( FILES "${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}-d.lib"
            #DESTINATION "${INSTALL_LIB_PATH}"
          #)

  # Install library
  #install ( TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}" )
endif ( PLATFORM_OSX ) # Platform-dependent build setup

# Install nomlib header files
install ( DIRECTORY ${NOMLIB_HEADERS_DIR}
          DESTINATION "${INSTALL_HEADERS_PATH}"
          PATTERN ".*" EXCLUDE # do not include those pesky dotfiles
        )

# Install nomlib Resource files
install ( FILES
          "${NOMLIB_RESOURCES_DIR}/nomlib.icns" # Icon
          DESTINATION "${INSTALL_RESOURCES_PATH}/${NOMLIB_RESOURCES_DIR}"
          OPTIONAL
        )

# TODO: Install nomlib "SharedSupport" Resource files.

# Install nomlib's cmake module; this should end up in a spot that is
# automatically searched by find_package.
if ( PLATFORM_OSX AND FRAMEWORK ) # Framework bundle profile
  install ( DIRECTORY
            "${NOMLIB_RESOURCES_DIR}/CMake"
            DESTINATION
            "${INSTALL_RESOURCES_PATH}/${NOMLIB_RESOURCES_DIR}"
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles
          )
else ( NOT PLATFORM_OSX AND NOT FRAMEWORK ) # POSIX install profile
  install ( DIRECTORY
            "${NOMLIB_RESOURCES_DIR}/CMake"
            DESTINATION
            "${INSTALL_RESOURCES_PATH}"
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles
          )
endif ( PLATFORM_OSX AND FRAMEWORK )

# Install software license & general project information
install ( FILES
          ${PROJECT_SOURCE_DIR}/README.md
          ${PROJECT_SOURCE_DIR}/LICENSE.md
          DESTINATION "${INSTALL_DOCS_PATH}"
        )

# Copy the generated documentation to the final distribution image
if( NOM_INSTALL_GENERATED_DOCS )

  # Install the optionally generated HTML documentation
  install (
            DIRECTORY ${PROJECT_BINARY_DIR}/docs/html
            # Should resolve alike: nomlib.framework/Resources/Documentation
            DESTINATION "${INSTALL_GENERATED_DOCS_PATH}"
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles
          )

  # Install the generated man page documentation if we are not building an OS X
  # framework bundle (POSIX friendly).
  if( NOT FRAMEWORK )

    install (
              DIRECTORY ${PROJECT_BINARY_DIR}/docs/man/man3
              DESTINATION "${INSTALL_MAN_PAGES_PATH}"
              OPTIONAL
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )

  endif( NOT FRAMEWORK )
endif( NOM_INSTALL_GENERATED_DOCS )

if ( PLATFORM_LINUX )
  install(CODE "MESSAGE(\"Post-install: sudo ldconfig.\")") # FIXME (prettify)
endif ( PLATFORM_LINUX )

# nomlib examples configuration
if( EXAMPLES )
  add_subdirectory( examples )
endif( EXAMPLES )

# NOTE: we *MUST* enable testing from the top-level build script -- that's this
# file -- or else 'make test' breaks. (Dated 2014-04-05)
if( NOM_BUILD_TESTS )
  enable_testing()
  add_subdirectory( ${NOM_TESTS_DIR} )
endif( NOM_BUILD_TESTS )

# Auto-generated library version C++ header file
configure_file  (
                  "${CMAKE_TEMPLATE_PATH}/version.hpp.in"
                  "${PROJECT_SOURCE_DIR}/${NOMLIB_HEADERS_DIR}/version.hpp"
                )

# Auto-generated library configuration C++ header file
configure_file  (
                  "${CMAKE_TEMPLATE_PATH}/config.hpp.in"
                  "${PROJECT_SOURCE_DIR}/${NOMLIB_HEADERS_DIR}/config.hpp"
                )

# Make a version file containing the current version from git
#
# Source: http://brianmilco.blogspot.com/2012/11/cmake-automatically-use-git-tags-as.html
include( GetGitRevisionDescription )
git_describe( GIT_REVISION )
string( REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${GIT_REVISION}" )

configure_file  (
                  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/revision.cpp.in
                  ${SRC_DIR}/revision.cpp
                )

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
