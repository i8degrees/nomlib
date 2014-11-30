# nomlib-file library

# Library headers inclusion path
include_directories( "${INC_ROOT_DIR}" )

set( NOM_FILE_LIBRARY "nomlib-file" )

set(  NOM_FILE_SOURCE
      # TODO: Relocate to file sub-directory
      # ${INC_DIR}/file.hpp

      ${SRC_DIR}/system/Path.cpp
      ${INC_DIR}/system/Path.hpp

      ${SRC_DIR}/system/File.cpp
      ${INC_DIR}/system/File.hpp

      ${INC_DIR}/system/IFile.hpp
)

# Platform-specific implementations & dependencies

if( NOM_PLATFORM_POSIX ) # BSD, OS X && Linux
  set(  NOM_FILE_SOURCE ${NOM_FILE_SOURCE}
        # TODO: Relocate to file sub-directory
        ${SRC_DIR}/system/unix/UnixFile.cpp
        ${INC_DIR}/system/unix/UnixFile.hpp
      )

endif( NOM_PLATFORM_POSIX )

if( PLATFORM_WINDOWS )
  set(  NOM_FILE_SOURCE
        ${NOM_FILE_SOURCE}
        # TODO: Relocate to file sub-directory
        ${SRC_DIR}/system/windows/WinFile.cpp
        ${INC_DIR}/system/windows/WinFile.hpp
      )
endif( PLATFORM_WINDOWS )

# Common on all platforms
set( NOM_FILE_DEPS nomlib-core )

if( PLATFORM_OSX )
  find_library( CORE_SERVICES_LIBRARY CoreServices REQUIRED )
  mark_as_advanced( CORE_SERVICES_LIBRARY )

  find_library( CORE_FOUNDATION_LIBRARY CoreFoundation REQUIRED )
  mark_as_advanced( CORE_FOUNDATION_LIBRARY )

  list( APPEND NOM_FILE_DEPS ${CORE_SERVICES_LIBRARY} ${CORE_FOUNDATION_LIBRARY} )
endif( PLATFORM_OSX )

# Add and link the library
nom_add_library(  ${NOM_FILE_LIBRARY} ${LIBRARY_OUTPUT_TYPE}
                  "${NOM_FILE_SOURCE}" ""
                  "${NOM_FILE_DEPS}" )

# Re-declare globally, so other build units that depend on us can refer to this
# variable
# set( NOM_FILE_LIBRARY ${NOM_FILE_LIBRARY} PARENT_SCOPE )
