cmake_minimum_required ( VERSION 2.6 )

include_directories ( ${EXT_HEADERS} )

link_libraries ( ${PROJECT_NAME} ${EXTLIBS} )

set ( EXECUTABLE_FLAGS "" )

if ( PLATFORM_WINDOWS )
  set ( EXECUTABLE_FLAGS "WIN32" )
else ( NOT PLATFORM_WINDOWS )
  # Do nothing -- use the default set EXECUTABLE_FLAGS
endif ( PLATFORM_WINDOWS )

add_executable ( audio "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/audio/audio.cpp" )
add_executable ( app "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/app/app.cpp" )
#add_executable ( font "${EXAMPLES_SRC_DIR}/fonts/font.cpp" )
#add_executable ( reader "${EXAMPLES_SRC_DIR}/json/reader.cpp" )
#add_executable ( writer "${EXAMPLES_SRC_DIR}/json/writer.cpp" )

# Install example resources
if ( PLATFORM_WINDOWS )
  # Do nothing stub
else ( NOT PLATFORM_WINDOWS ) # Assume POSIX-like platform
  install ( DIRECTORY
            "${EXAMPLES_SRC_DIR}/audio/Resources"
            DESTINATION
            "${CMAKE_CURRENT_BINARY_DIR}"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

  install ( DIRECTORY
            "${EXAMPLES_SRC_DIR}/fonts/Resources"
            DESTINATION
            "${CMAKE_CURRENT_BINARY_DIR}"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

  install ( DIRECTORY
            "${EXAMPLES_SRC_DIR}/json/Resources"
            DESTINATION
            "${CMAKE_CURRENT_BINARY_DIR}"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )

  install ( DIRECTORY
            "${EXAMPLES_SRC_DIR}/app/Resources"
            DESTINATION
            "${CMAKE_CURRENT_BINARY_DIR}"
            PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
          )
endif ( PLATFORM_WINDOWS )
