# nomlib-math library

# Platform-specific implementations & dependencies

# Add and link the library
add_library(nomlib-math
  ${LIBRARY_OUTPUT_TYPE}
)

# Let CMake generate our NOM_EXPORT macros, i.e.: __declspec(dllexport) for
# symbol visibility on WIN32 (to build a shared, AKA DLL library).
#
# We need only to do this once per project, as opposed to once per library
# target as previously implemented. One of the nasty side effects of doing it
# once per library target was that clang had to pass two include paths
# *per target*; the regular plus the special path of where export.hpp is
# located at build time, making a mess of things.
generate_export_header(nomlib-math
  BASE_NAME
    NOM
  EXPORT_FILE_NAME
    export.hpp
)

set_target_properties(nomlib-math
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
)

if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
  # Skip dllimport/dllexport logic when building as a static library target
  target_compile_definitions(nomlib-math
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-math
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

target_sources(nomlib-math
  PRIVATE
  #${SRC_DIR}/math/Color4.cpp
  ${SRC_DIR}/math/math_helpers.cpp
  ${SRC_DIR}/math/Point2.cpp
  #${SRC_DIR}/math/Point3.cpp
  ${SRC_DIR}/math/Rect.cpp
  ${SRC_DIR}/math/Size2.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    ${HDR_BASE}/math.hpp
    ${HDR_BASE}/math/Color4.hpp
    ${HDR_BASE}/math/Color4.inl
    ${HDR_BASE}/math/math_helpers.hpp
    ${HDR_BASE}/math/Point2.hpp
    #${HDR_BASE}/math/Point2.inl
    ${HDR_BASE}/math/Point3.hpp
    ${HDR_BASE}/math/Point3.inl
    ${HDR_BASE}/math/Rect.hpp
    ${HDR_BASE}/math/Size2.hpp
    FILE_SET generated_headers
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    FILES ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
)

target_include_directories(nomlib-math
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${SDL2_INCLUDE_DIR} # FIXME
)

target_compile_features(nomlib-math
  PRIVATE
    cxx_std_14
)

target_link_libraries(nomlib-math
  PUBLIC
    nomlib-core
  PRIVATE
    project_warnings
)

# MacOSX Framework bundle support
set_target_properties(nomlib-math
  PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST
      "${CMAKE_TEMPLATE_PATH}/framework.plist.in"
    MACOSX_FRAMEWORK_NAME
      nomlib-math
    MACOSX_FRAMEWORK_BUNDLE_VERSION
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_FRAMEWORK_IDENTIFIER
      "net.i8degrees.nomlib-math"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
      ""
    MACOSX_RPATH TRUE
    #INSTALL_RPATH
      #@executable_path/../Frameworks
)

# MacOSX Framework bundle support - public header files
if(APPLE)
  install(TARGETS nomlib-math
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-math.framework/Headers"
      COMPONENT Development
  )
endif(APPLE)

install(TARGETS nomlib-math
  EXPORT libtarget
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT Development
  FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT Development
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
)
