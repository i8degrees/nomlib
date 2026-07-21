# nomlib-audio library

# Add and link the library
add_library(nomlib-audio
  ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
)

# CMake generates our NOM_EXPORT macros, i.e.: __declspec(dllexport)
generate_export_header(nomlib-audio
  BASE_NAME
    NOM
  EXPORT_FILE_NAME
    export.hpp
)

# Default implementation sources (always built)
target_sources(nomlib-audio
  PRIVATE
  # ${SRC_DIR}/audio/AudioDeviceLocator.cpp
  # ${SRC_DIR}/audio/NullAudioDevice.cpp
  # ${SRC_DIR}/audio/NullAudioDeviceCaps.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    ${HDR_BASE}/audio.hpp
    ${HDR_BASE}/audio/audio_defs.hpp
    ${HDR_BASE}/audio/SoundBuffer.hpp
    ${HDR_BASE}/audio/ISoundFileReader.hpp
    ${HDR_BASE}/audio/ISoundFileWriter.hpp
    ${HDR_BASE}/audio/SoundFile.hpp
    ${HDR_BASE}/audio/IAudioDevice.hpp
    ${HDR_BASE}/audio/IOAudioEngine.hpp
    # ${HDR_BASE}/audio/AudioDeviceLocator.hpp
    # ${HDR_BASE}/audio/NullAudioDevice.hpp
    # ${HDR_BASE}/audio/NullAudioDeviceCaps.hpp
    FILE_SET generated_headers
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    FILES ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
)

# Common dependencies on all platforms
if(NOM_BUILD_AUDIO_UNIT)
  # Common implementation sources (OpenAL)
  target_sources(nomlib-audio
    PRIVATE
    ${SRC_DIR}/audio/AL/ALAudioDevice.cpp
    ${SRC_DIR}/audio/AL/ALAudioDeviceCaps.cpp
    ${SRC_DIR}/audio/AL/OpenAL.cpp
    # TODO(jeff): Divide libsndfile directory into its own source segment
    ${SRC_DIR}/audio/libsndfile/SoundFileReader.cpp
    # ${SRC_DIR}/audio/libsndfile/SoundFileWriter.cpp
    ${SRC_DIR}/audio/AL/SoundSource.cpp
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${HDR_BASE}
      FILES
      ${HDR_BASE}/audio/AL/ALAudioDevice.hpp
      ${HDR_BASE}/audio/AL/ALAudioDeviceCaps.hpp
      ${HDR_BASE}/audio/AL/OpenAL.hpp
      ${HDR_BASE}/audio/AL/SoundSource.hpp
      # TODO(jeff): Divide libsndfile directory into its own source segment
      ${HDR_BASE}/audio/libsndfile/SoundFileReader.hpp
      ${HDR_BASE}/audio/libsndfile/SoundFileWriter.hpp
    )
    # ...Mac OS X specific OpenAL implementation details...
    if(APPLE OR DEFINED NOM_USE_APPLE_OPENAL)
      target_sources(nomlib-audio
        PRIVATE
        ${SRC_DIR}/audio/AL/osx/apple_extensions.cpp
        PUBLIC
          FILE_SET HEADERS
          BASE_DIRS ${HDR_BASE}
          FILES
          ${HDR_BASE}/audio/AL/osx/apple_extensions.hpp
      )
    endif()
endif(NOM_BUILD_AUDIO_UNIT)

# This is embedded as the SONAME in the library's ELF header and determinnes
# which file the dynamic linker searches for at runtime, i.e.:
# `libnomlib-actions.so.1 -> `libnomlib-actions.so.0.11`
# The Application Binary Interface (ABI) version; PATCH level versions are
# intended **not** to break the ABI version.
set_target_properties(nomlib-audio
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
)

if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
  # Skip dllimport/dllexport logic when building as a static library target
  target_compile_definitions(nomlib-audio
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-audio
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

target_include_directories(nomlib-audio
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${SDL2_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIR}
  ${LIBSNDFILE_INCLUDE_DIR}
)

target_compile_features(nomlib-audio
  PRIVATE
    cxx_std_14
)

# Common dependencies on all platforms
target_link_libraries(nomlib-audio
  PUBLIC
  nomlib-core
  nomlib-math
  nomlib-system
  PRIVATE
    project_warnings
    OpenAL::OpenAL
    SndFile::sndfile
)

# MacOSX Framework bundle support
set_target_properties(nomlib-audio
  PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST
      "${CMAKE_TEMPLATE_PATH}/Info.plist.in"
    MACOSX_FRAMEWORK_NAME
      nomlib-audio
    MACOSX_FRAMEWORK_BUNDLE_VERSION
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_FRAMEWORK_IDENTIFIER
      "net.i8degrees.nomlib-audio"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
      ""
    MACOSX_RPATH TRUE
    #INSTALL_RPATH
      #@executable_path/../Frameworks
)

# MacOSX Framework bundle support - public header files
if(APPLE)
  install(TARGETS nomlib-audio
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-audio.framework/Headers"
      COMPONENT Development
    FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-audio.framework/Headers/audio"
      COMPONENT Development
  )
endif(APPLE)

install(TARGETS nomlib-audio
  EXPORT libtarget
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT Development
  FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/audio"
    COMPONENT Development
  # MacOSX framework bundle
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
)

