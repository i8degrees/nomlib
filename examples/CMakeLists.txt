# TODO: Remove checks for the necessary build units in order to compile
# examples. In hindsight, this check is worse than no check at all, because we
# have no warning / error when the build unit check is FALSE -- this can lead
# to confusion.
#
# TODO: consider adding a custom POST_BUILD command here so that we do not have
# to manually execute the 'install' target in order to get the example's
# Resources, etc. to copy.
#
#   OS X with cmake generated Xcode project files:
#
# xcodebuild -target install
#
#   MSBuild with Visual Studio 2012 project files:
#
# msbuild /t:build INSTALL.vcxproj
#
#   See also,
#
# http://stackoverflow.com/questions/13422202/adding-distinct-xcode-resources-for-multiple-executables-using-cmake
# http://stackoverflow.com/questions/6077810/how-to-correctly-add-resources-to-a-project?rq=1

set( NOM_BUILD_APP_EXAMPLE ON )
set( NOM_BUILD_AUDIO_EXAMPLE ON )
set( NOM_BUILD_DEVICE_INFO_EXAMPLE ON )
set( NOM_BUILD_EVENTS_EXAMPLE ON )
set( NOM_BUILD_JOYSTICK_EXAMPLE ON )
set( NOM_BUILD_GAME_CONTROLLER_EXAMPLE ON )
set( NOM_BUILD_FONTS_EXAMPLE ON )
set( NOM_BUILD_MOUSE_CURSORS_EXAMPLE ON )
set( NOM_BUILD_MACROS_EXAMPLE ON )
set(NOM_BUILD_GOL_EXAMPLE ON)

# Use common build output directories for MSVCPP && Xcode project files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")

# Always built
add_subdirectory(nom_jsonpp)

if(NOM_BUILD_APP_EXAMPLE) # nom_app_ex
  add_subdirectory(nom_app_ex)
endif(NOM_BUILD_APP_EXAMPLE)

if(NOM_BUILD_AUDIO_EXAMPLE AND NOM_BUILD_AUDIO_UNIT) # nom_sfx
  add_subdirectory(nom_sfx)
endif(NOM_BUILD_AUDIO_EXAMPLE AND NOM_BUILD_AUDIO_UNIT)

if(NOM_BUILD_EVENTS_EXAMPLE) # nom_ev
  add_subdirectory(nom_ev)
endif(NOM_BUILD_EVENTS_EXAMPLE)

if(NOM_BUILD_DEVICE_INFO_EXAMPLE) # nom_devinfo
  add_subdirectory(nom_devinfo)
endif(NOM_BUILD_DEVICE_INFO_EXAMPLE)

if(NOM_BUILD_FONTS_EXAMPLE) # nom_fontr
  add_subdirectory(nom_font)
endif(NOM_BUILD_FONTS_EXAMPLE)

if(NOM_BUILD_GAME_CONTROLLER_EXAMPLE) # nom_joystickgame_ev
  add_subdirectory(nom_gamecontroller_ev)
endif(NOM_BUILD_GAME_CONTROLLER_EXAMPLE)

if( NOM_BUILD_JOYSTICK_EXAMPLE ) # nom_joystick_ev
  add_subdirectory(nom_joystick_ev)
endif( NOM_BUILD_JOYSTICK_EXAMPLE )

if(NOM_BUILD_MOUSE_CURSORS_EXAMPLE) # nom_cursors
  add_subdirectory(nom_cursors)
endif(NOM_BUILD_MOUSE_CURSORS_EXAMPLE)

if(NOM_BUILD_MACROS_EXAMPLE) # nom_macros
  add_subdirectory(nom_macros)
endif(NOM_BUILD_MACROS_EXAMPLE)

if(NOM_BUILD_GOL_EXAMPLE)
  add_subdirectory(GameOfLife)
endif()

install(DIRECTORY "${PROJECT_SOURCE_DIR}/Resources/examples"
  DESTINATION  "${CMAKE_INSTALL_DATAROOTDIR}/nomlib/Resources"
  COMPONENT Examples
  PATTERN ".*" EXCLUDE # dot files
)

# FIXME(JEFF): The app example requires the tests resources path
install(DIRECTORY "${PROJECT_SOURCE_DIR}/Resources/tests"
  DESTINATION  "${CMAKE_INSTALL_DATAROOTDIR}/nomlib/Resources"
  COMPONENT Examples
  PATTERN ".*" EXCLUDE # dot files
)
