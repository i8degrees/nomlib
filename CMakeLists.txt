cmake_minimum_required ( VERSION 2.6 )

# CMake Environment
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" )
set ( CMAKE_TEMPLATE_PATH ${CMAKE_TEMPLATE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates" )

# Set the install prefix to the current build folder if the user has not chosen
# their own prefix path.
if ( NOT CMAKE_INSTALL_PREFIX )
  set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Install path prefix" )
endif ( NOT CMAKE_INSTALL_PREFIX )

set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 9 )
set ( PROJECT_VERSION_PATCH 0 )

# Project Options
option ( DEBUG "Build with all debugging features" off )
option ( DEBUG_ASSERT "Build with run-time assertions enabled" off )
option ( DEBUG_TRACE "Build with trace logging of object construction & destruction" off )
option ( EXAMPLES "Build nomlib usage examples" off )
option ( DOCS "Generate HTML based API documentation -- requires Doxygen" off )

project ( nomlib ) # Sets PROJECT_NAME variable for us

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
  set ( CMAKE_BUILD_TYPE "Debug" )

  if ( NOT PLATFORM_WINDOWS )
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D NOM_DEBUG -Wall -Wunused" )
  else ( PLATFORM_WINDOWS )
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D NOM_DEBUG /D NOM_DEBUG_ASSERT /D NOM_DEBUG_TRACE" )
  endif( NOT PLATFORM_WINDOWS )

  set ( CMAKE_VERBOSE_MAKEFILE on )
  message ( STATUS "Building ${PROJECT_NAME} with debugging." )
  message ( STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}" )
else () # Build with optimizations for maximum speed and minimal size
  set ( CMAKE_BUILD_TYPE "Release" )
  message ( STATUS "Building ${PROJECT_NAME} with high speed, low drag!" )
  message ( STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}" )
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

# Utility macro helpers
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros.cmake" )

# Relative path from project root to compiled source files
set ( SRC_DIR "${PROJECT_SOURCE_DIR}/src" )

# Relative path from project root containing additional data
set ( NOMLIB_RESOURCES_DIR "Resources" )

# Relative path from project root to header files that are used with the source
# files for compiling
set ( NOMLIB_HEADERS_DIR "include" )
set ( EXTERNAL_HEADERS_DIR "extlibs/osx/include" )

# Absolute path to the json_spirit header files
set ( HEADERS_JSON_SPIRIT_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_HEADERS_DIR}/json_spirit" )

# Relative path from project root to nomlib demonstration examples
set ( EXAMPLES_SRC_DIR "${PROJECT_SOURCE_DIR}/examples" )

# Files used with documentation generation
set ( PROJECT_DOXYGEN_DIR "${PROJECT_SOURCE_DIR}/${NOMLIB_RESOURCES_DIR}/doxygen" )

# OS X framework bundle variables used in generating the Info.plist
set ( BUNDLE_TEMPLATE_PLIST "${CMAKE_TEMPLATE_PATH}/Info.plist.in" )
set ( BUNDLE_NAME "nomlib" )
set ( BUNDLE_ICON "nomlib.icns" )
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

if ( PLATFORM_OSX )
  option ( FRAMEWORK "Build OSX Framework instead of dylib" on )
  option ( UNIVERSAL "Build as an OSX Universal Library" off )
endif ( PLATFORM_OSX )

# I was able to shave off close to 50% time compiling nomlib by leaving
# this option off by default.
#
# PPC is not officially supported because I have no means whatsoever of
# testing such a package.
if ( UNIVERSAL )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( STATUS "Building an OSX Universal Library" )
endif ( UNIVERSAL )

message ( STATUS "Installation Prefix: ${CMAKE_INSTALL_PREFIX}" )

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

elseif ( PLATFORM_WINDOWS )

  set ( CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}" )

else () # Not Linux nor OS X nor Windows
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
if ( PLATFORM_WINDOWS )

  # SDL2 dependency
  set ( SDL_INCLUDE_DIR "third-party/SDL2/include/" )
  set ( SDL_LIBRARY "third-party/SDL2/lib/SDL2.lib" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL_LIBRARY} )

  # SDL2_image dependency
  set ( SDL2_IMAGE_INCLUDE_DIR "third-party/SDL2_image/include/" )
  set ( SDL2_IMAGE_LIBRARY "third-party/SDL2_image/lib/SDL2_image.lib" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_IMAGE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_IMAGE_LIBRARY}  )

  # SDL2_ttf dependency
  set ( SDL2_TTF_INCLUDE_DIR "third-party/SDL2_ttf/include/" )
  set ( SDL2_TTF_LIBRARY "third-party/SDL2_ttf/lib/SDL2_ttf.lib" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_TTF_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_TTF_LIBRARY} )

  # OpenAL dependency -- must be 32-bit library
  set ( OPENAL_INCLUDE_DIR "third-party/OpenAL/include/" )
  set ( OPENAL_LIBRARY "third-party/OpenAL/lib/OpenAL32.lib" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${OPENAL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${OPENAL_LIBRARY} )

  # libsndfile dependency -- must be 32-bit library
  set ( LIBSNDFILE_INCLUDE_DIR "third-party/libsndfile/include/" )
  set ( LIBSNDFILE_LIBRARY "third-party/libsndfile/lib/libsndfile-1.lib" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBSNDFILE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBSNDFILE_LIBRARY} )

