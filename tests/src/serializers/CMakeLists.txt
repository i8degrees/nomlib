if( NOM_BUILD_SERIALIZERS_JSON_TESTS )

  add_executable  (
                    json_serializer
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonCppSerializerTest.cpp
                    ${NOM_TESTS_SOURCE_DIR}/serializers/common.cpp
                  )

  target_link_libraries( json_serializer UnitTest )

  GTEST_ADD_TESTS (
                    json_serializer
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonCppSerializerTest.cpp
                  )

  add_executable  (
                    json_deserializer
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonCppDeserializerTest.cpp
                    ${NOM_TESTS_SOURCE_DIR}/serializers/common.cpp
                  )


  target_link_libraries( json_deserializer UnitTest )

  GTEST_ADD_TESTS (
                    json_deserializer
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonCppDeserializerTest.cpp
                  )

endif( NOM_BUILD_SERIALIZERS_JSON_TESTS )

if( NOM_BUILD_SERIALIZERS_XML_TESTS )

  add_executable  (
                    xml_serializer
                    ${NOM_TESTS_SOURCE_DIR}/serializers/RapidXmlSerializerTest.cpp
                    ${NOM_TESTS_SOURCE_DIR}/serializers/common.cpp
                  )

  target_link_libraries( xml_serializer UnitTest )

  GTEST_ADD_TESTS (
                    xml_serializer
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/serializers/RapidXmlSerializerTest.cpp
                  )

  add_executable  (
                    xml_deserializer
                    ${NOM_TESTS_SOURCE_DIR}/serializers/RapidXmlDeserializerTest.cpp
                    ${NOM_TESTS_SOURCE_DIR}/serializers/common.cpp
                  )

  target_link_libraries( xml_deserializer UnitTest )

  GTEST_ADD_TESTS (
                    xml_deserializer
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
                    json_config_file
                    ${NOM_TESTS_SOURCE_DIR}/serializers/JsonConfigFileTest.cpp
                    # ${NOM_TESTS_SOURCE_DIR}/serializers/common.cpp
                  )

  target_link_libraries( json_config_file UnitTest )

  GTEST_ADD_TESTS (
                    json_config_file
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

  target_link_libraries( MiniHTMLTest UnitTest )

  GTEST_ADD_TESTS (
                    MiniHTMLTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/serializers/MiniHTMLTest.cpp
                  )

endif( NOM_BUILD_SERIALIZERS_HTML_TESTS )
