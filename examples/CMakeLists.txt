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
cmake_minimum_required ( VERSION 2.6 )

link_libraries ( ${PROJECT_NAME} )

if ( PLATFORM_WINDOWS )
  # Build as console app -- shows cmd.exe window for logging -- only when
  # debugging is toggled on
  if ( NOT DEBUG )
    set ( EXECUTABLE_FLAGS "WIN32" ) # Implies WIN_MAIN
  else ( DEBUG )
    # MSVCPP should default to Console if we set EXECUTABLE_FLAGS empty
    set ( EXECUTABLE_FLAGS "" )
  endif ( NOT DEBUG )

  # This workaround is to get the file paths to match up on all platforms -_-
  # build/examples/Debug under MSVC++
  set ( EXAMPLES_INSTALL_PEFIX "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}" )
else ( NOT PLATFORM_WINDOWS )

  if ( CMAKE_GENERATOR STREQUAL "Xcode" )

    # Match build/Debug/examples
    set ( EXAMPLES_INSTALL_PEFIX "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}" )

  else ( NOT CMAKE_GENERATOR STREQUAL "Xcode" )

    # We prefer lower-case file paths
    string ( TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE )

    # Match build/examples/debug (from MSVC++ configuration)
    set ( EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/examples/${CMAKE_BUILD_TYPE}" )
    set ( EXAMPLES_INSTALL_PEFIX "${EXECUTABLE_OUTPUT_PATH}" )
  endif ( CMAKE_GENERATOR STREQUAL "Xcode" )

endif ( PLATFORM_WINDOWS )

# Install example resources
if( NOT NOM_BUILD_MINI_UNIT )

  if( NOM_BUILD_CORE_UNIT AND NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_GUI_UNIT AND NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT AND NOM_BUILD_JSON_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )
    add_executable ( app "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/app/app.cpp" )

    install ( DIRECTORY
              "${EXAMPLES_SRC_DIR}/app/Resources"
              DESTINATION
              "${EXAMPLES_INSTALL_PEFIX}"
              PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
            )
  endif( NOM_BUILD_CORE_UNIT AND NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_GUI_UNIT AND NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT AND NOM_BUILD_JSON_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )

  if( NOM_BUILD_CORE_UNIT AND NOM_BUILD_AUDIO_UNIT AND NOM_BUILD_SYSTEM_UNIT )
    add_executable ( audio "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/audio/audio.cpp" )
    install ( DIRECTORY
              "${EXAMPLES_SRC_DIR}/audio/Resources"
              DESTINATION
              "${EXAMPLES_INSTALL_PEFIX}"
              PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
            )
  endif( NOM_BUILD_CORE_UNIT AND NOM_BUILD_AUDIO_UNIT AND NOM_BUILD_SYSTEM_UNIT )

  if( NOM_BUILD_CORE_UNIT AND NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )
    add_executable ( events "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/events.cpp" )
    install ( DIRECTORY
              "${EXAMPLES_SRC_DIR}/events/Resources"
              DESTINATION
              "${EXAMPLES_INSTALL_PEFIX}"
              PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
            )
  endif( NOM_BUILD_CORE_UNIT AND NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )

  if( NOM_BUILD_CORE_UNIT AND NOM_BUILD_SYSTEM_UNIT )
    add_executable ( device_info "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/device_info.cpp" )
  endif( NOM_BUILD_CORE_UNIT AND NOM_BUILD_SYSTEM_UNIT )

  if( NOM_BUILD_CORE_UNIT AND NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )
    add_executable ( fonts "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/fonts.cpp" )
    install ( DIRECTORY
              "${EXAMPLES_SRC_DIR}/fonts/Resources"
              DESTINATION
              "${EXAMPLES_INSTALL_PEFIX}"
              PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
            )
  endif( NOM_BUILD_CORE_UNIT AND NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )

  if( NOM_BUILD_CORE_UNIT AND NOM_BUILD_JSON_UNIT AND NOM_BUILD_PTREE_UNIT AND NOM_BUILD_SYSTEM_UNIT )
    # TODO (re-write with new nom::JSON API)
    #add_executable ( json_reader "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/json/json_reader.cpp" )
    #add_executable ( json_writer "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/json/json_writer.cpp" )
    add_executable( values "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/ptree/json/values.cpp" )

    # Used by: app, json_reader, sprite_sheets
    install ( DIRECTORY
              "${EXAMPLES_SRC_DIR}/json/Resources"
              DESTINATION
              "${EXAMPLES_INSTALL_PEFIX}"
              PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
            )
  endif( NOM_BUILD_CORE_UNIT AND NOM_BUILD_JSON_UNIT AND NOM_BUILD_PTREE_UNIT AND NOM_BUILD_SYSTEM_UNIT )

  if( NOM_BUILD_CORE_UNIT AND NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_SYSTEM_UNIT )
    add_executable ( sprite_sheets "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/sprites/sprite_sheets.cpp" )
  endif( NOM_BUILD_CORE_UNIT AND NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_SYSTEM_UNIT )

  # Install library dependencies into binary output directory
  if ( PLATFORM_WINDOWS )
    install ( DIRECTORY
              "${SDL2_LIBRARY_DIR}"
              "${SDL2_IMAGE_LIBRARY_DIR}"
              "${SDL2_TTF_LIBRARY_DIR}"
              "${OPENAL_LIBRARY_DIR}"
              "${LIBSNDFILE_LIBRARY_DIR}"
              "${MSVCPP_LIBRARY_REDIST}"
              DESTINATION
              "${EXAMPLES_INSTALL_PEFIX}"
              FILES_MATCHING PATTERN "*.dll"
            )
  endif ( PLATFORM_WINDOWS )
endif( NOT NOM_BUILD_MINI_UNIT )
