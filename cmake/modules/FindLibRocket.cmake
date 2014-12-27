# Locate LibRocket libraries
#
# This module defines:
# LIBROCKET_LIBRARIES, the name of the libraries to link against
# LIBROCKET_INCLUDE_DIRS, where to find the development header files
# LIBROCKET_FOUND, if false, do not try to link to LIBROCKET_LIBRARIES
#
# Written by Jeffrey Carpenter <i8degrees@gmail.com>

set(  LIBROCKET_CORE_NAMES
      libRocketCore # release
      libRocketCore-d # debug
      libRocketCore.1.dylib
      RocketCore
)

set(  LIBROCKET_CONTROLS_NAMES
      libRocketControls # release
      libRocketControls-d # debug
      libRocketControls.1.dylib
      RocketControls
)

set(  LIBROCKET_DEBUGGER_NAMES
      libRocketDebugger # release
      libRocketDebugger-d # debug
      libRocketDebugger.1.dylib
      RocketDebugger
)

# TODO
# set(  LIBROCKET_CORE_LUA_NAMES
#       libRocketCoreLua # release
#       libRocketCoreLua-d # debug
#       libRocketCoreLua.1.dylib
#       RocketCoreLua
# )

# TODO
# set(  LIBROCKET_CONTROLS_LUA_NAMES
#       libRocketControlsLua # release
#       libRocketControlsLua-d # debug
#       libRocketControlsLua.1.dylib
#       RocketControlsLua
# )

set(  LIBROCKET_SEARCH_PATHS
      ~/Library/Frameworks
      /Library/Frameworks
      /usr/local/include # homebrew
      /sw # Fink
      /opt/local # DarwinPorts
      /opt/csw # Blastwave
      /opt
)

find_path(  LIBROCKET_CORE_INCLUDE_DIR Rocket/Core # Core/Core.h
            HINTS
            $ENV{LIBROCKETDIR}
            PATH_SUFFIXES
            include # Rocket include/Rocket
            PATHS
            ${LIBROCKET_SEARCH_PATHS}
)

find_path(  LIBROCKET_CONTROLS_INCLUDE_DIR Rocket/Controls # Controls/Controls.h
            HINTS
            $ENV{LIBROCKETDIR}
            PATH_SUFFIXES
            include # Rocket include/Rocket
            PATHS
            ${LIBROCKET_SEARCH_PATHS}
)

find_path(  LIBROCKET_DEBUGGER_INCLUDE_DIR Rocket/Debugger # Debugger/Debugger.h
            HINTS
            $ENV{LIBROCKETDIR}
            PATH_SUFFIXES
            include # Rocket include/Rocket
            PATHS
            ${LIBROCKET_SEARCH_PATHS}
)

# TODO
# find_path(  LIBROCKET_LUA_INCLUDE_DIR Rocket/Lua # Lua/Lua.h
#             HINTS
#             $ENV{LIBROCKETDIR}
#             PATH_SUFFIXES
#             include # Rocket include/Rocket
#             PATHS
#             ${LIBROCKET_SEARCH_PATHS}
# )

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(VC_LIB_PATH_SUFFIX lib/x64)
else()
  set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

find_library( LIBROCKET_CORE_LIBRARY
              NAMES ${LIBROCKET_CORE_NAMES}
              HINTS
              $ENV{LIBROCKETDIR}
              PATH_SUFFIXES
              lib64 lib ${VC_LIB_PATH_SUFFIX}
              PATHS
              ${LIBROCKET_SEARCH_PATHS}
)

find_library( LIBROCKET_CONTROLS_LIBRARY
              NAMES ${LIBROCKET_CONTROLS_NAMES}
              HINTS
              $ENV{LIBROCKETDIR}
              PATH_SUFFIXES
              lib64 lib ${VC_LIB_PATH_SUFFIX}
              PATHS
              ${LIBROCKET_SEARCH_PATHS}
)

find_library( LIBROCKET_DEBUGGER_LIBRARY
              NAMES ${LIBROCKET_DEBUGGER_NAMES}
              HINTS
              $ENV{LIBROCKETDIR}
              PATH_SUFFIXES
              lib64 lib ${VC_LIB_PATH_SUFFIX}
              PATHS
              ${LIBROCKET_SEARCH_PATHS}
)

# TODO
# find_library( LIBROCKET_LUA_LIBRARY
#               NAMES ${LIBROCKET_LUA_NAMES}
#               HINTS
#               $ENV{LIBROCKETDIR}
#               PATH_SUFFIXES
#               lib64 lib ${VC_LIB_PATH_SUFFIX}
#               PATHS
#               ${LIBROCKET_SEARCH_PATHS}
# )

set(  LIBROCKET_INCLUDE_DIRS
      ${LIBROCKET_CORE_INCLUDE_DIR}
      ${LIBROCKET_CONTROLS_INCLUDE_DIR}
      ${LIBROCKET_DEBUGGER_INCLUDE_DIR}
      # CACHE PATH "libRocket development header files"
)

# TODO
# if( LIBROCKET_LUA_INCLUDE_DIR_FOUND )
#   list( APPEND LIBROCKET_INCLUDE_DIRS ${LIBROCKET_LUA_INCLUDE_DIR} )
# endif( LIBROCKET_LUA_INCLUDE_DIR_FOUND )

set(  LIBROCKET_LIBRARIES
      ${LIBROCKET_CORE_LIBRARY}
      ${LIBROCKET_CONTROLS_LIBRARY}
      ${LIBROCKET_DEBUGGER_LIBRARY}
      # CACHE PATH "LibRocket libraries"
)

# TODO
# if( LIBROCKET_LUA_LIBRARY_FOUND )
#   list( APPEND LIBROCKET_LIBRARIES ${LIBROCKET_LUA_LIBRARY} )
# endif( LIBROCKET_LUA_LIBRARY_FOUND )

# handle the QUIETLY and REQUIRED arguments and set LIBROCKET_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )

find_package_handle_standard_args(  LibRocket
                                    REQUIRED_VARS
                                    LIBROCKET_CORE_LIBRARY
                                    LIBROCKET_CONTROLS_LIBRARY
                                    LIBROCKET_DEBUGGER_LIBRARY
                                    LIBROCKET_CORE_INCLUDE_DIR
                                    LIBROCKET_CONTROLS_INCLUDE_DIR
                                    LIBROCKET_DEBUGGER_INCLUDE_DIR
)

mark_as_advanced( LIBROCKET_CORE_LIBRARY
                  LIBROCKET_CONTROLS_LIBRARY
                  LIBROCKET_DEBUGGER_LIBRARY
                  # LIBROCKET_LUA_LIBRARY
                  LIBROCKET_CORE_INCLUDE_DIR
                  LIBROCKET_CONTROLS_INCLUDE_DIR
                  LIBROCKET_DEBUGGER_INCLUDE_DIR
                  # LIBROCKET_LUA_INCLUDE_DIR
)
