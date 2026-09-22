# nomlib-system library

# Add and link the library
add_library(nomlib-system
  ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
)

set_target_properties(nomlib-system
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
    MAP_IMPORTED_CONFIG_MINSIZEREL Release
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO Debug
)

if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
  # Skip dllimport/dllexport logic when building as a static library target
  target_compile_definitions(nomlib-system
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-system
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

target_sources(nomlib-system
  PRIVATE
  # TODO: Consider relocating this to graphics
  ${SRC_DIR}/system/ColorDatabase.cpp
  ${SRC_DIR}/system/EventHandler.cpp
  ${SRC_DIR}/system/JoystickEventHandler.cpp
  ${SRC_DIR}/system/GameControllerEventHandler.cpp
  ${SRC_DIR}/system/Joystick.cpp
  ${SRC_DIR}/system/GameController.cpp
  ${SRC_DIR}/system/FPS.cpp
  ${SRC_DIR}/system/IState.cpp
  ${SRC_DIR}/system/ResourceFile.cpp
  ${SRC_DIR}/system/SDLApp.cpp
  ${SRC_DIR}/system/SDL_helpers.cpp
  ${SRC_DIR}/system/StateMachine.cpp
  ${SRC_DIR}/system/Timer.cpp
  ${SRC_DIR}/system/HighResolutionTimer.cpp
  ${SRC_DIR}/system/dialog_messagebox.cpp
  # !! Branch init into each library target to break up dependency chain
  ${SRC_DIR}/system/init.cpp
  # TODO: Consider relocating this to graphics
  # ${SRC_DIR}/system/resource_handlers.cpp
  ${SRC_DIR}/system/InputMapper/InputAction.cpp
  ${SRC_DIR}/system/InputMapper/InputActionMapper.cpp
  ${SRC_DIR}/system/InputMapper/InputStateMapper.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    ${HDR_BASE}/system.hpp
    # TODO: Consider relocating this to graphics
    ${HDR_BASE}/system/ColorDatabase.hpp
    ${HDR_BASE}/system/Event.hpp
    ${HDR_BASE}/system/EventHandler.hpp
    ${HDR_BASE}/system/JoystickEventHandler.hpp
    ${HDR_BASE}/system/GameControllerEventHandler.hpp
    ${HDR_BASE}/system/Joystick.hpp
    ${HDR_BASE}/system/GameController.hpp
    ${HDR_BASE}/system/FPS.hpp
    ${HDR_BASE}/system/IState.hpp
    ${HDR_BASE}/system/ResourceCache.hpp
    ${HDR_BASE}/system/ResourceFile.hpp
    ${HDR_BASE}/system/SDLApp.hpp
    ${HDR_BASE}/system/SDL_helpers.hpp
    ${HDR_BASE}/system/StateMachine.hpp
    ${HDR_BASE}/system/Timer.hpp
    ${HDR_BASE}/system/HighResolutionTimer.hpp
    ${HDR_BASE}/system/dialog_messagebox.hpp
    # TODO: Relocate
    ${HDR_BASE}/system/init.hpp
    # TODO: Consider relocating this to graphics
    # ${HDR_BASE}/system/resource_handlers.hpp
    ${HDR_BASE}/system/resource_types.hpp
    ${HDR_BASE}/system/InputMapper/InputAction.hpp
    ${HDR_BASE}/system/InputMapper/InputActionMapper.hpp
    ${HDR_BASE}/system/InputMapper/InputStateMapper.hpp
)

target_include_directories(nomlib-system
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${SDL2_INCLUDE_DIR}
  # FIXME: Needed only by nom::init and nom::quit
  ${SDL2_IMAGE_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIR}
  # TODO: add to init.cpp for inheriting cmdline
  #${TCLAP_INCLUDE_DIR}
)

target_compile_features(nomlib-system
  PRIVATE
    cxx_std_14
)

target_link_libraries(nomlib-system
  PUBLIC
    nomlib-core
    nomlib-file
    nomlib-math
  PRIVATE
    project_warnings
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

if(APPLE AND CMAKE_FRAMEWORK)
  target_compile_definitions(nomlib-core
    PUBLIC
      CMAKE_FRAMEWORK
  )
endif()

# MacOSX Framework bundle support
apple_setup_framework(nomlib-system
  "${CMAKE_TEMPLATE_PATH}/framework.plist.in"
)

# MacOSX Framework bundle support - public header files
apple_install_framework_headers(nomlib-system)

if(NOT CMAKE_FRAMEWORK)
  install(TARGETS nomlib-system
    EXPORT libtarget
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      COMPONENT Runtime
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
      COMPONENT Development
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX} #${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
  )
endif()
