cmake_minimum_required ( VERSION 2.6 )

link_libraries ( ${PROJECT_NAME} )

set ( EXECUTABLE_FLAGS "" )

if ( PLATFORM_WINDOWS )
  #set ( EXECUTABLE_FLAGS "CONSOLE" )
else ( NOT PLATFORM_WINDOWS )
  # Do nothing -- use the default set EXECUTABLE_FLAGS
endif ( PLATFORM_WINDOWS )

# Add header files (*.hpp) and source files (*.cpp) to IDE project list
# (For those that use Xcode, MSVC++, etc.)
# FIXME
file ( GLOB EXAMPLE_HEADER_FILES "${EXAMPLES_SRC_DIR}/*.hpp" )
file ( GLOB EXAMPLE_SOURCE_FILES "${EXAMPLES_SRC_DIR}/*.cpp" )
source_group  ( "include" FILES ${EXAMPLE_HEADER_FILES} )
source_group  ( "src" FILES ${EXAMPLE_SOURCE_FILES} )

add_executable ( audio "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/audio/audio.cpp" )
add_executable ( app "${EXECUTABLE_FLAGS}" "${EXAMPLES_SRC_DIR}/app/app.cpp" )
#add_executable ( font "${EXAMPLES_SRC_DIR}/fonts/font.cpp" )
#add_executable ( reader "${EXAMPLES_SRC_DIR}/json/reader.cpp" )
#add_executable ( writer "${EXAMPLES_SRC_DIR}/json/writer.cpp" )

# Install example resources
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

# Install library dependencies into binary output directory
# FIXME
if ( PLATFORM_WINDOWS )
  install ( FILES
            "${SDL2_LIBRARY_DLL}"
            "${SDL2_IMAGE_LIBRARY_DLL}"
            "${SDL2_TTF_LIBRARY_DLL}"
            "${LIBSNDFILE_LIBRARY_DLL}"
            "${OPENAL_LIBRARY_DLL}"
            DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
          )
endif ( PLATFORM_WINDOWS )
