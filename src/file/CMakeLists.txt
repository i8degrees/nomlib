# nomlib-file library

# !! This is an unfinished library target

# Platform-specific implementations & dependencies

if(PLATFORM_OSX)
  find_library(CORE_SERVICES_LIBRARY CoreServices REQUIRED)
  mark_as_advanced(CORE_SERVICES_LIBRARY)

  find_library(CORE_FOUNDATION_LIBRARY CoreFoundation REQUIRED)
  mark_as_advanced(CORE_FOUNDATION_LIBRARY)
endif(PLATFORM_OSX)

# Add and link the library
add_library(nomlib-file
  ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
)

# CMake generates our NOM_EXPORT macros, i.e.: __declspec(dllexport)
generate_export_header(nomlib-file
  BASE_NAME
    NOM
  EXPORT_FILE_NAME
    export.hpp
)

if(APPLE)
  target_sources(nomlib-file
    PRIVATE
    ${CORE_SERVICES_LIBRARY}
    ${CORE_FOUNDATION_LIBRARY}
  )
endif(APPLE)

if(UNIX)
  target_sources(nomlib-file
    PRIVATE
    # TODO: Relocate to file sub-directory
    ${SRC_DIR}/system/unix/UnixFile.cpp
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${HDR_BASE}
      FILES
      ${HDR_BASE}/system/unix/UnixFile.hpp
  )
endif(UNIX)

if(WIN32)
  target_sources(nomlib-file
    PRIVATE
    # TODO: Relocate to file sub-directory
    ${SRC_DIR}/system/windows/WinFile.cpp
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${HDR_BASE}
      FILES
      ${HDR_BASE}/system/windows/WinFile.hpp
  )
endif(WIN32)

set_target_properties(nomlib-file
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSION_REQUIRED OFF
)

if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
  # Skip dllimport/dllexport logic when building as a static library target
  target_compile_definitions(nomlib-file
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-file
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

# Base source tree
target_sources(nomlib-file
  PRIVATE
  ${SRC_DIR}/system/Path.cpp
  ${SRC_DIR}/system/File.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    # TODO: Relocate to file sub-directory
    # ${HDR_BASE}/file.hpp
    ${HDR_BASE}/system/Path.hpp
    ${HDR_BASE}/system/File.hpp
    ${HDR_BASE}/system/IFile.hpp
    FILE_SET generated_headers
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    FILES ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
)

target_include_directories(nomlib-file
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  # FIXME:
  ${SDL2_INCLUDE_DIR}
)

target_link_libraries(nomlib-file
  PUBLIC
  nomlib-core
)

# MacOSX Framework bundle support
set_target_properties(nomlib-file
  PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST
      "${CMAKE_TEMPLATE_PATH}/Info.plist.in"
    MACOSX_FRAMEWORK_NAME
      nomlib-file
    MACOSX_FRAMEWORK_BUNDLE_VERSION
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_FRAMEWORK_IDENTIFIER
      "net.i8degrees.nomlib-file"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
      ""
    MACOSX_RPATH TRUE
)

# MacOSX framework bundle support - public header files
if(PLATFORM_OSX)
  install(TARGETS nomlib-file
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-file.framework/Headers"
      COMPONENT Development
    FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-file.framework/Headers/file"
      COMPONENT Development
  )
endif(PLATFORM_OSX)

install(TARGETS nomlib-file
  EXPORT libtarget
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT Development
  FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/file"
    COMPONENT Development
  # MacOSX framework bundle
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
)

