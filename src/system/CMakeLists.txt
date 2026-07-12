# nomlib-system library

# Add and link the library
add_library(nomlib-system
  ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
)

# CMake generates our NOM_EXPORT macros, i.e.: __declspec(dllexport)
generate_export_header(nomlib-system
  BASE_NAME
    NOM
  EXPORT_FILE_NAME
    export.hpp
)

set_target_properties(nomlib-system
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSION_REQUIRED OFF
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
    FILE_SET generated_headers
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    FILES ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
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

target_link_libraries(nomlib-system
  PUBLIC
  nomlib-core
  nomlib-file
  nomlib-math
  PRIVATE
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

# MacOSX Framework bundle support
set_target_properties(nomlib-system
  PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST
      "${CMAKE_TEMPLATE_PATH}/Info.plist.in"
    MACOSX_FRAMEWORK_NAME
      nomlib-system
    MACOSX_FRAMEWORK_BUNDLE_VERSION
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_FRAMEWORK_IDENTIFIER
      "net.i8degrees.nomlib-system"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
      ""
    MACOSX_RPATH TRUE
    #INSTALL_RPATH
      #@executable_path/../Frameworks
)

# MacOSX Framework bundle support - public header files
if(PLATFORM_OSX)
  install(TARGETS nomlib-system
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-system.framework/Headers"
      COMPONENT Development
    FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-system.framework/Headers/system"
      COMPONENT Development
  )
endif(PLATFORM_OSX)

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
  FILE_SET generated_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/system"
    COMPONENT Development
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
)

