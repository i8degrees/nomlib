# nomlib-core library

set(  NOM_CORE_SOURCE
      ${SRC_DIR}/platforms.cpp
      ${SRC_DIR}/version.cpp
      ${SRC_DIR}/revision.cpp # auto-generated
      ${SRC_DIR}/core/strings.cpp
      ${SRC_DIR}/core/clock.cpp
      ${SRC_DIR}/core/ObjectTypeInfo.cpp
      ${SRC_DIR}/core/IObject.cpp
      ${SRC_DIR}/core/ConsoleOutput.cpp
      ${SRC_DIR}/core/SDL2Logger.cpp
      ${SRC_DIR}/core/SDL_assertion_helpers.cpp
      ${SRC_DIR}/core/err.cpp
)

set(NOM_CORE_HDR
  ${HDR_BASE}/config.hpp     # auto-generated
  ${HDR_BASE}/core.hpp
  ${HDR_BASE}/core/clock.hpp
  ${HDR_BASE}/core/ConsoleOutput.hpp
  ${HDR_BASE}/core/err.hpp
  ${HDR_BASE}/core/IObject.hpp
  ${HDR_BASE}/core/ObjectTypeInfo.hpp
  ${HDR_BASE}/core/SDL_assertion_helpers.hpp
  ${HDR_BASE}/core/SDL2Logger.hpp
  ${HDR_BASE}/core/strings.hpp
  ${HDR_BASE}/core/unique_ptr.hpp
  ${HDR_BASE}/macros.hpp
  ${HDR_BASE}/platforms.hpp
  ${HDR_BASE}/revision.hpp   # auto-generated
  ${HDR_BASE}/types.hpp
  ${HDR_BASE}/version.hpp    # auto-generated
)

# Add and link the library
add_library(nomlib-core
  ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
)

# CMake generates our NOM_EXPORT macros, i.e.: __declspec(dllexport)
generate_export_header(nomlib-core
  BASE_NAME
    NOM
  EXPORT_FILE_NAME
    export.hpp
)

# Platform-specific implementations & dependencies

# BSD, OS X && Linux
if(NOM_PLATFORM_POSIX)
  target_sources(nomlib-core
    PRIVATE
      ${SRC_DIR}/core/UnixConsoleOutput.cpp
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${HDR_BASE}
      FILES
      ${HDR_BASE}/core/UnixConsoleOutput.hpp
  )
endif(NOM_PLATFORM_POSIX)

if(PLATFORM_WINDOWS)
  target_sources(nomlib-core
    PRIVATE
      ${SRC_DIR}/core/WindowsConsoleOutput.cpp
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${HDR_BASE}
      FILES
      ${HDR_BASE}/core/WindowsConsoleOutput.hpp
  )
endif(PLATFORM_WINDOWS)

# base unit
target_sources(nomlib-core
  PRIVATE
  ${NOM_CORE_SOURCE}
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    ${NOM_CORE_HDR}
    FILE_SET generated_headers
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    FILES ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
)

# This is embedded as the SONAME in the library's ELF header and determinnes
# which file the dynamic linker searches for at runtime, i.e.:
# `libnomlib-actions.so.1 -> `libnomlib-actions.so.0.11`
# The Application Binary Interface (ABI) version; PATCH level versions are
# intended **not** to break the ABI version.
set_target_properties(nomlib-core
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
  target_compile_definitions(nomlib-core
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-core
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

target_include_directories(nomlib-core
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${SDL2_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
)

target_link_libraries(nomlib-core
  PRIVATE
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

# MacOSX Framework bundle support
set_target_properties(nomlib-core
  PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST
      "${CMAKE_TEMPLATE_PATH}/Info.plist.in"
    MACOSX_FRAMEWORK_NAME
      nomlib-core
    MACOSX_FRAMEWORK_BUNDLE_VERSION
      "${PROJECT_VERSION}-${CMAKE_BUILD_TYPE}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING
      "${PROJECT_VERSION}"
    MACOSX_FRAMEWORK_IDENTIFIER
      "net.i8degrees.nomlib-core"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
      ""
    MACOSX_RPATH TRUE
    #INSTALL_RPATH
      #@executable_path/../Frameworks
)

# MacOSX Framework bundle support - public header files
if(PLATFORM_OSX)
  install(TARGETS nomlib-core
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-core.framework/Headers"
      COMPONENT Development
    FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-core.framework/Headers/core"
      COMPONENT Development
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
  )
endif(PLATFORM_OSX)

install(TARGETS nomlib-core
  EXPORT libtarget
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT Development
  FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/core"
    COMPONENT Development
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
)

