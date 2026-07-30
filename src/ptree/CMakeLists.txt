# nomlib-ptree library

# Add and link the library
add_library(nomlib-ptree
  ${LIBRARY_OUTPUT_TYPE} # SHARED || STATIC
)

set_target_properties(nomlib-ptree
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
)

if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
  # Skip dllimport/dllexport logic when building as a static library target
  target_compile_definitions(nomlib-ptree
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-ptree
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

target_sources(nomlib-ptree
  PRIVATE
  ${SRC_DIR}/ptree/VString.cpp
  ${SRC_DIR}/ptree/Value.cpp
  ${SRC_DIR}/ptree/ValueConstIterator.cpp
  ${SRC_DIR}/ptree/ValueIterator.cpp
  ${SRC_DIR}/ptree/ValueIteratorBase.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    ${HDR_BASE}/ptree.hpp
    ${HDR_BASE}/ptree/Object.hpp
    ${HDR_BASE}/ptree/VString.hpp
    ${HDR_BASE}/ptree/Value.hpp
    ${HDR_BASE}/ptree/ValueConstIterator.hpp
    ${HDR_BASE}/ptree/ValueIterator.hpp
    ${HDR_BASE}/ptree/ValueIteratorBase.hpp
    ${HDR_BASE}/ptree/ptree_config.hpp
    ${HDR_BASE}/ptree/ptree_forwards.hpp
    ${HDR_BASE}/ptree/ptree_types.hpp
)

target_include_directories(nomlib-ptree
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${SDL2_INCLUDE_DIR}
)

target_compile_features(nomlib-ptree
  PRIVATE
  cxx_std_14
)

target_link_libraries(nomlib-ptree
  PUBLIC
    nomlib-core
  PRIVATE
    project_warnings
)

# MacOSX Framework bundle support
set_target_properties(nomlib-ptree
  PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST
      "${CMAKE_TEMPLATE_PATH}/Info.plist.in"
    MACOSX_FRAMEWORK_NAME
      nomlib-ptree
    MACOSX_FRAMEWORK_BUNDLE_VERSION
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_FRAMEWORK_IDENTIFIER
      "net.i8degrees.nomlib-ptree"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
      ""
    MACOSX_RPATH TRUE
    #INSTALL_RPATH
      #@executable_path/../Frameworks
)

# MacOSX Framework bundle support - public header files
if(APPLE)
  install(TARGETS nomlib-ptree
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-ptree.framework/Headers"
      COMPONENT Development
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
  )
endif(APPLE)

install(TARGETS nomlib-ptree
  EXPORT libtarget
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT Development
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
)
