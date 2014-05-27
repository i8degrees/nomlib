# Project header and source files

# TODO: Break nomlib up into separate compilation units

file ( GLOB ROOT_HEADERS "${NOMLIB_HEADERS_DIR}/*.hpp" )
set ( ROOT_SOURCE
      ${SRC_DIR}/resources.cpp
      ${SRC_DIR}/CMakeLists.txt # added so it will show up within source_group
    )

# Audio module
file ( GLOB AUDIO_HEADERS "${NOMLIB_HEADERS_DIR}/audio/AL/*.hpp" )
set ( AUDIO_SOURCE
      ${SRC_DIR}/audio/AL/AudioDevice.cpp
      ${SRC_DIR}/audio/AL/Listener.cpp
      ${SRC_DIR}/audio/AL/Music.cpp
      ${SRC_DIR}/audio/AL/OpenAL.cpp
      ${SRC_DIR}/audio/AL/Sound.cpp
      ${SRC_DIR}/audio/AL/SoundBuffer.cpp
      ${SRC_DIR}/audio/AL/SoundFile.cpp
      ${SRC_DIR}/audio/AL/SoundSource.cpp
    )

# Graphics module
file ( GLOB GRAPHICS_HEADERS "${NOMLIB_HEADERS_DIR}/graphics/*.hpp" )
set ( GRAPHICS_SOURCE
      ${SRC_DIR}/graphics/Text.cpp
      ${SRC_DIR}/graphics/RendererInfo.cpp
      ${SRC_DIR}/graphics/Texture.cpp
      ${SRC_DIR}/graphics/Image.cpp
      ${SRC_DIR}/graphics/RenderWindow.cpp
      ${SRC_DIR}/graphics/Renderer.cpp
      ${SRC_DIR}/graphics/VideoMode.cpp
      ${SRC_DIR}/graphics/Gradient.cpp
      ${SRC_DIR}/graphics/fonts/BitmapFont.cpp
      ${SRC_DIR}/graphics/fonts/TrueTypeFont.cpp
      ${SRC_DIR}/graphics/fonts/Font.cpp
      ${SRC_DIR}/graphics/fonts/Glyph.cpp
    )

# third-party hqx algorithm
file ( GLOB GRAPHICS_HQX_HEADERS "${NOMLIB_HEADERS_DIR}/graphics/hqx/*.hpp" )
set ( GRAPHICS_HQX_SOURCE
      ${SRC_DIR}/graphics/hqx/hqx.cpp
      ${SRC_DIR}/graphics/hqx/hq2x.cpp
      ${SRC_DIR}/graphics/hqx/hq3x.cpp
      ${SRC_DIR}/graphics/hqx/hq4x.cpp
    )

# third-party scale2x algorithm
file ( GLOB GRAPHICS_SCALE2X_HEADERS "${NOMLIB_HEADERS_DIR}/graphics/scale2x/*.hpp" )
set ( GRAPHICS_SCALE2X_SOURCE
      ${SRC_DIR}/graphics/scale2x/scale2x.cpp
      ${SRC_DIR}/graphics/scale2x/scale3x.cpp
      ${SRC_DIR}/graphics/scale2x/scale4x.cpp
    )

# graphics/shapes
file ( GLOB GRAPHICS_SHAPES_HEADERS "${NOMLIB_HEADERS_DIR}/graphics/shapes/*.hpp" )
set ( GRAPHICS_SHAPES_SOURCE
      ${SRC_DIR}/graphics/shapes/Shape.cpp
      ${SRC_DIR}/graphics/shapes/Point.cpp
      ${SRC_DIR}/graphics/shapes/Line.cpp
      ${SRC_DIR}/graphics/shapes/Rectangle.cpp
    )

# graphics/sprite
file ( GLOB GRAPHICS_SPRITE_HEADERS "${NOMLIB_HEADERS_DIR}/graphics/sprite/*.hpp" )
set ( GRAPHICS_SPRITE_SOURCE
      ${SRC_DIR}/graphics/sprite/Sprite.cpp
      # ${SRC_DIR}/graphics/sprite/SpriteBatch.cpp
      # ${SRC_DIR}/graphics/sprite/AnimatedSprite.cpp
      # ${SRC_DIR}/graphics/sprite/SpriteSheet.cpp
    )

