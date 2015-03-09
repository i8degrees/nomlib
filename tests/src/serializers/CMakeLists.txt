# nomlib-serializers module tests

set( NOM_BUILD_SERIALIZERS_JSON_TESTS ON )
set( NOM_BUILD_SERIALIZERS_XML_TESTS ON )
set( NOM_BUILD_JSON_CONFIG_FILE_TESTS ON )
set( NOM_BUILD_SERIALIZERS_HTML_TESTS ON )

if( EXISTS "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
  include( "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
endif()

set(  NOM_SERIALIZER_TESTS_DEPS ${GTEST_LIBRARY} nomlib-serializers
      # FIXME: Just needs init...
      nomlib-system
)

if( PLATFORM_WINDOWS )
  list( APPEND NOM_SERIALIZER_TESTS_DEPS ${SDL2MAIN_LIBRARY} )
endif( PLATFORM_WINDOWS )

if( NOM_BUILD_SERIALIZERS_JSON_TESTS )

  add_executable( JsonCppSerializerTest
                  "JsonCppSerializerTest.cpp" "common.cpp" )

  target_link_libraries( JsonCppSerializerTest ${NOM_SERIALIZER_TESTS_DEPS} )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/JsonCppSerializerTest
                    "" # args
                    "JsonCppSerializerTest.cpp"
                  )

  add_executable( JsonCppDeserializerTest
                  "JsonCppDeserializerTest.cpp"
                  "common.cpp" )


  target_link_libraries( JsonCppDeserializerTest ${NOM_SERIALIZER_TESTS_DEPS} )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/JsonCppDeserializerTest
                    "" # args
                    "JsonCppDeserializerTest.cpp" )

endif( NOM_BUILD_SERIALIZERS_JSON_TESTS )

if( NOM_BUILD_SERIALIZERS_XML_TESTS )

  add_executable( RapidXmlSerializerTest
                  "RapidXmlSerializerTest.cpp" "common.cpp" )

  target_link_libraries( RapidXmlSerializerTest ${NOM_SERIALIZER_TESTS_DEPS} )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/RapidXmlSerializerTest
                    "" # args
                    "RapidXmlSerializerTest.cpp" )

  add_executable( RapidXmlDeserializerTest
                  "RapidXmlDeserializerTest.cpp"
                  "common.cpp" )

  target_link_libraries( RapidXmlDeserializerTest ${NOM_SERIALIZER_TESTS_DEPS} )

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

  add_executable( JsonConfigFileTest
                  "JsonConfigFileTest.cpp" )

  target_link_libraries( JsonConfigFileTest ${NOM_SERIALIZER_TESTS_DEPS} )

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

  add_executable( MiniHTMLTest
                  "MiniHTMLTest.cpp" )

  # NOTE: We don't actually need nomlib-system here
  target_link_libraries( MiniHTMLTest ${NOM_SERIALIZER_TESTS_DEPS} )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/MiniHTMLTest
                    "" # args
                    "MiniHTMLTest.cpp" )

endif( NOM_BUILD_SERIALIZERS_HTML_TESTS )
