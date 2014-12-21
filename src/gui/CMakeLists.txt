# nomlib-gui library

# Library headers inclusion path
include_directories( "${INC_ROOT_DIR}" )

# Library name
set( NOM_GUI_LIBRARY "nomlib-gui" )

set(  NOM_GUI_SOURCE
      ${INC_DIR}/gui.hpp

      ${SRC_DIR}/gui/RocketSDL2RenderInterface.cpp
      ${INC_DIR}/gui/RocketSDL2RenderInterface.hpp

      ${SRC_DIR}/gui/RocketSDL2SystemInterface.cpp
      ${INC_DIR}/gui/RocketSDL2SystemInterface.hpp

      ${SRC_DIR}/gui/RocketFileInterface.cpp
      ${INC_DIR}/gui/RocketFileInterface.hpp

      ${SRC_DIR}/gui/UIEventListener.cpp
      ${INC_DIR}/gui/UIEventListener.hpp

      ${SRC_DIR}/gui/DecoratorInstancerFinalFantasyFrame.cpp
      ${INC_DIR}/gui/DecoratorInstancerFinalFantasyFrame.hpp

      ${SRC_DIR}/gui/DecoratorFinalFantasyFrame.cpp
      ${INC_DIR}/gui/DecoratorFinalFantasyFrame.hpp

      ${SRC_DIR}/gui/DecoratorPhotograph.cpp
      ${INC_DIR}/gui/DecoratorPhotograph.hpp

      ${SRC_DIR}/gui/DecoratorInstancerSprite.cpp
      ${INC_DIR}/gui/DecoratorInstancerSprite.hpp

      ${SRC_DIR}/gui/DecoratorSpriteBatch.cpp
      ${INC_DIR}/gui/DecoratorSpriteBatch.hpp

      ${SRC_DIR}/gui/UIWidget.cpp
      ${INC_DIR}/gui/UIWidget.hpp

      ${SRC_DIR}/gui/UIMessageBox.cpp
      ${INC_DIR}/gui/UIMessageBox.hpp

      ${SRC_DIR}/gui/UIQuestionDialogBox.cpp
      ${INC_DIR}/gui/UIQuestionDialogBox.hpp

      ${SRC_DIR}/gui/UIDataViewList.cpp
      ${INC_DIR}/gui/UIDataViewList.hpp

      # ${SRC_DIR}/gui/DOM.cpp
      # ${INC_DIR}/gui/DOM.hpp

      ${SRC_DIR}/gui/RocketUtilities.cpp
      ${INC_DIR}/gui/RocketUtilities.hpp

      ${SRC_DIR}/gui/UIContext.cpp
      ${INC_DIR}/gui/UIContext.hpp

      ${SRC_DIR}/gui/init_librocket.cpp
      ${INC_DIR}/gui/init_librocket.hpp

      ${SRC_DIR}/gui/UIContextEventHandler.cpp
      ${INC_DIR}/gui/UIContextEventHandler.hpp

      # Decorator helpers
      ${SRC_DIR}/gui/FinalFantasyFrame.cpp
      ${INC_DIR}/gui/FinalFantasyFrame.hpp

      ${SRC_DIR}/gui/Drawables.cpp
      ${INC_DIR}/gui/Drawables.hpp

      ${INC_DIR}/gui/IDecorator.hpp

      ${SRC_DIR}/gui/Decorator.cpp
      ${INC_DIR}/gui/Decorator.hpp

      ${SRC_DIR}/gui/MinimalDecorator.cpp
      ${INC_DIR}/gui/MinimalDecorator.hpp
      ${SRC_DIR}/gui/FinalFantasyDecorator.cpp
)

# Common dependencies on all platforms
set( NOM_GUI_DEPS nomlib-graphics )

# Platform-specific implementations & dependencies

find_package( SDL2_image REQUIRED )
if( SDL2_IMAGE_FOUND )
  # Add development header files; SDL_image.h
  include_directories( ${SDL2_IMAGE_INCLUDE_DIR} )
endif( SDL2_IMAGE_FOUND )

find_package( OpenGL REQUIRED )
if( OPENGL_FOUND )
  # Add development header files; gl.h
  include_directories( ${OPENGL_INCLUDE_DIR} )
  list( APPEND NOM_GUI_DEPS ${OPENGL_LIBRARY} )
endif( OPENGL_FOUND )

# if( NOM_USE_LIBROCKET_LUA )
#   find_package( Lua REQUIRED )
#   if( LUA_FOUND )
#     include_directories( ${LUA_INCLUDE_DIR} )
#   endif( LUA_FOUND )
# endif( NOM_USE_LIBROCKET_LUA )

find_package( LibRocket REQUIRED )
if( LIBROCKET_FOUND )
  # Add development header files
  include_directories( ${LIBROCKET_INCLUDE_DIRS} )
  list( APPEND NOM_GUI_DEPS ${LIBROCKET_LIBRARIES} )
endif( LIBROCKET_FOUND )

# Add and link the library
nom_add_library(  ${NOM_GUI_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                  "${NOM_GUI_SOURCE}" ""
                  "${NOM_GUI_DEPS}" )

# Re-declare globally, so other build units that depend on us can refer to this
# variable
# set( NOM_GUI_LIBRARY ${NOM_GUI_LIBRARY} PARENT_SCOPE )