# User Interface module
file ( GLOB GUI_HEADERS "${NOMLIB_HEADERS_DIR}/gui/*.hpp" )
set ( GUI_SOURCE
      ${SRC_DIR}/gui/Cursor.cpp
      ${SRC_DIR}/gui/MessageBox.cpp
      ${SRC_DIR}/gui/GrayWindow.cpp
    )

# JSON module
file ( GLOB JSON_HEADERS "${NOMLIB_HEADERS_DIR}/serializers/*.hpp" )
set ( JSON_SOURCE
      ${SRC_DIR}/serializers/JsonCppSerializer.cpp
      ${SRC_DIR}/serializers/JsonCppDeserializer.cpp
    )

# XML module
file ( GLOB XML_HEADERS "${NOMLIB_HEADERS_DIR}/serializers/*.hpp" )
set ( XML_SOURCE
      ${SRC_DIR}/serializers/RapidXmlSerializer.cpp
      ${SRC_DIR}/serializers/RapidXmlDeserializer.cpp
    )

# Third-party RapidXml (headers only)
file ( GLOB RAPIDXML_HEADERS "${RAPIDXML_INCLUDE_DIR}/*.hpp" )

# Math module
file ( GLOB MATH_HEADERS "${NOMLIB_HEADERS_DIR}/math/*.hpp" )
set ( MATH_SOURCE
      ${SRC_DIR}/math/helpers.cpp
      ${SRC_DIR}/math/Size2.cpp
      ${SRC_DIR}/math/Point2.cpp
      ${SRC_DIR}/math/Point3.cpp
      ${SRC_DIR}/math/Rect.cpp
      ${SRC_DIR}/math/Color4.cpp
      ${SRC_DIR}/math/Transformable.cpp
    )

# System module
file ( GLOB SYSTEM_HEADERS "${NOMLIB_HEADERS_DIR}/system/*.hpp" )
file ( GLOB SYSTEM_OSX_HEADERS "${NOMLIB_HEADERS_DIR}/system/osx/*.hpp" )
file ( GLOB SYSTEM_UNIX_HEADERS "${NOMLIB_HEADERS_DIR}/system/unix/*.hpp" )
file ( GLOB SYSTEM_WINDOWS_HEADERS "${NOMLIB_HEADERS_DIR}/system/windows/*.hpp" )
set ( SYSTEM_SOURCE
      ${SRC_DIR}/system/helpers.cpp
      ${SRC_DIR}/system/IState.cpp
      ${SRC_DIR}/system/log.cpp
      ${SRC_DIR}/system/SDL_helpers.cpp
      ${SRC_DIR}/system/init.cpp
      ${SRC_DIR}/system/dialog_messagebox.cpp
      ${SRC_DIR}/system/Path.cpp
      ${SRC_DIR}/system/File.cpp
      ${SRC_DIR}/system/clock.cpp
      ${SRC_DIR}/system/FPS.cpp
      ${SRC_DIR}/system/StateMachine.cpp
      ${SRC_DIR}/system/SDLApp.cpp
      ${SRC_DIR}/system/EventHandler.cpp
      ${SRC_DIR}/system/SDLJoystick.cpp
      ${SRC_DIR}/system/Joystick.cpp
      ${SRC_DIR}/system/AnimationTimer.cpp
      ${SRC_DIR}/system/Timer.cpp
      ${SRC_DIR}/system/EventDispatcher.cpp
      ${SRC_DIR}/system/EventCallback.cpp
      ${SRC_DIR}/system/InputMapper/InputAction.cpp
      ${SRC_DIR}/system/InputMapper/InputActionMapper.cpp
      ${SRC_DIR}/system/InputMapper/InputStateMapper.cpp

      # Resource management
      ${SRC_DIR}/system/resource_handlers.cpp
      ${SRC_DIR}/system/ResourceFile.cpp
    )

file ( GLOB PTREE_HEADERS "${NOMLIB_HEADERS_DIR}/ptree/*.hpp" )
set ( PTREE_SOURCE
      ${SRC_DIR}/ptree/Value.cpp
      ${SRC_DIR}/ptree/VString.cpp
      ${SRC_DIR}/ptree/ValueIteratorBase.cpp
      ${SRC_DIR}/ptree/ValueIterator.cpp
      ${SRC_DIR}/ptree/ValueConstIterator.cpp
    )

