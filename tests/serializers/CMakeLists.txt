set( NOM_BUILD_SERIALIZERS_JSON_TESTS off )
set( NOM_BUILD_SERIALIZERS_XML_TESTS off )
set( NOM_BUILD_JSON_CONFIG_FILE_TESTS off )

if( NOM_BUILD_SERIALIZERS_JSON_TESTS )

  add_executable  (
                    json_serializer
                    ${NOM_TESTS_DIR}/serializers/JsonCppSerializerTest.cpp
                    ${NOM_TESTS_DIR}/serializers/common.cpp
                  )

  target_link_libraries (
                          json_serializer
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    json_serializer
                    "JsonCppSerializerTest"
                    ${NOM_TESTS_DIR}/serializers/JsonCppSerializerTest.cpp
                  )

  add_executable  (
                    json_deserializer
                    ${NOM_TESTS_DIR}/serializers/JsonCppDeserializerTest.cpp
                    ${NOM_TESTS_DIR}/serializers/common.cpp
                  )


  target_link_libraries (
                          json_deserializer
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    json_deserializer
                    "JsonCppDeserializerTest"
                    ${NOM_TESTS_DIR}/serializers/JsonCppDeserializerTest.cpp
                  )

endif( NOM_BUILD_SERIALIZERS_JSON_TESTS )

if( NOM_BUILD_SERIALIZERS_XML_TESTS )

  add_executable  (
                    xml_serializer
                    ${NOM_TESTS_DIR}/serializers/RapidXmlSerializerTest.cpp
                    ${NOM_TESTS_DIR}/serializers/common.cpp
                  )

  target_link_libraries (
                          xml_serializer
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    xml_serializer
                    "RapidXmlSerializer"
                    ${NOM_TESTS_DIR}/serializers/RapidXmlSerializerTest.cpp
                  )

  add_executable  (
                    xml_deserializer
                    ${NOM_TESTS_DIR}/serializers/RapidXmlDeserializerTest.cpp
                    ${NOM_TESTS_DIR}/serializers/common.cpp
                  )

  target_link_libraries (
                          xml_deserializer
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    xml_deserializer
                    "RapidXmlDeserializer"
                    ${NOM_TESTS_DIR}/serializers/RapidXmlDeserializerTest.cpp
                  )

endif( NOM_BUILD_SERIALIZERS_XML_TESTS )

# Copy JSON resources for unit tests only if necessary (built).
if( NOM_BUILD_SERIALIZERS_JSON_TESTS )

  install (
            DIRECTORY
            "${NOM_EXAMPLES_RESOURCES_DIR}/json"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

endif( NOM_BUILD_SERIALIZERS_JSON_TESTS )

# Copy XML resources for unit tests only if necessary (built).
if( NOM_BUILD_SERIALIZERS_XML_TESTS )

  install (
            DIRECTORY
            "${NOM_EXAMPLES_RESOURCES_DIR}/xml"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

endif( NOM_BUILD_SERIALIZERS_XML_TESTS )

if( NOM_BUILD_JSON_CONFIG_FILE_TESTS )

  add_executable  (
                    json_config_file
                    ${NOM_TESTS_DIR}/serializers/JsonConfigFileTest.cpp
                    # ${NOM_TESTS_DIR}/serializers/common.cpp
                  )

  target_link_libraries (
                          json_config_file
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    json_config_file
                    "JsonConfigFileTest"
                    ${NOM_TESTS_DIR}/serializers/JsonConfigFileTest.cpp
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
