# nomlib-serializers module tests

set(NOM_BUILD_SERIALIZERS_JSON_TESTS ON)
set(NOM_BUILD_SERIALIZERS_XML_TESTS ON)
set(NOM_BUILD_JSON_CONFIG_FILE_TESTS ON)
set(NOM_BUILD_SERIALIZERS_HTML_TESTS ON)

if(NOM_BUILD_SERIALIZERS_JSON_TESTS)

  add_executable(JsonCppSerializerTest
    #WIN32 # Link SDL2main over WINMAIN (console apps)
    common.cpp
    JsonCppSerializerTest.cpp
  )

  add_dependencies(JsonCppSerializerTest gtest)

  target_include_directories(JsonCppSerializerTest
    PUBLIC
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
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/JsonCppSerializerTest
                    "" # args
                    "JsonCppSerializerTest.cpp"
                  )

  # Copy resources file for unit tests
  # For development: Search paths to use in resolving the full resources path
  add_custom_command(TARGET JsonCppSerializerTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/serializers/JsonCppSerializerTest.json"
    $<TARGET_FILE_DIR:JsonCppSerializerTest>
  )

  add_executable(JsonCppDeserializerTest
    #WIN32 # Link SDL2main over WINMAIN (console apps)
    common.cpp
    JsonCppDeserializerTest.cpp
  )

  add_dependencies(JsonCppDeserializerTest gtest)

  target_include_directories(JsonCppDeserializerTest
    PUBLIC
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
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/JsonCppDeserializerTest
                    "" # args
                    "JsonCppDeserializerTest.cpp" )

endif(NOM_BUILD_SERIALIZERS_JSON_TESTS)

if(NOM_BUILD_SERIALIZERS_XML_TESTS)

  add_executable(RapidXmlSerializerTest
    #WIN32 # Link SDL2main over WINMAIN (console apps)
    common.cpp
    RapidXmlSerializerTest.cpp
  )

  add_dependencies(RapidXmlSerializerTest gtest)

  target_include_directories(RapidXmlSerializerTest
    PUBLIC
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
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/RapidXmlSerializerTest
                    "" # args
                    "RapidXmlSerializerTest.cpp" )

  add_executable(RapidXmlDeserializerTest
    #WIN32 # Link SDL2main over WINMAIN (console apps)
    common.cpp
    RapidXmlDeserializerTest.cpp
  )

  add_dependencies(RapidXmlDeserializerTest gtest)

  target_include_directories(RapidXmlDeserializerTest
    PUBLIC
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
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/RapidXmlDeserializerTest
                    "" # args
                    "RapidXmlDeserializerTest.cpp" )

  # Copy resources file for unit test
  # For development: Search paths to use in resolving the full resources path
  add_custom_command(TARGET RapidXmlSerializerTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/serializers/RapidXmlSerializerTest.json"
    $<TARGET_FILE_DIR:RapidXmlSerializerTest>
  )
endif(NOM_BUILD_SERIALIZERS_XML_TESTS)

# TODO: Use resources cfg file
if(NOM_BUILD_JSON_CONFIG_FILE_TESTS)

  add_executable(JsonConfigFileTest
    #WIN32 # Link SDL2main over WINMAIN (console apps)
    JsonConfigFileTest.cpp
  )

  add_dependencies(JsonConfigFileTest gtest)

  target_include_directories(JsonConfigFileTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include # include/nomlib
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
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
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/JsonConfigFileTest
                    "" # args
                    "JsonConfigFileTest.cpp" )

  # !! VERIFY
  # For development:
  add_custom_command(TARGET JsonConfigFileTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/json/"
    $<TARGET_FILE_DIR:JsonConfigFileTest>
  )
endif(NOM_BUILD_JSON_CONFIG_FILE_TESTS)

if( NOM_BUILD_SERIALIZERS_HTML_TESTS )

  add_executable(MiniHTMLTest
    #WIN32 # Link SDL2main over WINMAIN (console apps)
    MiniHTMLTest.cpp
  )

  add_dependencies(MiniHTMLTest gtest)

  target_include_directories(MiniHTMLTest
    PUBLIC
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
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/MiniHTMLTest
                    "" # args
                    "MiniHTMLTest.cpp" )

endif(NOM_BUILD_SERIALIZERS_HTML_TESTS)