file ( GLOB TEMPLATE_HEADERS "${NOMLIB_SHARED_SUPPORT_DIR}/*.hpp" )
set ( TEMPLATE_SOURCE
      ${NOMLIB_SHARED_SUPPORT_DIR}/Template.cpp
    )

file( GLOB EXAMPLES_TEMPLATE_HEADERS "${NOMLIB_SHARED_SUPPORT_DIR}/*.hpp" )
set(  EXAMPLES_TEMPLATE_SOURCE
      ${NOMLIB_SHARED_SUPPORT_DIR}/ExamplesTemplate.cpp
    )

# Platform-specific source files
if ( PLATFORM_OSX OR PLATFORM_LINUX )
  set ( SYSTEM_SOURCE ${SYSTEM_SOURCE} "${SRC_DIR}/system/unix/UnixFile.cpp" )
endif ( PLATFORM_OSX OR PLATFORM_LINUX )

if ( PLATFORM_OSX )
  # Stub
elseif ( PLATFORM_LINUX )
  # Stub
elseif ( PLATFORM_WINDOWS )
  set ( SYSTEM_SOURCE ${SYSTEM_SOURCE} "${SRC_DIR}/system/windows/WinFile.cpp" )
endif ( PLATFORM_OSX )

if( NOM_BUILD_CORE_UNIT )
  set ( NOMLIB_HEADERS
        ${NOMLIB_HEADERS}
        ${ROOT_HEADERS}
      )
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        ${ROOT_SOURCE}
      )
endif( NOM_BUILD_CORE_UNIT )

if( NOM_BUILD_AUDIO_UNIT )
  set ( NOMLIB_HEADERS
        ${NOMLIB_HEADERS}
        ${AUDIO_HEADERS}
      )
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        ${AUDIO_SOURCE}
      )
endif( NOM_BUILD_AUDIO_UNIT )

if( NOM_BUILD_GRAPHICS_UNIT )
  set ( NOMLIB_HEADERS
        ${NOMLIB_HEADERS}
        ${GRAPHICS_HEADERS}
        ${GRAPHICS_SHAPES_HEADERS}
        ${GRAPHICS_SPRITE_HEADERS}
      )
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        ${GRAPHICS_SOURCE}
        ${GRAPHICS_SHAPES_SOURCE}
        ${GRAPHICS_SPRITE_SOURCE}
      )
endif( NOM_BUILD_GRAPHICS_UNIT )

if( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )
  set ( NOMLIB_HEADERS
        ${NOMLIB_HEADERS}
        ${GRAPHICS_HQX_HEADERS}
        ${GRAPHICS_SCALE2X_HEADERS}
      )
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        ${GRAPHICS_HQX_SOURCE}
        ${GRAPHICS_SCALE2X_SOURCE}
      )
endif( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )

if( NOM_BUILD_GUI_UNIT )
  set ( NOMLIB_HEADERS
        ${NOMLIB_HEADERS}
        ${GUI_HEADERS}
      )
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        ${GUI_SOURCE}
      )
endif( NOM_BUILD_GUI_UNIT )

if( NOM_BUILD_JSON_UNIT )
  set ( NOMLIB_HEADERS
        ${NOMLIB_HEADERS}
        ${JSON_HEADERS}
        ${JSONCPP_INCLUDE_DIR}
        ${SRC_DIR}/graphics/sprite/SpriteSheet.cpp
        ${SRC_DIR}/graphics/sprite/SpriteBatch.cpp
        ${SRC_DIR}/graphics/sprite/AnimatedSprite.cpp
      )
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        ${JSON_SOURCE}
        ${JSONCPP_SOURCE}
      )
endif( NOM_BUILD_JSON_UNIT )

if( NOM_BUILD_XML_UNIT )
  set ( NOMLIB_HEADERS
        ${NOMLIB_HEADERS}
        ${RAPIDXML_HEADERS}
        ${XML_HEADERS}
      )
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        ${XML_SOURCE}
      )
endif( NOM_BUILD_XML_UNIT )

if( NOM_BUILD_MATH_UNIT )
  set ( NOMLIB_HEADERS
        ${NOMLIB_HEADERS}
        ${MATH_HEADERS}
      )
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        ${MATH_SOURCE}
      )
endif( NOM_BUILD_MATH_UNIT )

if( NOM_BUILD_PTREE_UNIT )
  set ( NOMLIB_HEADERS
        ${NOMLIB_HEADERS}
        ${PTREE_HEADERS}
      )
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        ${PTREE_SOURCE}
      )
