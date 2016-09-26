# nomlib-audio module tests

set(NOM_BUILD_OPENAL_AUDIO_TESTS ON)

if(NOM_BUILD_OPENAL_AUDIO_TESTS)
  set(TEST_SRC "ALAudioTest.cpp")
  add_executable(ALAudioTest ${TEST_SRC})

  # FIXME: We only need init from nomlib-system
  set(AUDIO_DEPS ${GTEST_LIBRARY} nomlib-core nomlib-math nomlib-audio
      nomlib-system nomlib-serializers)

  if(PLATFORM_WINDOWS)
    list(APPEND AUDIO_DEPS ${SDL2MAIN_LIBRARY})
  endif(PLATFORM_WINDOWS)

  target_link_libraries(ALAudioTest ${AUDIO_DEPS})

  GTEST_ADD_TESTS(${TESTS_INSTALL_DIR}/ALAudioTest "" ${TEST_SRC})

  # Copy resource files for the test
  nom_install_resources("${NOM_TESTS_RESOURCES_DIR}/audio/ALAudioTest.json"
                        "${TESTS_INSTALL_DIR}")

endif(NOM_BUILD_OPENAL_AUDIO_TESTS)
