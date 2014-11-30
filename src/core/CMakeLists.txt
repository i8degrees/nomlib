# nomlib-core library

# Library headers inclusion path
include_directories("${INC_ROOT_DIR}")

# Library name
set( NOM_CORE_LIBRARY "nomlib-core" )

set(  NOM_CORE_SOURCE
      ${INC_DIR}/core.hpp
      ${INC_DIR}/macros.hpp
      ${INC_DIR}/platforms.hpp
      ${INC_DIR}/types.hpp

      ${INC_DIR}/config.hpp # auto-generated
      ${CMAKE_TEMPLATE_PATH}/config.hpp.in

      ${SRC_DIR}/version.cpp
      ${INC_DIR}/version.hpp # auto-generated
      ${CMAKE_TEMPLATE_PATH}/version.hpp.in

      ${SRC_DIR}/revision.cpp # auto-generated
      ${CMAKE_TEMPLATE_PATH}/revision.cpp.in
      ${INC_DIR}/revision.hpp

      ${SRC_DIR}/core/helpers.cpp
      ${INC_DIR}/core/helpers.hpp

      ${SRC_DIR}/core/clock.cpp
      ${INC_DIR}/core/clock.hpp

      ${SRC_DIR}/core/ObjectTypeInfo.cpp
      ${INC_DIR}/core/ObjectTypeInfo.hpp

      ${SRC_DIR}/core/IObject.cpp
      ${INC_DIR}/core/IObject.hpp

      ${SRC_DIR}/core/ConsoleOutput.cpp
      ${INC_DIR}/core/ConsoleOutput.hpp

      ${SRC_DIR}/core/SDL2Logger.cpp
      ${INC_DIR}/core/SDL2Logger.hpp

      ${SRC_DIR}/core/SDL_assertion_helpers.cpp
      ${INC_DIR}/core/SDL_assertion_helpers.hpp
)

# Platform-specific implementations & dependencies

# Common on all platforms
set( NOM_CORE_DEPS ${SDL2_LIBRARY} )

if( NOM_PLATFORM_POSIX ) # BSD, OS X && Linux
  set(  NOM_CORE_SOURCE ${NOM_CORE_SOURCE}
        ${SRC_DIR}/core/UnixConsoleOutput.cpp )
endif( NOM_PLATFORM_POSIX )

if( PLATFORM_WINDOWS )
  set(  NOM_CORE_SOURCE ${NOM_CORE_SOURCE}
        ${SRC_DIR}/core/WindowsConsoleOutput.cpp )
endif( PLATFORM_WINDOWS )

# Add and link the library
nom_add_library(  ${NOM_CORE_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                  "${NOM_CORE_SOURCE}" ""
                  "${NOM_CORE_DEPS}" )

# Re-declare globally, so other build units that depend on us can refer to this
# variable
# set( NOM_CORE_LIBRARY "${NOM_CORE_LIBRARY}" PARENT_SCOPE )

# Auto-generated header file containing engine version
configure_file( "${CMAKE_TEMPLATE_PATH}/version.hpp.in"
                "${INC_DIR}/version.hpp" )

# Auto-generated config for engine
configure_file( "${CMAKE_TEMPLATE_PATH}/config.hpp.in"
                "${INC_DIR}/config.hpp" )
