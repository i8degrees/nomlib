cmake_minimum_required ( VERSION 2.6 )

# FIXME
if ( PLATFORM_WINDOWS )

  set ( EXT_HEADERS "" )
  set ( EXTLIBS "" )

  # SDL2 dependency
  set ( SDL_INCLUDE_DIR "../../third-party/SDL2/include/" )
  set ( SDL_LIBRARY "../../third-party/SDL2/lib/SDL2" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL_LIBRARY} )

  # SDL2_image dependency
  set ( SDL2_IMAGE_INCLUDE_DIR "../../third-party/SDL2_image/include/" )
  set ( SDL2_IMAGE_LIBRARY "../../third-party/SDL2_image/lib/SDL2_image" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_IMAGE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_IMAGE_LIBRARY}  )

  # SDL2_ttf dependency
  set ( SDL2_TTF_INCLUDE_DIR "../../third-party/SDL2_ttf/include/" )
  set ( SDL2_TTF_LIBRARY "../../third-party/SDL2_ttf/lib/SDL2_ttf" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_TTF_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_TTF_LIBRARY} )

  # OpenAL dependency -- must be 32-bit library
  set ( OPENAL_INCLUDE_DIR "../../third-party/OpenAL/include/" )
  set ( OPENAL_LIBRARY "../../third-party/OpenAL/lib/OpenAL32" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${OPENAL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${OPENAL_LIBRARY} )

  # libsndfile dependency -- must be 32-bit library
  set ( LIBSNDFILE_INCLUDE_DIR "../../third-party/libsndfile/include/" )
  set ( LIBSNDFILE_LIBRARY "../../third-party/libsndfile/lib/libsndfile-1" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBSNDFILE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBSNDFILE_LIBRARY} )

endif ( PLATFORM_WINDOWS )

# FIXME
link_libraries ( "../../nomlib-d" ${EXTLIBS} "../../third-party/SDL2/lib/SDL2main" )

set ( EXECUTABLE_FLAGS "" )

if ( PLATFORM_WINDOWS )
  #set ( EXECUTABLE_FLAGS "CONSOLE" )
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
