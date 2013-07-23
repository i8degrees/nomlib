cmake_minimum_required ( VERSION 2.8 )

set ( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR} )

set ( NOMLIB_EXAMPLES_AUDIO
      ${EXAMPLES_SRC_DIR}/audio/audio.cpp
    )

#     Required Dependencies
link_libraries ( nomlib SDLmain )

add_executable ( audio/audio ${NOMLIB_EXAMPLES_AUDIO} )
