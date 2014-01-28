# Project header and source files

file ( GLOB ROOT_HEADERS "${NOMLIB_HEADERS_DIR}/*.hpp" )
set ( ROOT_SOURCE
      ${SRC_DIR}/resources.cpp
      ${SRC_DIR}/version.cpp # auto-generated
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
      ${SRC_DIR}/graphics/Window.cpp
      ${SRC_DIR}/graphics/Renderer.cpp
      ${SRC_DIR}/graphics/VideoMode.cpp
      ${SRC_DIR}/graphics/Gradient.cpp
      ${SRC_DIR}/graphics/fonts/BitmapFont.cpp
      ${SRC_DIR}/graphics/fonts/TrueTypeFont.cpp
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
      ${SRC_DIR}/graphics/sprite/SpriteBatch.cpp
      ${SRC_DIR}/graphics/sprite/AnimatedSprite.cpp
      ${SRC_DIR}/graphics/sprite/SpriteSheet.cpp
    )

# User Interface module
file ( GLOB GUI_HEADERS "${NOMLIB_HEADERS_DIR}/gui/*.hpp" )
set ( GUI_SOURCE
      ${SRC_DIR}/gui/Cursor.cpp
      ${SRC_DIR}/gui/MessageBox.cpp
      ${SRC_DIR}/gui/GrayFrame.cpp
    )

# JSON module
file ( GLOB JSON_HEADERS "${NOMLIB_HEADERS_DIR}/json/*.hpp" )
set ( JSON_SOURCE
      #${SRC_DIR}/json/Object.cpp
      ${SRC_DIR}/json/FileReader.cpp
      ${SRC_DIR}/json/FileWriter.cpp
      ${SRC_DIR}/json/Value.cpp
    )

# third-party jsoncpp library
file ( GLOB JSONCPP_HEADERS "${JSONCPP_HEADERS_DIR}/*.hpp" )
set ( JSONCPP_SOURCE
      ${SRC_DIR}/json/jsoncpp/jsoncpp.cpp
    )

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
      ${SRC_DIR}/system/IState.cpp
      ${SRC_DIR}/system/log.cpp
      ${SRC_DIR}/system/SDL_helpers.cpp
      ${SRC_DIR}/system/init.cpp
      ${SRC_DIR}/system/dialog_messagebox.cpp
      ${SRC_DIR}/system/EventDispatcher.cpp
      ${SRC_DIR}/system/Path.cpp
      ${SRC_DIR}/system/File.cpp
      ${SRC_DIR}/system/clock.cpp
      ${SRC_DIR}/system/FPS.cpp
      ${SRC_DIR}/system/StateMachine.cpp
      ${SRC_DIR}/system/ObjectCache.cpp
      ${SRC_DIR}/system/SDLApp.cpp
      ${SRC_DIR}/system/Input.cpp
      ${SRC_DIR}/system/AnimationTimer.cpp
      ${SRC_DIR}/system/Timer.cpp
      ${SRC_DIR}/system/Sleep.cpp
    )

# Platform-specific source files
if ( PLATFORM_OSX OR PLATFORM_LINUX )
  set ( SYSTEM_SOURCE ${SYSTEM_SOURCE} "${SRC_DIR}/system/unix/UnixFile.cpp" )
endif ( PLATFORM_OSX OR PLATFORM_LINUX )

if ( PLATFORM_OSX )

  set ( SYSTEM_SOURCE ${SYSTEM_SOURCE} "${SRC_DIR}/system/osx/ResourcePath.cpp" )

elseif ( PLATFORM_LINUX )
  # nothing to be done
elseif ( PLATFORM_WINDOWS )
  set ( SYSTEM_SOURCE ${SYSTEM_SOURCE} "${SRC_DIR}/system/windows/WinFile.cpp" )
  set ( SYSTEM_SOURCE ${SYSTEM_SOURCE} "${SRC_DIR}/system/windows/ResourcePath.cpp" )

endif ( PLATFORM_OSX )

# Full nomlib project header files (*.hpp; *.h)
set ( NOMLIB_HEADERS
      ${ROOT_HEADERS}
      ${AUDIO_HEADERS}
      ${GRAPHICS_HEADERS}
      ${GRAPHICS_HQX_HEADERS}
      ${GRAPHICS_SCALE2X_HEADERS}
      ${GRAPHICS_SHAPES_HEADERS}
      ${GRAPHICS_SPRITE_HEADERS}
      ${GUI_HEADERS}
      ${JSON_HEADERS}
      ${JSONCPP_HEADERS}
      ${MATH_HEADERS}
      ${SYSTEM_HEADERS}
      ${SYSTEM_OSX_HEADERS}
      ${SYSTEM_UNIX_HEADERS}
      ${SYSTEM_WINDOWS_HEADERS}
    )

if ( NOM_BUILD_MINI )
  # Build an absolute minimum nomlib -- for testing new CMake options, etc.
  set ( NOMLIB_SOURCE ${ROOT_SOURCE} )
else ( NOT NOM_BUILD_MINI )
  # Full nomlib project
  set ( NOMLIB_SOURCE
        ${ROOT_SOURCE}
        ${AUDIO_SOURCE}
        ${GRAPHICS_SOURCE}
        ${GRAPHICS_HQX_SOURCE}
        ${GRAPHICS_SCALE2X_SOURCE}
        ${GRAPHICS_SHAPES_SOURCE}
        ${GRAPHICS_SPRITE_SOURCE}
        ${GUI_SOURCE}
        ${JSON_SOURCE}
        ${JSONCPP_SOURCE}
        ${MATH_SOURCE}
        ${SYSTEM_SOURCE}
      )
endif ( NOM_BUILD_MINI )

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
source_group ( "include\\json\\jsoncpp" FILES ${JSONCPP_HEADERS} )
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
source_group ( "src\\json\\jsoncpp" FILES ${JSONCPP_SOURCE} )
source_group ( "src\\math" FILES ${MATH_SOURCE} )
source_group ( "src\\system" FILES ${SYSTEM_SOURCE} )

# FIXME
# They do not show up under the system/<platform> filter (...look at the variables used!)
source_group ( "src\\system\\osx" FILES ${SYSTEM_OSX_SOURCE} )
source_group ( "src\\system\\unix" FILES ${SYSTEM_UNIX_SOURCE} )
source_group ( "src\\system\\windows" FILES ${SYSTEM_WINDOWS_SOURCE} )
