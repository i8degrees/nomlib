cmake_minimum_required ( VERSION 2.6 )

link_libraries ( ${PROJECT_NAME} ${SDL_LIBRARY} )

add_executable ( audio "${EXAMPLES_SRC_DIR}/audio/audio.cpp" )
add_executable ( app "${EXAMPLES_SRC_DIR}/app/app.cpp" )
add_executable ( font "${EXAMPLES_SRC_DIR}/fonts/font.cpp" )
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

# install ( DIRECTORY
#           "${EXAMPLES_SRC_DIR}/json/Resources"
#           DESTINATION
#           "${CMAKE_CURRENT_BINARY_DIR}"
#           PATTERN ".*" EXCLUDE # do not include those pesky dotfiles!
#         )
