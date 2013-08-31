
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
      # graphics
      ${SRC_DIR}/graphics/VideoMode.cpp
      ${SRC_DIR}/graphics/hqx/hqx.cpp
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
      # nomJSON
      #${SRC_DIR}/json/Array.cpp
      #${SRC_DIR}/json/FileReader.cpp
      #${SRC_DIR}/json/FileWriter.cpp
      #${SRC_DIR}/json/Object.cpp
      #${SRC_DIR}/json/Value.cpp
      # system
      ${SRC_DIR}/system/Path.cpp
      ${SRC_DIR}/system/File.cpp
      ${SRC_DIR}/system/clock.cpp
      ${SRC_DIR}/system/DialogMessageBox.cpp
      ${SRC_DIR}/system/FPS.cpp
      ${SRC_DIR}/system/GameStates.cpp
      ${SRC_DIR}/system/ObjectCache.cpp
      ${SRC_DIR}/system/SDL_App.cpp
      ${SRC_DIR}/system/Input.cpp
      ${SRC_DIR}/system/Timer.cpp
      ${SRC_DIR}/system/Sleep.cpp
      ${SRC_DIR}/system/random.cpp
      # math
      ${SRC_DIR}/math/Color.cpp
      ${SRC_DIR}/math/Coords.cpp
      ${SRC_DIR}/math/Transformable.cpp
      # gui
      ${SRC_DIR}/gui/Cursor.cpp
      ${SRC_DIR}/gui/MessageBox.cpp
      ${SRC_DIR}/gui/GrayFrame.cpp
      # sdl
      ${SRC_DIR}/sdl/utils.cpp
    )

# Platform-specific source files
if ( PLATFORM_OSX OR PLATFORM_LINUX )
  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/UnixFile.cpp )
endif ( PLATFORM_OSX OR PLATFORM_LINUX )

if ( PLATFORM_OSX )

  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/osx/ResourcePath.cpp )
  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/osx/DialogMessageBox.cpp )

elseif ( PLATFORM_LINUX )

  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/unix/DialogMessageBox.cpp )

elseif ( PLATFORM_WINDOWS )

  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/windows/DialogMessageBox.cpp )
  set ( NOMLIB_SOURCE ${NOMLIB_SOURCE} ${SRC_DIR}/system/WinFile.cpp )

endif ( PLATFORM_OSX )
