# nomlib-audio library

# Library headers inclusion path
include_directories( "${INC_ROOT_DIR}" )

# Library name
set( NOM_AUDIO_LIBRARY "nomlib-audio" )

# Default implementation sources (always built)
set(  NOM_AUDIO_SOURCE
      ${INC_DIR}/audio.hpp
      # ${SRC_DIR}/audio/AudioDeviceLocator.cpp
      # ${INC_DIR}/audio/AudioDeviceLocator.hpp

      ${INC_DIR}/audio/IAudioDevice.hpp
      ${INC_DIR}/audio/IListener.hpp
      # ${INC_DIR}/audio/ISoundBuffer.hpp
      # ${INC_DIR}/audio/ISoundSource.hpp

      # ${SRC_DIR}/audio/NullAudioDevice.cpp
      # ${INC_DIR}/audio/NullAudioDevice.hpp

      # ${SRC_DIR}/audio/NullListener.cpp
      # ${INC_DIR}/audio/NullListener.hpp

      # ${SRC_DIR}/audio/NullMusic.cpp
      # ${INC_DIR}/audio/NullMusic.hpp

      # ${SRC_DIR}/audio/NullSound.cpp
      # ${INC_DIR}/audio/NullSound.hpp

      # ${SRC_DIR}/audio/NullSoundBuffer.cpp
      # ${INC_DIR}/audio/NullSoundBuffer.hpp

      # ${SRC_DIR}/audio/NullSoundSource.cpp
      # ${INC_DIR}/audio/NullSoundSource.hpp
)

# Common dependencies on all platforms
set(NOM_AUDIO_DEPS nomlib-core nomlib-math)

# OpenAL implementation
if( NOM_BUILD_AUDIO_UNIT )

  set(  NOM_AUDIO_AL_SOURCE
        ${SRC_DIR}/audio/AL/AudioDevice.cpp
        ${INC_DIR}/audio/AL/AudioDevice.hpp

        ${SRC_DIR}/audio/AL/ALAudioDeviceCaps.cpp

        ${SRC_DIR}/audio/AL/Listener.cpp
        ${INC_DIR}/audio/AL/Listener.hpp

        # ${SRC_DIR}/audio/AL/Music.cpp
        # ${INC_DIR}/audio/AL/Music.hpp

        ${SRC_DIR}/audio/AL/OpenAL.cpp
        ${INC_DIR}/audio/AL/OpenAL.hpp

        # ${SRC_DIR}/audio/AL/Sound.cpp
        # ${INC_DIR}/audio/AL/Sound.hpp

        ${SRC_DIR}/audio/AL/SoundBuffer.cpp
        # ${INC_DIR}/audio/AL/SoundBuffer.hpp

        ${SRC_DIR}/audio/AL/SoundFile.cpp
        # ${INC_DIR}/audio/AL/SoundFile.hpp
        # ${SRC_DIR}/audio/AL/SoundFile.hpp

        ${SRC_DIR}/audio/AL/SoundSource.cpp
        ${INC_DIR}/audio/AL/SoundSource.hpp
  )

  list( APPEND NOM_AUDIO_SOURCE ${NOM_AUDIO_AL_SOURCE} )

  # Platform-specific dependencies

  find_package( OpenAL REQUIRED )
  if( OPENAL_FOUND )
    # Add development header files; al.h, alc.h
    include_directories( ${OPENAL_INCLUDE_DIR} )
    list( APPEND NOM_AUDIO_DEPS ${OPENAL_LIBRARY} )
  endif( OPENAL_FOUND )

  find_package( libsndfile REQUIRED )
  if( LIBSNDFILE_FOUND )
    # Add development header files; sndfile.h
    include_directories( ${LIBSNDFILE_INCLUDE_DIR} )
    list( APPEND NOM_AUDIO_DEPS ${LIBSNDFILE_LIBRARY} )
  endif( LIBSNDFILE_FOUND )

endif( NOM_BUILD_AUDIO_UNIT )

# Add and link the library
nom_add_library(  ${NOM_AUDIO_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                  "${NOM_AUDIO_SOURCE}" ""
                  "${NOM_AUDIO_DEPS}" )

# Re-declare globally, so other build units that depend on us can refer to this
# variable
# set( NOM_AUDIO_LIBRARY ${NOM_AUDIO_LIBRARY} PARENT_SCOPE )
