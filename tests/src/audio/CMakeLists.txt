# nomlib-audio module tests

set( NOM_BUILD_OPENAL_AUDIO_TESTS ON )

if( NOM_BUILD_OPENAL_AUDIO_TESTS )

  add_executable( ALAudioTest "ALAudioTest.cpp" )

  # FIXME: We only need init from nomlib-system
  set(  AUDIO_DEPS ${GTEST_LIBRARY} nomlib-core nomlib-math nomlib-audio
        nomlib-system )

  if( PLATFORM_WINDOWS )
    list( APPEND AUDIO_DEPS ${SDL2MAIN_LIBRARY} )
  endif( PLATFORM_WINDOWS )

  target_link_libraries( ALAudioTest ${AUDIO_DEPS} )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ALAudioTest
                    "" # args
                    "ALAudioTest.cpp" )

  # Resources for audio tests
  install (
            DIRECTORY
            "${EXAMPLES_SRC_DIR}/audio/Resources"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
            FILES_MATCHING PATTERN "*.wav"
          )

endif( NOM_BUILD_OPENAL_AUDIO_TESTS )
