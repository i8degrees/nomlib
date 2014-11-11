if( NOM_BUILD_OPENAL_AUDIO_TESTS )

  add_executable  (
                    ALAudioTest
                    ${NOM_TESTS_SOURCE_DIR}/audio/ALAudioTest.cpp
                  )

  target_link_libraries( ALAudioTest UnitTest )

  GTEST_ADD_TESTS (
                    ALAudioTest
                    "" # args
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
