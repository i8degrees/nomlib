# nomlib-audio module tests

set(NOM_BUILD_OPENAL_AUDIO_TESTS ON)

if(NOM_BUILD_OPENAL_AUDIO_TESTS)
  add_executable(ALAudioTest
    #WIN32 # Link SDL2main over WINMAIN for console apps
    ALAudioTest.cpp
  )

  add_dependencies(ALAudioTest gtest)

  # console app
  if(TARGET SDL2::SDL2main)
    target_link_libraries(ALAudioTest
      PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    )
  endif(TARGET SDL2::SDL2main)

  target_include_directories(ALAudioTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
      ${OPENAL_INCLUDE_DIR}
      #${LIBSNDFILE_INCLUDE_DIR}
  )

  target_link_libraries(ALAudioTest
    PUBLIC
      nomlib-audio
      nomlib-core
      nomlib-math
      nomlib-graphics
      # FIXME: We only need init from nomlib-system
      nomlib-system
      #nomlib-serializers
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<IF:$<TARGET_EXISTS:SndFile::sndfile>,SndFile::sndfile,SndFile::sndfile-static>
      $<IF:$<TARGET_EXISTS:OpenAL::OpenAL>,OpenAL::OpenAL,OpenAL::OpenAL-static>
  )

  GTEST_ADD_TESTS(${TESTS_INSTALL_DIR}/ALAudioTest "" ALAudioTest.cpp)

  # For develpopment: Copy resource files for the test:
  add_custom_command(TARGET ALAudioTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/audio/ALAudioTest.json"
    $<TARGET_FILE_DIR:ALAudioTest>
  )

endif(NOM_BUILD_OPENAL_AUDIO_TESTS)
