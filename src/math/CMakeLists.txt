# nomlib-math library

# Platform-specific implementations & dependencies

# Add and link the library
add_library(nomlib-math
  ${LIBRARY_OUTPUT_TYPE}
)

set_target_properties(nomlib-math
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
    MAP_IMPORTED_CONFIG_MINSIZEREL Release
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO Debug
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
  ${SRC_DIR}/math/Color4.cpp
  ${SRC_DIR}/math/math_helpers.cpp
  ${SRC_DIR}/math/Point2.cpp
  ${SRC_DIR}/math/Point3.cpp
  ${SRC_DIR}/math/Rect.cpp
  ${SRC_DIR}/math/Size2.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    ${HDR_BASE}/math.hpp
    ${HDR_BASE}/math/Color4.hpp
    ${HDR_BASE}/math/math_helpers.hpp
    ${HDR_BASE}/math/Point2.hpp
    ${HDR_BASE}/math/Point3.hpp
    ${HDR_BASE}/math/Rect.hpp
    ${HDR_BASE}/math/Size2.hpp
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
apple_setup_framework(nomlib-math
  "${CMAKE_TEMPLATE_PATH}/framework.plist.in"
  nomlib-math
)

# MacOSX Framework bundle support - public header files
apple_install_framework_headers(nomlib-math)

if(NOT CMAKE_FRAMEWORK)
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
    #FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      #COMPONENT Runtime
  )
endif()
