
# Project source files
set ( NOMLIB_SOURCE ${NOMLIB_SOURCE}
      # audio
      ${SRC_DIR}/audio/AL/AudioDevice.cpp
      ${SRC_DIR}/audio/AL/Listener.cpp
      ${SRC_DIR}/audio/AL/OpenAL.cpp
      ${SRC_DIR}/audio/AL/SoundBuffer.cpp
      ${SRC_DIR}/audio/AL/SoundFile.cpp
      ${SRC_DIR}/audio/AL/SoundSource.cpp
      ${SRC_DIR}/audio/AL/Music.cpp
      ${SRC_DIR}/audio/AL/Sound.cpp
      # graphics
      ${SRC_DIR}/graphics/Text.cpp
      ${SRC_DIR}/graphics/BitmapFont.cpp
      ${SRC_DIR}/graphics/TrueTypeFont.cpp
      ${SRC_DIR}/graphics/IDisplay.cpp
      ${SRC_DIR}/graphics/Canvas.cpp
      ${SRC_DIR}/graphics/Display.cpp
      ${SRC_DIR}/graphics/Gradient.cpp
      ${SRC_DIR}/graphics/Image.cpp
      ${SRC_DIR}/graphics/Line.cpp
      ${SRC_DIR}/graphics/Pixel.cpp
      ${SRC_DIR}/graphics/Rectangle.cpp
      ${SRC_DIR}/graphics/Sprite.cpp
      # system
      ${SRC_DIR}/system/File.cpp
      ${SRC_DIR}/system/Clock.cpp
      ${SRC_DIR}/system/FPS.cpp
      ${SRC_DIR}/system/GameStates.cpp
      ${SRC_DIR}/system/Logger.cpp
      ${SRC_DIR}/system/ObjectCache.cpp
      ${SRC_DIR}/system/SDL_App.cpp
      ${SRC_DIR}/system/Input.cpp
      ${SRC_DIR}/system/Timer.cpp
      ${SRC_DIR}/system/Sleep.cpp
      # math
      ${SRC_DIR}/math/Color.cpp
      ${SRC_DIR}/math/Coords.cpp
      ${SRC_DIR}/math/Transformable.cpp
      # gui
      ${SRC_DIR}/gui/Cursor.cpp
      ${SRC_DIR}/gui/MessageBox.cpp
      # sdl
      ${SRC_DIR}/sdl/utils.cpp
    )

# Platform-specific source files
if ( PLATFORM_OSX )

  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/OSX/ResourcePath.cpp )
  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/UnixFile.cpp )

elseif ( PLATFORM_LINUX )

  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/UnixFile.cpp )

elseif ( PLATFORM_WINDOWS )

  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/WinFile.cpp )

endif ( PLATFORM_OSX )