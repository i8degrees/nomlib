if( NOM_BUILD_SERIALIZERS_JSON_TESTS )

  add_executable  (
                    JsonCppSerializerTest
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonCppSerializerTest.cpp
                    ${NOM_TESTS_SOURCE_DIR}/serializers/common.cpp
                  )

  target_link_libraries( JsonCppSerializerTest ${NOM_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    JsonCppSerializerTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonCppSerializerTest.cpp
                  )

  add_executable  (
                    JsonCppDeserializerTest
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonCppDeserializerTest.cpp
                    ${NOM_TESTS_SOURCE_DIR}/serializers/common.cpp
                  )


  target_link_libraries( JsonCppDeserializerTest ${NOM_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    JsonCppDeserializerTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonCppDeserializerTest.cpp
                  )

endif( NOM_BUILD_SERIALIZERS_JSON_TESTS )

if( NOM_BUILD_SERIALIZERS_XML_TESTS )

  add_executable  (
                    RapidXmlSerializerTest
                    ${NOM_TESTS_SOURCE_DIR}/serializers/RapidXmlSerializerTest.cpp
                    ${NOM_TESTS_SOURCE_DIR}/serializers/common.cpp
                  )

  target_link_libraries( RapidXmlSerializerTest ${NOM_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    RapidXmlSerializerTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/serializers/RapidXmlSerializerTest.cpp
                  )

  add_executable  (
                    RapidXmlDeserializerTest
                    ${NOM_TESTS_SOURCE_DIR}/serializers/RapidXmlDeserializerTest.cpp
                    ${NOM_TESTS_SOURCE_DIR}/serializers/common.cpp
                  )

  target_link_libraries( RapidXmlDeserializerTest ${NOM_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    RapidXmlDeserializerTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/serializers/RapidXmlDeserializerTest.cpp
                  )

endif( NOM_BUILD_SERIALIZERS_XML_TESTS )

# Copy resources file for unit tests if it has been built.
if( NOM_BUILD_SERIALIZERS_JSON_TESTS )
    # Search paths to use in resolving the full resources path
    install (
              FILES
              "${NOM_TESTS_RESOURCES_DIR}/serializers/JsonCppSerializerTest.json"
              DESTINATION
              "${TESTS_DEPS_INSTALL_PEFIX}"
            )
endif( NOM_BUILD_SERIALIZERS_JSON_TESTS )

# Copy resources file for unit tests if it has been built.
if( NOM_BUILD_SERIALIZERS_XML_TESTS )
    # Search paths to use in resolving the full resources path
    install (
              FILES
              "${NOM_TESTS_RESOURCES_DIR}/serializers/RapidXmlSerializerTest.json"
              DESTINATION
              "${TESTS_DEPS_INSTALL_PEFIX}"
            )
endif( NOM_BUILD_SERIALIZERS_XML_TESTS )

# TODO: Use resources cfg file
if( NOM_BUILD_JSON_CONFIG_FILE_TESTS )

  add_executable  (
                    JsonConfigFileTest
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonConfigFileTest.cpp
                    # ${NOM_TESTS_SOURCE_DIR}/serializers/common.cpp
                  )

  target_link_libraries( JsonConfigFileTest ${NOM_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    JsonConfigFileTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonConfigFileTest.cpp
                  )

  install (
            DIRECTORY
            "${NOM_TESTS_RESOURCES_DIR}/json"
            DESTINATION
            # "${CMAKE_CURRENT_BINARY_DIR}/Resources"
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

endif( NOM_BUILD_JSON_CONFIG_FILE_TESTS )

if( NOM_BUILD_SERIALIZERS_HTML_TESTS )

  add_executable  (
                    MiniHTMLTest
                    ${NOM_TESTS_SOURCE_DIR}/serializers/MiniHTMLTest.cpp
                  )

  target_link_libraries( MiniHTMLTest ${NOM_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    MiniHTMLTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/serializers/MiniHTMLTest.cpp
                  )

endif( NOM_BUILD_SERIALIZERS_HTML_TESTS )
