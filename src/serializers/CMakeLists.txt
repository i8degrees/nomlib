# nomlib-serializers library

# Platform-specific implementations & dependencies

# Add and link the library
add_library(nomlib-serializers
  ${LIBRARY_OUTPUT_TYPE}
)

set_target_properties(nomlib-serializers
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
    MAP_IMPORTED_CONFIG_MINSIZEREL Release
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO Debug
)

if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
  # Skip dllimport/dllexport logic when building as a static library target
  target_compile_definitions(nomlib-serializers
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-serializers
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

target_sources(nomlib-serializers
  PRIVATE
  ${SRC_DIR}/serializers/IConfigFile.cpp
  ${SRC_DIR}/serializers/JsonConfigFile.cpp
  ${SRC_DIR}/serializers/JsonCppDeserializer.cpp
  ${SRC_DIR}/serializers/JsonCppSerializer.cpp
  ${SRC_DIR}/serializers/MiniHTML.cpp
  ${SRC_DIR}/serializers/RapidXmlDeserializer.cpp
  ${SRC_DIR}/serializers/RapidXmlSerializer.cpp
  ${SRC_DIR}/serializers/SearchPath.cpp
  # jsoncpp v0.7.0
  ${JSONCPP_SOURCE_DIR}/jsoncpp.cpp
  # RapidXml by discord - v1.x
  ${RAPIDXML_INCLUDE_DIR}/rapidxml.hpp
  ${RAPIDXML_INCLUDE_DIR}/rapidxml_iterators.hpp
  ${RAPIDXML_INCLUDE_DIR}/rapidxml_print.hpp
  ${RAPIDXML_INCLUDE_DIR}/rapidxml_utils.hpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    ${HDR_BASE}/serializers.hpp
    ${HDR_BASE}/serializers/IConfigFile.hpp
    ${HDR_BASE}/serializers/JsonConfigFile.hpp
    ${HDR_BASE}/serializers/JsonCppDeserializer.hpp
    ${HDR_BASE}/serializers/JsonCppSerializer.hpp
    ${HDR_BASE}/serializers/IValueSerializer.hpp
    ${HDR_BASE}/serializers/IValueDeserializer.hpp
    ${HDR_BASE}/serializers/MiniHTML.hpp
    ${HDR_BASE}/serializers/RapidXmlDeserializer.hpp
    ${HDR_BASE}/serializers/RapidXmlSerializer.hpp
    ${HDR_BASE}/serializers/SearchPath.hpp
    ${HDR_BASE}/serializers/serializers_config.hpp
)

target_include_directories(nomlib-serializers
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${SDL2_INCLUDE_DIR} # SDL.h
  ${JSONCPP_INCLUDE_DIR} # Jsoncpp v0.7.0 - headers only
  ${RAPIDXML_INCLUDE_DIR} # RapidXML v1.x - headers only
)

target_compile_features(nomlib-serializers
  PRIVATE
    cxx_std_14
)

target_link_libraries(nomlib-serializers
  PUBLIC
    nomlib-core
    nomlib-file
    nomlib-ptree
    # FIXME: Once nomlib-file is a complete library target, we can remove this dep!
    nomlib-system
  PRIVATE
    project_warnings
)

# MacOSX Framework bundle support
apple_setup_framework(nomlib-serializers
  "${CMAKE_TEMPLATE_PATH}/framework.plist.in"
)

# MacOSX Framework bundle support - public header files
apple_install_framework_headers(nomlib-serializers)

if(NOT CMAKE_FRAMEWORK)
  install(TARGETS nomlib-serializers
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

