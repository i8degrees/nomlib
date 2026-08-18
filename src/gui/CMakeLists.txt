# nomlib-gui library

# Platform-specific implementations & dependencies

# if( NOM_USE_LIBROCKET_LUA )
#   find_package( Lua REQUIRED )
#   if( LUA_FOUND )
#     include_directories( ${LUA_INCLUDE_DIR} )
#   endif( LUA_FOUND )
# endif( NOM_USE_LIBROCKET_LUA )

# Add and link the library
add_library(nomlib-gui
  ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
)

set_target_properties(nomlib-gui
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEBUG_POSTFIX "-d"
)

if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
  # Skip dllimport/dllexport logic when building as a static library target
  target_compile_definitions(nomlib-gui
    PUBLIC
      NOM_STATIC_DEFINE
  )
  # Ensure that our target can be cleanly combined or linked by shared
  # library targets; this adds -fPIC to the build for this unit
  set_target_properties(nomlib-gui
    PROPERTIES
      POSITION_INDEPENDENT_CODE ON
  )
endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

target_sources(nomlib-gui
  PRIVATE
  ${SRC_DIR}/gui/RocketSDL2RenderInterface.cpp
  ${SRC_DIR}/gui/RocketSDL2SystemInterface.cpp
  ${SRC_DIR}/gui/RocketFileInterface.cpp
  ${SRC_DIR}/gui/UIEventListener.cpp
  ${SRC_DIR}/gui/DecoratorInstancerFinalFantasyFrame.cpp
  ${SRC_DIR}/gui/DecoratorFinalFantasyFrame.cpp
  ${SRC_DIR}/gui/DecoratorPhotograph.cpp
  ${SRC_DIR}/gui/DecoratorInstancerSprite.cpp
  ${SRC_DIR}/gui/DecoratorSpriteBatch.cpp
  ${SRC_DIR}/gui/UIWidget.cpp
  ${SRC_DIR}/gui/UIMessageBox.cpp
  ${SRC_DIR}/gui/UIQuestionDialogBox.cpp
  ${SRC_DIR}/gui/UIDataViewList.cpp
  # ${SRC_DIR}/gui/DOM.cpp
  ${SRC_DIR}/gui/RocketUtilities.cpp
  ${SRC_DIR}/gui/UIContext.cpp
  ${SRC_DIR}/gui/init_librocket.cpp
  ${SRC_DIR}/gui/UIContextEventHandler.cpp
  # Decorator helpers
  ${SRC_DIR}/gui/FinalFantasyFrame.cpp
  ${SRC_DIR}/gui/Drawables.cpp
  ${SRC_DIR}/gui/Decorator.cpp
  ${SRC_DIR}/gui/MinimalDecorator.cpp
  ${SRC_DIR}/gui/FinalFantasyDecorator.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${HDR_BASE}
    FILES
    ${HDR_BASE}/gui.hpp
    ${HDR_BASE}/gui/RocketSDL2RenderInterface.hpp
    ${HDR_BASE}/gui/RocketSDL2RenderInterface.hpp
    ${HDR_BASE}/gui/RocketSDL2SystemInterface.hpp
    ${HDR_BASE}/gui/RocketFileInterface.hpp
    ${HDR_BASE}/gui/UIEventListener.hpp
    ${HDR_BASE}/gui/DecoratorInstancerFinalFantasyFrame.hpp
    ${HDR_BASE}/gui/DecoratorFinalFantasyFrame.hpp
    ${HDR_BASE}/gui/DecoratorPhotograph.hpp
    ${HDR_BASE}/gui/DecoratorInstancerSprite.hpp
    ${HDR_BASE}/gui/DecoratorSpriteBatch.hpp
    ${HDR_BASE}/gui/UIWidget.hpp
    ${HDR_BASE}/gui/UIMessageBox.hpp
    ${HDR_BASE}/gui/UIQuestionDialogBox.hpp
    ${HDR_BASE}/gui/UIDataViewList.hpp
    ${HDR_BASE}/gui/DOM.hpp
    ${HDR_BASE}/gui/RocketUtilities.hpp
    ${HDR_BASE}/gui/UIContext.hpp
    ${HDR_BASE}/gui/init_librocket.hpp
    ${HDR_BASE}/gui/UIContextEventHandler.hpp
    # Decorator helpers
    ${HDR_BASE}/gui/FinalFantasyDecorator.hpp
    ${HDR_BASE}/gui/FinalFantasyFrame.hpp
    ${HDR_BASE}/gui/Drawables.hpp
    ${HDR_BASE}/gui/IDecorator.hpp
    ${HDR_BASE}/gui/Decorator.hpp
    ${HDR_BASE}/gui/MinimalDecorator.hpp
)

target_include_directories(nomlib-gui
  # INTERFACE
  # Path used when building this project locally
  # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/nomlib>
  # Path used by downstream consumers after the project is installed
  # $<INSTALL_INTERFACE:include/nomlib>
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  ${SDL2_INCLUDE_DIR}
  ${libRocket_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
)

target_compile_features(nomlib-gui
  PRIVATE
    cxx_std_14
)

target_link_libraries(nomlib-gui
  PUBLIC
    nomlib-graphics
  PRIVATE
    project_warnings
    #
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    # libRocket requires OpenGL
    #OpenGL::GL
    # FIXME(JEFF): We are unable to build ttcards without changing this dep
    # to the one below this line. This may not be OK on macos builds!
    GL
    $<IF:$<TARGET_EXISTS:RocketCore>,RocketCore,RocketCore-static>
    $<IF:$<TARGET_EXISTS:RocketControls>,RocketControls,RocketControls-static>
    $<IF:$<TARGET_EXISTS:RocketDebugger>,RocketDebugger,RocketDebugger-static>
)

# MacOSX Framework bundle support
set_target_properties(nomlib-gui
  PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_INFO_PLIST
      "${CMAKE_TEMPLATE_PATH}/Info.plist.in"
    MACOSX_FRAMEWORK_NAME
      nomlib-gui
    MACOSX_FRAMEWORK_BUNDLE_VERSION
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_FRAMEWORK_IDENTIFIER
      "net.i8degrees.nomlib-gui"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
      ""
    MACOSX_RPATH TRUE
    #INSTALL_RPATH
      #@executable_path/../Frameworks
)

# MacOSX Framework bundle support - public header files
if(APPLE)
  install(TARGETS nomlib-gui
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_LIBDIR}/nomlib-gui.framework/Headers"
      COMPONENT Development
  )
endif(APPLE)

install(TARGETS nomlib-gui
  EXPORT libtarget
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT Development
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Runtime
)
