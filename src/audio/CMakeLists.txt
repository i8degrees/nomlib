# nomlib-audio library

# Library headers inclusion path
include_directories( "${INC_ROOT_DIR}" )

# Library name
set( NOM_AUDIO_LIBRARY "nomlib-audio" )

# Default implementation sources (always built)
set(  NOM_AUDIO_SOURCE
      ${INC_DIR}/audio.hpp
      ${INC_DIR}/audio/audio_defs.hpp

      ${INC_DIR}/audio/SoundBuffer.hpp
      ${INC_DIR}/audio/ISoundFileReader.hpp
      ${INC_DIR}/audio/ISoundFileWriter.hpp
      ${INC_DIR}/audio/SoundFile.hpp

      ${INC_DIR}/audio/IAudioDevice.hpp
      ${INC_DIR}/audio/IOAudioEngine.hpp

      # ${SRC_DIR}/audio/AudioDeviceLocator.cpp
      # ${INC_DIR}/audio/AudioDeviceLocator.hpp

      # ${SRC_DIR}/audio/NullAudioDevice.cpp
      # ${INC_DIR}/audio/NullAudioDevice.hpp
      # ${SRC_DIR}/audio/NullAudioDeviceCaps.cpp
      # ${INC_DIR}/audio/NullAudioDeviceCaps.hpp
)

# Common dependencies on all platforms
# TODO(jeff): Try and rid ourselves of dependence on nomlib-system here for the
# audio subsystem!
set(NOM_AUDIO_DEPS nomlib-core nomlib-math nomlib-system)

if(NOM_BUILD_AUDIO_UNIT)

  # Common implementation sources (OpenAL)
  set(NOM_AUDIO_AL_SOURCE
      ${SRC_DIR}/audio/AL/ALAudioDevice.cpp
      ${INC_DIR}/audio/AL/ALAudioDevice.hpp
      ${SRC_DIR}/audio/AL/ALAudioDeviceCaps.cpp
      ${INC_DIR}/audio/AL/ALAudioDeviceCaps.hpp

      ${SRC_DIR}/audio/AL/OpenAL.cpp
      ${INC_DIR}/audio/AL/OpenAL.hpp

      # TODO(jeff): Divide libsndfile directory into its own source segment
      ${SRC_DIR}/audio/libsndfile/SoundFileReader.cpp
      ${INC_DIR}/audio/libsndfile/SoundFileReader.hpp
      # ${SRC_DIR}/audio/libsndfile/SoundFileWriter.cpp
      # ${INC_DIR}/audio/libsndfile/SoundFileWriter.hpp

      ${SRC_DIR}/audio/AL/SoundSource.cpp
      ${INC_DIR}/audio/AL/SoundSource.hpp)

  list(APPEND NOM_AUDIO_SOURCE ${NOM_AUDIO_AL_SOURCE})

  # ...Mac OS X specific OpenAL implementation details...
  # FIXME(jeff): ...
  if(PLATFORM_OSX AND NOM_USE_APPLE_OPENAL)
    set(NOM_AUDIO_AL_OSX_SOURCE
        ${SRC_DIR}/audio/AL/osx/apple_extensions.cpp
        ${INC_DIR}/audio/AL/osx/apple_extensions.hpp)
    list(APPEND NOM_AUDIO_SOURCE ${NOM_AUDIO_AL_OSX_SOURCE})
  endif(PLATFORM_OSX AND NOM_USE_APPLE_OPENAL)

  # OpenAL Library dependencies
  if(NOM_USE_OPENAL OR NOM_USE_APPLE_OPENAL)
    find_package(OpenAL REQUIRED)
  elseif(NOM_USE_OPENAL_SOFT)
    # TODO(jeff): Implement custom packaging of the OpenAL library and header
    # file paths; nomlib-config.cmake & friends.
    set(OPENAL_INCLUDE_DIR "/usr/local/opt/openal-soft/include/")
    set(OPENAL_LIBRARY "/usr/local/opt/openal-soft/lib/libopenal.dylib")
    set(OPENAL_FOUND true)
  endif(NOM_USE_OPENAL OR NOM_USE_APPLE_OPENAL)

  if(OPENAL_FOUND)
    # Add development header files; al.h, alc.h
    NOM_LOG_INFO("OPENAL_INCLUDE_DIR: ${OPENAL_INCLUDE_DIR}")
    include_directories(${OPENAL_INCLUDE_DIR})
    list(APPEND NOM_AUDIO_DEPS ${OPENAL_LIBRARY})
  endif(OPENAL_FOUND)

  find_package(libsndfile REQUIRED)
  if(LIBSNDFILE_FOUND)
    # Add development header files; sndfile.h
    include_directories(${LIBSNDFILE_INCLUDE_DIR})
  endif(LIBSNDFILE_FOUND)

  list(APPEND NOM_AUDIO_DEPS ${LIBSNDFILE_LIBRARY})

endif(NOM_BUILD_AUDIO_UNIT)

# Add and link the library
nom_add_library(  ${NOM_AUDIO_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                  "${NOM_AUDIO_SOURCE}" ""
                  "${NOM_AUDIO_DEPS}" )

# Re-declare globally, so other build units that depend on us can refer to this
# variable
# set( NOM_AUDIO_LIBRARY ${NOM_AUDIO_LIBRARY} PARENT_SCOPE )
