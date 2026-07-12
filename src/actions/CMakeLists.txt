# nomlib-actions library

# Add and link the library
add_library(nomlib-actions
  ${LIBRARY_OUTPUT_TYPE}
)

# CMake generates our NOM_EXPORT macros, i.e.: __declspec(dllexport)
generate_export_header(nomlib-actions
  BASE_NAME
    NOM
  EXPORT_FILE_NAME
    export.hpp
)

set_target_properties(nomlib-actions
  PROPERTIES
    VERSION "${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSION_REQUIRED OFF
)

# TODO(JEFF): Add debug compile flags when configuration uses debug flag

if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

  # Skip dllimport/dllexport logic when building as a static library target
  target_compile_definitions(nomlib-actions
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-actions
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

# Base unit
target_sources(nomlib-actions
  PRIVATE
  ${SRC_DIR}/actions/ActionTimingCurves.cpp
  ${SRC_DIR}/actions/IActionObject.cpp
  ${SRC_DIR}/actions/ActionPlayer.cpp
  ${SRC_DIR}/actions/CallbackAction.cpp
  ${SRC_DIR}/actions/DispatchQueue.cpp
  ${SRC_DIR}/actions/GroupAction.cpp
  ${SRC_DIR}/actions/RemoveAction.cpp
  ${SRC_DIR}/actions/RepeatForAction.cpp
  ${SRC_DIR}/actions/RepeatForeverAction.cpp
  ${SRC_DIR}/actions/ReversedAction.cpp
  ${SRC_DIR}/actions/SequenceAction.cpp
  ${SRC_DIR}/actions/WaitForDurationAction.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    ${HDR_BASE}/actions.hpp
    ${HDR_BASE}/actions/ActionTimingCurves.hpp
    ${HDR_BASE}/actions/IActionObject.hpp
    ${HDR_BASE}/actions/ActionPlayer.hpp
    ${HDR_BASE}/actions/CallbackAction.hpp
    ${HDR_BASE}/actions/DispatchQueue.hpp
    ${HDR_BASE}/actions/GroupAction.hpp
    ${HDR_BASE}/actions/RemoveAction.hpp
    ${HDR_BASE}/actions/RepeatForAction.hpp
    ${HDR_BASE}/actions/RepeatForeverAction.hpp
    ${HDR_BASE}/actions/ReversedAction.hpp
    ${HDR_BASE}/actions/SequenceAction.hpp
    ${HDR_BASE}/actions/WaitForDurationAction.hpp
    FILE_SET generated_headers
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    FILES ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
)

target_include_directories(nomlib-actions
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${SDL2_INCLUDE_DIR}
)

# sub-feature unit available with nomlib-graphics
if(NOM_BUILD_GRAPHICS_UNIT)
  # add graphics option for using sprite actions
  target_sources(nomlib-actions
    PRIVATE
    ${SRC_DIR}/actions/AnimateTexturesAction.cpp
    ${SRC_DIR}/actions/FadeInAction.cpp
    ${SRC_DIR}/actions/FadeOutAction.cpp
    ${SRC_DIR}/actions/FadeAlphaByAction.cpp
    ${SRC_DIR}/actions/MoveByAction.cpp
    ${SRC_DIR}/actions/ScaleByAction.cpp
    ${SRC_DIR}/actions/SpriteBatchAction.cpp
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${HDR_BASE}
      FILES
      ${HDR_BASE}/actions/AnimateTexturesAction.hpp
      ${HDR_BASE}/actions/FadeInAction.hpp
      ${HDR_BASE}/actions/FadeOutAction.hpp
      ${HDR_BASE}/actions/FadeAlphaByAction.hpp
      ${HDR_BASE}/actions/MoveByAction.hpp
      ${HDR_BASE}/actions/ScaleByAction.hpp
      ${HDR_BASE}/actions/SpriteBatchAction.hpp
  )

  # Internal dependencies for this action subgroup
  target_link_libraries(nomlib-actions
    PUBLIC
      nomlib-graphics
    #PRIVATE
      #$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  )
endif(NOM_BUILD_GRAPHICS_UNIT)

# sub-feature unit available with nomlib-audio for audio actions
if(NOM_BUILD_AUDIO_UNIT)
  target_sources(nomlib-actions
    PRIVATE
    ${SRC_DIR}/actions/FadeAudioGainBy.cpp
    ${SRC_DIR}/actions/PlayAudioSource.cpp
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${HDR_BASE}
      FILES
      ${HDR_BASE}/actions/FadeAudioGainBy.hpp
      ${HDR_BASE}/actions/PlayAudioSource.hpp
  )

  # Internal dependencies for this action subgroup
  target_link_libraries(nomlib-actions
    PUBLIC
      nomlib-audio
    PRIVATE
    $<IF:$<TARGET_EXISTS:OpenAL::OpenAL>,OpenAL::OpenAL,OpenAL::OpenAL-static>
  )
endif(NOM_BUILD_AUDIO_UNIT)

# Common internal dependencies for actions
target_link_libraries(nomlib-actions
  PUBLIC
  nomlib-core
  nomlib-math
)

# MacOSX Framework bundle support
set_target_properties(nomlib-actions
  PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST
      "${CMAKE_TEMPLATE_PATH}/Info.plist.in"
    MACOSX_FRAMEWORK_NAME
      nomlib-actions
    MACOSX_FRAMEWORK_BUNDLE_VERSION
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_FRAMEWORK_IDENTIFIER
      "net.i8degrees.nomlib-actions"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
      ""
    MACOSX_RPATH TRUE
)

# MacOSX framework bundle support - public header files
if(PLATFORM_OSX)
  install(TARGETS nomlib-actions
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-actions.framework/Headers"
      COMPONENT Development
    FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-actions.framework/Headers/actions"
      COMPONENT Development
  )
endif(PLATFORM_OSX)

install(TARGETS nomlib-actions
  EXPORT libtarget
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT Development
  FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/actions"
    COMPONENT Development
  # MacOSX framework bundle
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
)

