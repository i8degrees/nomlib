# nomlib-actions library

# Add and link the library
add_library(nomlib-actions
  ${LIBRARY_OUTPUT_TYPE}
)

set_target_properties(nomlib-actions
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
    MAP_IMPORTED_CONFIG_MINSIZEREL Release
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO Debug
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
    PRIVATE
      project_warnings
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
      project_warnings
      $<IF:$<TARGET_EXISTS:OpenAL::OpenAL>,OpenAL::OpenAL,OpenAL::OpenAL-static>
  )
endif(NOM_BUILD_AUDIO_UNIT)

target_compile_features(nomlib-actions
  PRIVATE
    cxx_std_14
)

# Common internal dependencies for actions
target_link_libraries(nomlib-actions
  PUBLIC
    nomlib-core
    nomlib-math
  PRIVATE
    project_warnings
)

# MacOSX Framework bundle support
apple_setup_framework(nomlib-actions
  ${CMAKE_TEMPLATE_PATH}/framework.plist.in
)

# MacOSX Framework bundle support - public header files
apple_install_framework_headers(nomlib-actions)

if(NOT CMAKE_FRAMEWORK)
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
    # MacOSX framework bundle
    #FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      #COMPONENT Runtime
  )
endif()
