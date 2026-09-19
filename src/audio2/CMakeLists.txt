# nomlib-audio2 library

# Add and link the library
add_library(nomlib-audio2
  ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
)

set(USE_LIBOGG TRUE)
set(LOAD_WAV TRUE)

if(LOAD_WAV)
  # sndfile is already found by this point
endif()

if(USE_LIBOGG)
  find_package(Vorbis CONFIG REQUIRED)
endif()

if(USE_LIBOGG)
  target_sources(nomlib-audio2
    PRIVATE
      ${SRC_DIR}/audio2/OggSound.cpp
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${HDR_BASE}/audio2
      FILES
        ${HDR_BASE}/audio2/OggSound.hpp
  )
endif()

# Default implementation sources (always built)
target_sources(nomlib-audio2
  PRIVATE
    ${SRC_DIR}/audio/libsndfile/SoundFileReader.cpp
    ${SRC_DIR}/audio/AL/OpenAL.cpp
    ${SRC_DIR}/audio2/ALAudioContext.cpp
    ${SRC_DIR}/audio2/ALAudioRenderer.cpp
    ${SRC_DIR}/audio2/ALSoundSource.cpp
    ${SRC_DIR}/audio2/AudioDeviceLocator.cpp
    ${SRC_DIR}/audio2/ISoundSource.cpp
    ${SRC_DIR}/audio2/Sound.cpp
    ${SRC_DIR}/audio2/SoundManager.cpp
    ${SRC_DIR}/audio2/StreamSound.cpp
    ${SRC_DIR}/audio2/init.cpp
    ${SRC_DIR}/audio2/vendor/Matrix4.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}/audio2
    FILES
      ${HDR_BASE}/audio2/ALAudioContext.hpp
      ${HDR_BASE}/audio2/ALAudioRenderer.hpp
      ${HDR_BASE}/audio2/ALSoundSource.hpp
      ${HDR_BASE}/audio2/AudioDeviceLocator.hpp
      ${HDR_BASE}/audio2/IAudioContext.hpp
      ${HDR_BASE}/audio2/IAudioDevice.hpp
      ${HDR_BASE}/audio2/init.hpp
      ${HDR_BASE}/audio2/ISoundSource.hpp
      ${HDR_BASE}/audio2/OggSound.hpp
      # FIXME
      #${HDR_DIR}/audio/AL/OpenAL.hpp
      ${HDR_BASE}/audio2/Sound.hpp
      #${HDR_BASE}/SoundFileReader.hpp
      ${HDR_BASE}/audio2/SoundManager.hpp
      ${HDR_BASE}/audio2/StreamSound.hpp
      # ${HDR_BASE}/audio2/types.hpp
)

# This is embedded as the SONAME in the library's ELF header and determinnes
# which file the dynamic linker searches for at runtime, i.e.:
# `libnomlib-actions.so.1 -> `libnomlib-actions.so.0.11`
# The Application Binary Interface (ABI) version; PATCH level versions are
# intended **not** to break the ABI version.
set_target_properties(nomlib-audio2
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
)

if(LOAD_WAV)
  target_compile_definitions(nomlib-audio2
    PUBLIC
      LOAD_WAV
  )
endif()

if(USE_LIBOGG)
  target_compile_definitions(nomlib-audio2
    PUBLIC
      USE_LIBOGG
  )
endif()

target_compile_definitions(nomlib-audio2
  PUBLIC
    VERBOSE
    DEBUG
)

if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
  # Skip dllimport/dllexport logic when building as a static library target
  target_compile_definitions(nomlib-audio2
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-audio2
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

target_include_directories(nomlib-audio2
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${SDL2_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${LIBSNDFILE_INCLUDE_DIR}
)

target_compile_features(nomlib-audio2
  PRIVATE
    ${nom_cxx_std}
)

# Common dependencies on all platforms
target_link_libraries(nomlib-audio2
  PUBLIC
    nomlib-core
    nomlib-math
    nomlib-system
  PRIVATE
    project_warnings
    OpenAL::OpenAL
)

if(LOAD_WAV)
  target_link_libraries(nomlib-audio2
    PUBLIC
      nomlib-core
      nomlib-math
      nomlib-system
    PRIVATE
      SndFile::sndfile
  )
endif(LOAD_WAV)

if(USE_LIBOGG)
  target_link_libraries(nomlib-audio2
    PUBLIC
      nomlib-core
      nomlib-math
      nomlib-system
    PRIVATE
      Vorbis::vorbisfile
  )
endif(USE_LIBOGG)

# MacOSX Framework bundle support
set_target_properties(nomlib-audio2
  PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST
      "${CMAKE_TEMPLATE_PATH}/framework.plist.in"
    MACOSX_FRAMEWORK_NAME
      nomlib-audio
    MACOSX_FRAMEWORK_BUNDLE_VERSION
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_FRAMEWORK_IDENTIFIER
      "net.i8degrees.nomlib-audio2"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
      ""
    MACOSX_RPATH TRUE
    #INSTALL_RPATH
      #@executable_path/../Frameworks
)

# MacOSX Framework bundle support - public header files
if(APPLE)
  install(TARGETS nomlib-audio2
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-audio.framework/Headers"
      COMPONENT Development
  )
endif(APPLE)

install(TARGETS nomlib-audio2
  EXPORT libtarget
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT Development
  # MacOSX framework bundle
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX}
    COMPONENT Runtime
)
