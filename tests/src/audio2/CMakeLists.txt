# nomlib-audio2 module tests

if(NOM_BUILD_AUDIO2_TESTS)

  add_executable(Audio2Test
    WIN32
    Audio2Test.cpp
  )

  add_dependencies(Audio2Test gtest)

  if(TARGET SDL2::SDL2main)
    target_link_libraries(Audio2Test
      PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    )
  endif(TARGET SDL2::SDL2main)

  target_include_directories(Audio2Test
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include       # include/nomlib
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
      ${OPENAL_INCLUDE_DIR}
  )

  target_link_libraries(Audio2Test
    PUBLIC
      nomlib-audio2
      nomlib-core
      nomlib-math
      nomlib-graphics
      nomlib-system
    PRIVATE
      project_warnings
      Threads::Threads
      ${GTEST_LIBRARY}
      $<IF:$<TARGET_EXISTS:OpenAL::OpenAL>,OpenAL::OpenAL,OpenAL::OpenAL-static>
  )

  target_compile_definitions(Audio2Test
    PRIVATE
      NOM_PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
  )

  GTEST_ADD_TESTS(${TESTS_INSTALL_DIR}/Audio2Test "" Audio2Test.cpp)

  # Copy resource metadata for the test
  install_resource_file(
    "${NOM_TESTS_RESOURCES_DIR}/audio/ALAudioTest.json"
    "${TESTS_INSTALL_DIR}"
  )

endif(NOM_BUILD_AUDIO2_TESTS)
