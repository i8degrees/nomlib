# Unit testing with Google Test
#
# NOTE: Ensure that when you use the GTEST_ADD_TESTS macro, that you supply the
# necessary arguments -- name of test(s) to run -- otherwise you may notice
# CTest skipping the tests all together!
#
# NOTE: You may need to issue a 'make rebuild_cache' after changing test
# fixture names, in order to get 'make test' to recognize the modified test
# code.
#

if( NOM_BUILD_PTREE_UNIT )
  set( NOM_BUILD_PTREE_TESTS on )
endif( NOM_BUILD_PTREE_UNIT )

if( NOM_BUILD_JSON_UNIT )
  set( NOM_BUILD_PTREE_JSON_TESTS on )
endif( NOM_BUILD_JSON_UNIT )

if( NOM_BUILD_XML_UNIT )
  set( NOM_BUILD_PTREE_XML_TESTS on )
endif( NOM_BUILD_XML_UNIT )

if( NOM_BUILD_SYSTEM_UNIT )
  set( NOM_BUILD_SYSTEM_TESTS on )
endif( NOM_BUILD_SYSTEM_UNIT )

# Path under Windows should resolve to: build/tests/Debug
set ( TESTS_DEPS_INSTALL_PEFIX
      "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
    )

# NOTE: we set GTEST_ROOT in the dependencies section of our build script.
find_package( GTest REQUIRED )

include_directories( ${GTEST_INCLUDE_DIRS} )

if( NOM_BUILD_PTREE_TESTS )

  add_executable  (
                    ptree
                    ${NOM_TESTS_DIR}/ptree/ptree.cpp
                  )

  target_link_libraries ( ptree
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    ptree
                    "values"
                    ${NOM_TESTS_DIR}/ptree/ptree.cpp
                  )

endif( NOM_BUILD_PTREE_TESTS )

# Additional Header file dependencies
include_directories( "${NOM_TESTS_DIR}/serializers" )

if( NOM_BUILD_PTREE_JSON_TESTS )

  add_executable  (
                    json_serializer
                    ${NOM_TESTS_DIR}/serializers/JsonCppSerializerTest.cpp
                    ${NOM_TESTS_DIR}/serializers/common.cpp
                  )

  target_link_libraries ( json_serializer
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

endif( NOM_BUILD_PTREE_JSON_TESTS )

if( NOM_BUILD_PTREE_XML_TESTS )

add_executable  (
                  xml_serializer
                  ${NOM_TESTS_DIR}/serializers/RapidXmlSerializerTest.cpp
                  ${NOM_TESTS_DIR}/serializers/common.cpp
                )

target_link_libraries ( xml_serializer
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

target_link_libraries ( xml_deserializer
                        ${GTEST_BOTH_LIBRARIES}
                        ${PROJECT_NAME} # nomlib
                      )

GTEST_ADD_TESTS (
                  xml_deserializer
                  "RapidXmlDeserializer"
                  ${NOM_TESTS_DIR}/serializers/RapidXmlDeserializerTest.cpp
                )

endif( NOM_BUILD_PTREE_XML_TESTS )

if( NOM_BUILD_SYSTEM_TESTS )

  add_executable  (
                    font_cache
                    ${NOM_TESTS_DIR}/system/FontCacheTest.cpp
                  )

  target_link_libraries (
                          font_cache
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    font_cache
                    "FontCacheTest"
                    ${NOM_TESTS_DIR}/system/FontCacheTest.cpp
                  )

endif( NOM_BUILD_SYSTEM_TESTS )

if( NOM_BUILD_TEMPLATES_UNIT )

  # Resources/SharedSupport/UnitTestTemplate.cpp

  add_executable  (
                    UnitTestTemplate
                    "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/UnitTestTemplate.cpp"
                  )

  target_link_libraries ( UnitTestTemplate
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    Template
                    "TemplateTestDesc"
                    "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/UnitTestTemplate.cpp"
                  )

endif( NOM_BUILD_TEMPLATES_UNIT )

if( NOM_BUILD_JSON_UNIT )
  install ( DIRECTORY
            "${NOM_EXAMPLES_RESOURCES_DIR}/json"
            DESTINATION
            "${CMAKE_CURRENT_BINARY_DIR}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )
endif( NOM_BUILD_JSON_UNIT )

if( NOM_BUILD_XML_UNIT )
  install ( DIRECTORY
            "${NOM_EXAMPLES_RESOURCES_DIR}/xml"
            DESTINATION
            "${CMAKE_CURRENT_BINARY_DIR}/Resources"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )
endif( NOM_BUILD_XML_UNIT )

# Install library dependencies into tests output directory so we can always
# execute the binaries with the proper dependency versions.
if ( PLATFORM_WINDOWS )
  install ( DIRECTORY
            "${SDL2_LIBRARY_DIR}"
            "${SDL2_IMAGE_LIBRARY_DIR}"
            "${SDL2_TTF_LIBRARY_DIR}"
            "${OPENAL_LIBRARY_DIR}"
            "${LIBSNDFILE_LIBRARY_DIR}"
            ${MSVCPP_LIBRARY_REDIST}
            "${GTEST_ROOT}/lib/"        # GTest re-dist libs
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
            FILES_MATCHING PATTERN "*.dll"
          )
endif ( PLATFORM_WINDOWS )