endif( NOM_BUILD_PTREE_UNIT )

if( NOM_BUILD_SYSTEM_UNIT )
  set ( NOMLIB_HEADERS
        ${NOMLIB_HEADERS}
        ${SYSTEM_HEADERS}
        ${SYSTEM_OSX_HEADERS}
        ${SYSTEM_UNIX_HEADERS}
        ${SYSTEM_WINDOWS_HEADERS}
      )
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        ${SYSTEM_SOURCE}
      )
endif( NOM_BUILD_SYSTEM_UNIT )

if( NOM_BUILD_TEMPLATES_UNIT )
  if( NOM_BUILD_CORE_UNIT AND NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )
    set ( NOMLIB_HEADERS
          ${NOMLIB_HEADERS}
          ${TEMPLATE_HEADERS}
        )
    set ( NOMLIB_SOURCE
          ${NOMLIB_SOURCE}
          ${TEMPLATE_SOURCE}
          ${EXAMPLES_TEMPLATE_SOURCE}
        )
  endif( NOM_BUILD_CORE_UNIT AND NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )
endif( NOM_BUILD_TEMPLATES_UNIT )

# Add header files (*.hpp) and source files (*.cpp) to IDE project list
# (For those that use Xcode, MSVC++, etc.)
source_group ( "include" FILES ${ROOT_HEADERS} )
source_group ( "include\\audio" FILES ${AUDIO_HEADERS} )
source_group ( "include\\graphics" FILES ${GRAPHICS_HEADERS} )
source_group ( "include\\graphics\\hqx" FILES ${GRAPHICS_HQX_HEADERS} )
source_group ( "include\\graphics\\scale2x" FILES ${GRAPHICS_SCALE2X_HEADERS} )
source_group ( "include\\graphics\\shapes" FILES ${GRAPHICS_SHAPES_HEADERS} )
source_group ( "include\\graphics\\sprite" FILES ${GRAPHICS_SPRITE_HEADERS} )
source_group ( "include\\gui" FILES ${GUI_HEADERS} )
source_group ( "include\\json" FILES ${JSON_HEADERS} )
source_group ( "third-party\\common\\jsoncpp" FILES ${JSONCPP_INCLUDE_DIR} )
source_group ( "third-party\\common\\rapidxml" FILES ${RAPIDXML_INCLUDE_DIR} )
source_group ( "include\\math" FILES ${MATH_HEADERS} )
source_group ( "include\\system" FILES ${SYSTEM_HEADERS} )
source_group ( "include\\system\\osx" FILES ${SYSTEM_OSX_HEADERS} )
source_group ( "include\\system\\unix" FILES ${SYSTEM_UNIX_HEADERS} )
source_group ( "include\\system\\windows" FILES ${SYSTEM_WINDOWS_HEADERS} )

source_group ( "src" FILES ${ROOT_SOURCE} )
source_group ( "src\\audio" FILES ${AUDIO_SOURCE} )
source_group ( "src\\graphics" FILES ${GRAPHICS_SOURCE} )
source_group ( "src\\graphics\\hqx" FILES ${GRAPHICS_HQX_SOURCE} )
source_group ( "src\\graphics\\scale2x" FILES ${GRAPHICS_SCALE2X_SOURCE} )
source_group ( "src\\graphics\\shapes" FILES ${GRAPHICS_SHAPES_SOURCE} )
source_group ( "src\\graphics\\sprite" FILES ${GRAPHICS_SPRITE_SOURCE} )
source_group ( "src\\gui" FILES ${GUI_SOURCE} )
source_group ( "src\\json" FILES ${JSON_SOURCE} )
source_group ( "third-party\\common\\jsoncpp" FILES ${JSONCPP_SOURCE} )
source_group ( "src\\math" FILES ${MATH_SOURCE} )
source_group ( "src\\system" FILES ${SYSTEM_SOURCE} )

# FIXME
# They do not show up under the system/<platform> filter (...look at the variables used!)
source_group ( "src\\system\\osx" FILES ${SYSTEM_OSX_SOURCE} )
source_group ( "src\\system\\unix" FILES ${SYSTEM_UNIX_SOURCE} )
source_group ( "src\\system\\windows" FILES ${SYSTEM_WINDOWS_SOURCE} )
