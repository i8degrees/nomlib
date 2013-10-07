
# Project source files
set ( NOMLIB_SOURCE ${NOMLIB_SOURCE}
      ${SRC_DIR}/version.cpp # auto-generated
      ${SRC_DIR}/resources.cpp
      # audio
      ${SRC_DIR}/audio/AL/AudioDevice.cpp
      ${SRC_DIR}/audio/AL/Listener.cpp
      ${SRC_DIR}/audio/AL/OpenAL.cpp
      ${SRC_DIR}/audio/AL/SoundBuffer.cpp
      ${SRC_DIR}/audio/AL/SoundFile.cpp
      ${SRC_DIR}/audio/AL/SoundSource.cpp
      ${SRC_DIR}/audio/AL/Music.cpp
      ${SRC_DIR}/audio/AL/Sound.cpp
      # third-party graphics/hqx algorithm
      ${SRC_DIR}/graphics/hqx/hqx.cpp
      ${SRC_DIR}/graphics/hqx/hq2x.cpp
      ${SRC_DIR}/graphics/hqx/hq3x.cpp
      ${SRC_DIR}/graphics/hqx/hq4x.cpp
      # third-party graphics/scale2x algorithm
      ${SRC_DIR}/graphics/scale2x/scale2x.cpp
      ${SRC_DIR}/graphics/scale2x/scale3x.cpp
      ${SRC_DIR}/graphics/scale2x/scale4x.cpp
      # graphics
      ${SRC_DIR}/graphics/smart_ptr.cpp
      ${SRC_DIR}/graphics/Texture.cpp
      ${SRC_DIR}/graphics/Image.cpp
      ${SRC_DIR}/graphics/Window.cpp
      ${SRC_DIR}/graphics/Renderer.cpp
      ${SRC_DIR}/graphics/VideoMode.cpp
      ${SRC_DIR}/graphics/BitmapFont.cpp
      ${SRC_DIR}/graphics/TrueTypeFont.cpp
      ${SRC_DIR}/graphics/Gradient.cpp
      ${SRC_DIR}/graphics/Pixel.cpp
      #${SRC_DIR}/graphics/IDisplay.cpp
      # graphics/shapes
      ${SRC_DIR}/graphics/shapes/Point.cpp
      ${SRC_DIR}/graphics/shapes/Line.cpp
      ${SRC_DIR}/graphics/shapes/Rectangle.cpp
      # graphics/sprite
      ${SRC_DIR}/graphics/sprite/Sprite.cpp
      ${SRC_DIR}/graphics/sprite/SpriteBatch.cpp
      ${SRC_DIR}/graphics/sprite/AnimatedSprite.cpp
      ${SRC_DIR}/graphics/sprite/SpriteSheet.cpp
      # system
      ${SRC_DIR}/system/dialog_messagebox.cpp
      ${SRC_DIR}/system/EventDispatcher.cpp
      ${SRC_DIR}/system/Path.cpp
      ${SRC_DIR}/system/File.cpp
      ${SRC_DIR}/system/clock.cpp
      ${SRC_DIR}/system/FPS.cpp
      ${SRC_DIR}/system/GameStates.cpp
      ${SRC_DIR}/system/ObjectCache.cpp
      ${SRC_DIR}/system/SDL_App.cpp
      ${SRC_DIR}/system/Input.cpp
      ${SRC_DIR}/system/AnimationTimer.cpp
      ${SRC_DIR}/system/Timer.cpp
      ${SRC_DIR}/system/Sleep.cpp
      # math
      ${SRC_DIR}/math/helpers.cpp
      ${SRC_DIR}/math/Color.cpp
      ${SRC_DIR}/math/Coords.cpp
      ${SRC_DIR}/math/Transformable.cpp
      # gui
      ${SRC_DIR}/gui/Cursor.cpp
      ${SRC_DIR}/gui/MessageBox.cpp
      ${SRC_DIR}/gui/GrayFrame.cpp
    )

# Platform-specific source files
if ( PLATFORM_OSX OR PLATFORM_LINUX )
  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/unix/UnixFile.cpp )

  # nomJSON
  set ( NOMLIB_SOURCE
        ${NOMLIB_SOURCE}
        #${SRC_DIR}/json/Array.cpp
        ${SRC_DIR}/json/FileReader.cpp
        ${SRC_DIR}/json/FileWriter.cpp
        #${SRC_DIR}/json/Object.cpp
        #${SRC_DIR}/json/Value.cpp
      )
endif ( PLATFORM_OSX OR PLATFORM_LINUX )

if ( PLATFORM_OSX )
  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/osx/ResourcePath.cpp )
elseif ( PLATFORM_LINUX )
  # nothing to do be done
elseif ( PLATFORM_WINDOWS )

  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/windows/WinFile.cpp )

  # Add header files (*.hpp) and source files (*.cpp) to project inside MSVC++ IDE
  source_group  (
                  "include/nomlib"
                  FILES "*.hpp"
                )

  source_group ( "src" FILES ${NOMLIB_SOURCE} )

endif ( PLATFORM_OSX )

