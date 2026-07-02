# nomlib-serializers module tests

set( NOM_BUILD_SERIALIZERS_JSON_TESTS ON )
set( NOM_BUILD_SERIALIZERS_XML_TESTS ON )
set( NOM_BUILD_JSON_CONFIG_FILE_TESTS ON )
set( NOM_BUILD_SERIALIZERS_HTML_TESTS ON )

#if( PLATFORM_WINDOWS )
  #list( APPEND NOM_SERIALIZER_TESTS_DEPS ${SDL2MAIN_LIBRARY} )
#endif( PLATFORM_WINDOWS )

if( NOM_BUILD_SERIALIZERS_JSON_TESTS )

  add_executable(JsonCppSerializerTest
    WIN32
    common.cpp
    JsonCppSerializerTest.cpp
  )

  add_dependencies(JsonCppSerializerTest gtest)

  target_include_directories(JsonCppSerializerTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
    ${GTEST_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(JsonCppSerializerTest
    PUBLIC
    nomlib-serializers
    # FIXME: just needs init
    nomlib-system
    PRIVATE
    Threads::Threads
    ${GTEST_LIBRARY}
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/JsonCppSerializerTest
                    "" # args
                    "JsonCppSerializerTest.cpp"
                  )

  add_executable(JsonCppDeserializerTest
    WIN32
    common.cpp
    JsonCppDeserializerTest.cpp
  )

  add_dependencies(JsonCppDeserializerTest gtest)

  target_include_directories(JsonCppDeserializerTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
    ${GTEST_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(JsonCppDeserializerTest
    PUBLIC
    nomlib-serializers
    # FIXME: Just needs init...
    nomlib-system
    Threads::Threads
    ${GTEST_LIBRARY}
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/JsonCppDeserializerTest
                    "" # args
                    "JsonCppDeserializerTest.cpp" )

endif( NOM_BUILD_SERIALIZERS_JSON_TESTS )

if( NOM_BUILD_SERIALIZERS_XML_TESTS )

  add_executable(RapidXmlSerializerTest
    WIN32
    common.cpp
    RapidXmlSerializerTest.cpp
  )

  add_dependencies(RapidXmlSerializerTest gtest)

  target_include_directories(RapidXmlSerializerTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
    ${GTEST_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(RapidXmlSerializerTest
    PUBLIC
    nomlib-serializers
    # FIXME: Just needs init...
    nomlib-system
    Threads::Threads
    ${GTEST_LIBRARY}
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/RapidXmlSerializerTest
                    "" # args
                    "RapidXmlSerializerTest.cpp" )

  add_executable(RapidXmlDeserializerTest
    WIN32
    common.cpp
    RapidXmlDeserializerTest.cpp
  )

  add_dependencies(RapidXmlDeserializerTest gtest)

  target_include_directories(RapidXmlDeserializerTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
    ${GTEST_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(RapidXmlDeserializerTest
    PUBLIC
    nomlib-serializers
    # FIXME: Just needs init...
    nomlib-system
    Threads::Threads
    ${GTEST_LIBRARY}
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/RapidXmlDeserializerTest
                    "" # args
                    "RapidXmlDeserializerTest.cpp" )

endif( NOM_BUILD_SERIALIZERS_XML_TESTS )

# Copy resources file for unit tests if it has been built.
if( NOM_BUILD_SERIALIZERS_JSON_TESTS )
  # Search paths to use in resolving the full resources path
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/serializers/JsonCppSerializerTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )
endif( NOM_BUILD_SERIALIZERS_JSON_TESTS )

# Copy resources file for unit tests if it has been built.
if( NOM_BUILD_SERIALIZERS_XML_TESTS )
  # Search paths to use in resolving the full resources path
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/serializers/RapidXmlSerializerTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )
endif( NOM_BUILD_SERIALIZERS_XML_TESTS )

# TODO: Use resources cfg file
if( NOM_BUILD_JSON_CONFIG_FILE_TESTS )

  add_executable(JsonConfigFileTest
    WIN32
    JsonConfigFileTest.cpp
  )

  add_dependencies(JsonConfigFileTest gtest)

  target_include_directories(JsonConfigFileTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
    # PRIVATE
    ${GTEST_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(JsonConfigFileTest
    PUBLIC
    nomlib-serializers
    # FIXME: just needs init
    nomlib-system
    PRIVATE
    Threads::Threads
    ${GTEST_LIBRARY}
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/JsonConfigFileTest
                    "" # args
                    "JsonConfigFileTest.cpp" )

  install (
            DIRECTORY
            "${NOM_TESTS_RESOURCES_DIR}/json"
            DESTINATION
            "${TESTS_INSTALL_DIR}/Resources"
            PATTERN ".*" EXCLUDE
          )

endif( NOM_BUILD_JSON_CONFIG_FILE_TESTS )

if( NOM_BUILD_SERIALIZERS_HTML_TESTS )

  add_executable(MiniHTMLTest
    WIN32
    MiniHTMLTest.cpp
  )

  add_dependencies(MiniHTMLTest gtest)

  target_include_directories(MiniHTMLTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
    ${GTEST_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
  )

  # NOTE: We don't actually need nomlib-system here
  target_link_libraries(MiniHTMLTest
    PUBLIC
    nomlib-serializers
    # FIXME: we only need init from here
    nomlib-system
    PRIVATE
    Threads::Threads
    ${GTEST_LIBRARY}
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/MiniHTMLTest
                    "" # args
                    "MiniHTMLTest.cpp" )

endif( NOM_BUILD_SERIALIZERS_HTML_TESTS )