else ( NOT PLATFORM_WINDOWS )

  find_package ( SDL2 REQUIRED )
  find_package ( SDL2_image REQUIRED )
  find_package ( SDL2_ttf REQUIRED )

  find_package ( OpenAL REQUIRED )
  find_package ( sndfile REQUIRED )

  if ( SDL2_FOUND )
    set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_INCLUDE_DIR} )
    set ( EXTLIBS ${EXTLIBS} ${SDL2_LIBRARY} )
  endif ( SDL2_FOUND )

  if ( SDL2_IMAGE_FOUND )
    set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_IMAGE_INCLUDE_DIR} )
    set ( EXTLIBS ${EXTLIBS} ${SDL2_IMAGE_LIBRARY} )
  endif ( SDL2_IMAGE_FOUND )

  if ( SDL2_TTF_FOUND )
    set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_TTF_INCLUDE_DIR} )
    set ( EXTLIBS ${EXTLIBS} ${SDL2_TTF_LIBRARY} )
  endif ( SDL2_TTF_FOUND )

  if ( OPENAL_FOUND )
    set ( EXT_HEADERS ${EXT_HEADERS} ${OPENAL_INCLUDE_DIR} )
    set ( EXTLIBS ${EXTLIBS} ${OPENAL_LIBRARY} )
  else ( NOT OPENAL_FOUND )
    message ( FATAL_ERROR "Required dependency OpenAL missing!" )
  endif ( OPENAL_FOUND )

  if ( LIBSNDFILE_FOUND )
    set ( EXT_HEADERS ${EXT_HEADERS} ${LIBSNDFILE_INCLUDE_DIR} )
    set ( EXTLIBS ${EXTLIBS} ${LIBSNDFILE_LIBRARY} )
  else ( NOT LIBSNDFILE_FOUND )
    message ( FATAL_ERROR "Required dependency sndfile missing!" )
  endif ( LIBSNDFILE_FOUND )

  # First, try finding our packaged header files for json_spirit;
  # ...if this fails, attempt to find the headers directory of a system install of
  # the library header files
  if ( IS_DIRECTORY "${HEADERS_JSON_SPIRIT_DIR}" )
    set ( EXT_HEADERS ${EXT_HEADERS} "${HEADERS_JSON_SPIRIT_DIR}" )
    message ( STATUS "Found packaged json_spirit: ${HEADERS_JSON_SPIRIT_DIR}" )
  elseif ( NOT IS_DIRECTORY "${HEADERS_JSON_SPIRIT_DIR}" )

    find_package ( json_spirit REQUIRED )
    if ( JSON_SPIRIT_FOUND )
      set ( EXT_HEADERS ${EXT_HEADERS} ${JSON_SPIRIT_INCLUDE_DIR} )
      message ( WARNING "Found json_spirit: ${JSON_SPIRIT_INCLUDE_DIR}" )
    else ( NOT JSON_SPIRIT_FOUND )
      message ( FATAL_ERROR "Required dependency json_spirit (headers only) missing!" )
    endif ( JSON_SPIRIT_FOUND )
  endif ( IS_DIRECTORY "${HEADERS_JSON_SPIRIT_DIR}" )

  if ( PLATFORM_OSX )
    find_library ( COCOA_LIBRARY Cocoa )
    mark_as_advanced ( COCOA_LIBRARY )
    set ( EXTLIBS ${EXTLIBS} ${COCOA_LIBRARY} )
  endif ( PLATFORM_OSX )

