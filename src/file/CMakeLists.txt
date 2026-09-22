# nomlib-file library

# !! This is an unfinished library target

# Platform-specific implementations & dependencies

if(APPLE)
  find_library(CORE_SERVICES_LIBRARY CoreServices REQUIRED)
  mark_as_advanced(CORE_SERVICES_LIBRARY)

  find_library(CORE_FOUNDATION_LIBRARY CoreFoundation REQUIRED)
  mark_as_advanced(CORE_FOUNDATION_LIBRARY)
endif(APPLE)

# Add and link the library
add_library(nomlib-file
  ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
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
    MAP_IMPORTED_CONFIG_MINSIZEREL Release
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO Debug
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
)

target_include_directories(nomlib-file
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  # FIXME:
  ${SDL2_INCLUDE_DIR}
)

target_compile_features(nomlib-file
  PRIVATE
    cxx_std_14
)

target_link_libraries(nomlib-file
  PUBLIC
    nomlib-core
  PRIVATE
    project_warnings
)

# MacOSX Framework bundle support
apple_setup_framework(nomlib-file
  "${CMAKE_TEMPLATE_PATH}/framework.plist.in"
)

# MacOSX Framework bundle support - public header files
apple_install_framework_headers(nomlib-file)

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
  # MacOSX framework bundle
  #FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    #COMPONENT Runtime
)
