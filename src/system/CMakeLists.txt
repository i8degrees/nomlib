# nomlib-system library

# Library headers inclusion path
include_directories( "${INC_ROOT_DIR}" )

# Library name
set( NOM_SYSTEM_LIBRARY "nomlib-system" )

set(  NOM_SYSTEM_SOURCE
      ${INC_DIR}/system.hpp
      ${SRC_DIR}/system/AnimationTimer.cpp
      ${INC_DIR}/system/AnimationTimer.hpp

      # TODO: Consider relocating this to graphics
      ${SRC_DIR}/system/ColorDatabase.cpp
      ${INC_DIR}/system/ColorDatabase.hpp

      ${INC_DIR}/system/Event.hpp

      ${SRC_DIR}/system/EventCallback.cpp
      ${INC_DIR}/system/EventCallback.hpp

      ${SRC_DIR}/system/EventDispatcher.cpp
      ${INC_DIR}/system/EventDispatcher.hpp

      ${SRC_DIR}/system/EventHandler.cpp
      ${INC_DIR}/system/EventHandler.hpp

      ${SRC_DIR}/system/FPS.cpp
      ${INC_DIR}/system/FPS.hpp

      ${INC_DIR}/system/IJoystick.hpp
      ${SRC_DIR}/system/IState.cpp
      ${INC_DIR}/system/IState.hpp

      ${SRC_DIR}/system/Joystick.cpp
      ${INC_DIR}/system/Joystick.hpp

      ${INC_DIR}/system/ResourceCache.hpp
      ${SRC_DIR}/system/ResourceFile.cpp
      ${INC_DIR}/system/ResourceFile.hpp

      ${SRC_DIR}/system/SDLApp.cpp
      ${INC_DIR}/system/SDLApp.hpp

      ${SRC_DIR}/system/SDLJoystick.cpp
      ${INC_DIR}/system/SDLJoystick.hpp

      ${SRC_DIR}/system/SDL_helpers.cpp
      ${INC_DIR}/system/SDL_helpers.hpp

      ${SRC_DIR}/system/StateMachine.cpp
      ${INC_DIR}/system/StateMachine.hpp

      ${SRC_DIR}/system/Timer.cpp
      ${INC_DIR}/system/Timer.hpp

      ${SRC_DIR}/system/dialog_messagebox.cpp
      ${INC_DIR}/system/dialog_messagebox.hpp

      # TODO: Relocate
      ${SRC_DIR}/system/init.cpp
      ${INC_DIR}/system/init.hpp

      # TODO: Consider relocating this to graphics
      # ${SRC_DIR}/system/resource_handlers.cpp
      # ${INC_DIR}/system/resource_handlers.hpp

      ${INC_DIR}/system/resource_types.hpp

      ${SRC_DIR}/system/InputMapper/InputAction.cpp
      ${INC_DIR}/system/InputMapper/InputAction.hpp

      ${SRC_DIR}/system/InputMapper/InputActionMapper.cpp
      ${INC_DIR}/system/InputMapper/InputActionMapper.hpp

      ${SRC_DIR}/system/InputMapper/InputStateMapper.cpp
      ${INC_DIR}/system/InputMapper/InputStateMapper.hpp
)

# Platform-specific implementations & dependencies

find_package( SDL2_image REQUIRED )
if( SDL2_IMAGE_FOUND )
  # Add development header files; SDL_image.h
  include_directories( ${SDL2_IMAGE_INCLUDE_DIR} )
endif( SDL2_IMAGE_FOUND )

find_package( SDL2_ttf REQUIRED )
if( SDL2_TTF_FOUND )
  # Add development header files; SDL_ttf.h
  include_directories( ${SDL2_TTF_INCLUDE_DIR} )
endif( SDL2_TTF_FOUND )

# Common on all platforms
set(  NOM_SYSTEM_DEPS nomlib-core nomlib-file nomlib-math
      # FIXME: Needed only by nom::init and nom::quit
      ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} )

# Add and link the library
nom_add_library(  ${NOM_SYSTEM_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                  "${NOM_SYSTEM_SOURCE}" ""
                  "${NOM_SYSTEM_DEPS}" )

# Re-declare globally, so other build units that depend on us can refer to this
# variable
# set( NOM_SYSTEM_LIBRARY ${NOM_SYSTEM_LIBRARY} PARENT_SCOPE )
