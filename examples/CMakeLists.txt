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

# Additional flags to pass the add_executable command
set ( EXECUTABLE_FLAGS "" )

# We wish to compile our examples as console apps on Windows
# TODO: Console app should perhaps be a Debug release default, whereas if
# compiling a Release binary, we might ought to toggle WIN32 flag? Further
# investigation is needed...
if ( PLATFORM_WINDOWS )
  #set ( EXECUTABLE_FLAGS "WIN32" )

  # This workaround is to get the file paths to match up on all platforms -_-
  # build/Debug/examples under MSVC++
  set ( EXAMPLES_INSTALL_PEFIX "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}" )
else ( NOT PLATFORM_WINDOWS )

  if ( CMAKE_GENERATOR STREQUAL "Xcode" )

    # Match build/Debug/examples
    set ( EXAMPLES_INSTALL_PEFIX "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}" )

  else ( NOT CMAKE_GENERATOR STREQUAL "Xcode" )

    # We prefer lower-case file paths
    string ( TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE )

    # Match build/debug/examples
    set ( EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/examples/${CMAKE_BUILD_TYPE}" )
    set ( EXAMPLES_INSTALL_PEFIX "${EXECUTABLE_OUTPUT_PATH}" )
  endif ( CMAKE_GENERATOR STREQUAL "Xcode" )

endif ( PLATFORM_WINDOWS )

add_executable ( audio "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/audio/audio.cpp" )
add_executable ( app "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/app/app.cpp" )
add_executable ( sprite_sheets "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/sprites/sprite_sheets.cpp" )
# TODO (re-write with new nom::JSON API)
#add_executable ( json_reader "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/json/json_reader.cpp" )
#add_executable ( json_writer "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/json/json_writer.cpp" )

# Install example resources
install ( DIRECTORY
          "${EXAMPLES_SRC_DIR}/audio/Resources"
          DESTINATION
          "${EXAMPLES_INSTALL_PEFIX}"
          PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
        )

# Used by: app, json_reader, sprite_sheets
install ( DIRECTORY
          "${EXAMPLES_SRC_DIR}/json/Resources"
          DESTINATION
          "${EXAMPLES_INSTALL_PEFIX}"
          PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
        )

install ( DIRECTORY
          "${EXAMPLES_SRC_DIR}/app/Resources"
          DESTINATION
          "${EXAMPLES_INSTALL_PEFIX}"
          PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
        )

# Install library dependencies into binary output directory
if ( PLATFORM_WINDOWS )
  install ( DIRECTORY
            "${SDL2_LIBRARY_DIR}"
            "${SDL2_IMAGE_LIBRARY_DIR}"
            "${SDL2_TTF_LIBRARY_DIR}"
            "${OPENAL_LIBRARY_DIR}"
            "${LIBSNDFILE_LIBRARY_DIR}"
            DESTINATION
            "${EXAMPLES_INSTALL_PEFIX}"
            FILES_MATCHING PATTERN "*.dll"
          )

endif ( PLATFORM_WINDOWS )
