# nomlib-audio library

# Add and link the library
add_library(nomlib-audio
  ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
)

# !! Rename to NOM_USE_LIBOGG
set(USE_LIBOGG TRUE)

if(USE_LIBOGG)
  find_package(Vorbis CONFIG REQUIRED)
endif()

if(USE_LIBOGG)
  target_sources(nomlib-audio
    PRIVATE
      ${SRC_DIR}/audio/OggSound.cpp
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${HDR_BASE}/audio
      FILES
        ${HDR_BASE}/audio/OggSound.hpp
  )
endif()

# Default implementation sources (always built)
target_sources(nomlib-audio
  PRIVATE
    ${SRC_DIR}/audio/libsndfile/SoundFileReader.cpp
    ${SRC_DIR}/audio/AL/OpenAL.cpp
    ${SRC_DIR}/audio/ALAudioContext.cpp
    ${SRC_DIR}/audio/ALAudioRenderer.cpp
    ${SRC_DIR}/audio/ALSoundSource.cpp
    ${SRC_DIR}/audio/AudioDeviceLocator.cpp
    ${SRC_DIR}/audio/ISoundSource.cpp
    ${SRC_DIR}/audio/Sound.cpp
    ${SRC_DIR}/audio/SoundManager.cpp
    ${SRC_DIR}/audio/StreamSound.cpp
    ${SRC_DIR}/audio/init.cpp
    ${SRC_DIR}/audio/vendor/Matrix4.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}/audio
    FILES
      ${HDR_BASE}/audio/AL/OpenAL.hpp
      ${HDR_BASE}/audio/ALAudioContext.hpp
      ${HDR_BASE}/audio/ALAudioRenderer.hpp
      ${HDR_BASE}/audio/ALSoundSource.hpp
      ${HDR_BASE}/audio/AudioDeviceLocator.hpp
      ${HDR_BASE}/audio/IAudioContext.hpp
      ${HDR_BASE}/audio/IAudioDevice.hpp
      ${HDR_BASE}/audio/init.hpp
      ${HDR_BASE}/audio/ISoundSource.hpp
      ${HDR_BASE}/audio/OggSound.hpp
      ${HDR_BASE}/audio/Sound.hpp
      ${HDR_BASE}/audio/SoundManager.hpp
      ${HDR_BASE}/audio/StreamSound.hpp
)

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

if(USE_LIBOGG)
  target_compile_definitions(nomlib-audio
    PUBLIC
      USE_LIBOGG
  )
endif()

target_compile_definitions(nomlib-audio
  PUBLIC
    VERBOSE
    DEBUG
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
    ${nom_cxx_std}
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

if(USE_LIBOGG)
  target_link_libraries(nomlib-audio
    PUBLIC
      nomlib-core
      nomlib-math
      nomlib-system
    PRIVATE
      Vorbis::vorbisfile
  )
endif(USE_LIBOGG)

# MacOSX Framework bundle support
set_target_properties(nomlib-audio
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
  # MacOSX framework bundle
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX}
    COMPONENT Runtime
)
