if( NOM_BUILD_OPENAL_AUDIO_TESTS )

  add_executable  (
                    al_audio_test
                    ${NOM_TESTS_SOURCE_DIR}/audio/ALAudioTest.cpp
                  )

  target_link_libraries( al_audio_test UnitTest )

  GTEST_ADD_TESTS (
                    al_audio_test
                    "ALAudioTest"
                    ${NOM_TESTS_SOURCE_DIR}/audio/ALAudioTest.cpp
                  )

  # Resources for audio tests
  install (
            DIRECTORY
            "${EXAMPLES_SRC_DIR}/audio/Resources"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
            FILES_MATCHING PATTERN "*.wav"
          )

endif( NOM_BUILD_OPENAL_AUDIO_TESTS )