endif ( PLATFORM_WINDOWS )

# External header files inclusion
include_directories ( ${EXT_HEADERS} )

if ( BUILD_SHARED_LIBS )
  add_library ( ${PROJECT_NAME} SHARED ${NOMLIB_SOURCE} )
else ( NOT BUILD_SHARED_LIBS )
  add_library ( ${PROJECT_NAME} STATIC ${NOMLIB_SOURCE} )
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
              "${SDL2_LIBRARY}" # SDL.framework
              "${SDL2_IMAGE_LIBRARY}" # SDL_image.framework
              "${SDL2_TTF_LIBRARY}" # SDL_ttf.framework
              "${LIBSNDFILE_LIBRARY}" # sndfile.framework
              DESTINATION "${INSTALL_EXT_FRAMEWORKS_PATH}"
              PATTERN ".*" EXCLUDE # do not include those pesky dotfiles
            )

    # Add all the necessary runtime search paths for locating the installed
    # dependencies we bundled.
    set ( RUNTIME_PATH
          "@loader_path/../Frameworks"
          # SDL2_image.framework & SDL2_ttf.framework each have their own bundled
          # dependencies and therefore things get a bit crazier:
          "@loader_path/../../Frameworks"
        )

    add_rpath ( "${RUNTIME_PATH}" "${INSTALL_BINARY_PATH}" )

    install_name_rpath ( "@rpath/sndfile.framework/Versions/A/sndfile" "${LIBSNDFILE_LIBRARY}/Versions/A/sndfile" )

  endif ( FRAMEWORK )
endif ( PLATFORM_OSX )

if ( NOT FRAMEWORK )

  if ( PLATFORM_WINDOWS )

    install ( TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION "${INSTALL_LIB_PATH}" )
  else ( NOT PLATFORM_WINDOWS )

    # Install library in POSIX standard location by default
    install ( TARGETS ${PROJECT_NAME} LIBRARY
              DESTINATION "${INSTALL_LIB_PATH}"
            )
  endif ( PLATFORM_WINDOWS )
endif ( NOT FRAMEWORK )

if ( NOT PLATFORM_WINDOWS )

  # Install nomlib header files
  install ( DIRECTORY ${NOMLIB_HEADERS_DIR}/${PROJECT_NAME}
            DESTINATION "${INSTALL_HEADERS_PATH}"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles
          )

  # Install nomlib Resource files
  install ( FILES
            "${NOMLIB_RESOURCES_DIR}/nomlib.icns" # Icon
            DESTINATION "${INSTALL_RESOURCES_PATH}/${NOMLIB_RESOURCES_DIR}"
            OPTIONAL
          )

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
endif ( NOT PLATFORM_WINDOWS )

# nomlib examples configuration
if ( EXAMPLES )
  add_subdirectory ( examples )
endif ( EXAMPLES )

# Auto-generated source file
configure_file  ( "${CMAKE_TEMPLATE_PATH}/version.cpp.in"
                  "${SRC_DIR}/version.cpp"
                )

# Auto-generated header file
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

