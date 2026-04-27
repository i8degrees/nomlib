# cmake/functions.cmake:jeff
#
# Helper functions for CMake build scripts
#

# Create and link a library module
#
# source parameter should be enclosed within double quotes.
# headers parameter is not implemented; reserved for future implementation.
#
# external_deps parameters should be separated by semicolons when multiple
# dependencies are specified and enclosed within double quotes.
#
# TODO: Future expansion of this macro should strongly consider refactoring with
# the use of the CMakeParseArguments module.
# http://www.cmake.org/cmake/help/v3.0/module/CMakeParseArguments.html
function(nom_add_library target lib_type source headers external_deps )
  # The library version defines the full build version of said library and
  # is used in the actual filename on disk.
  set ( LIB_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )

  # This is embedded as the SONAME in the library's ELF header and determinnes
  # which file the dynamic linker searches for at runtime, i.e.:
  # `libnomlib-actions.so.1 -> `libnomlib-actions.so.0.11`
  # The Application Binary Interface (ABI) version; PATCH level versions are
  # intended **not** to break the ABI version.
  set ( LIB_SOVERSION ${PROJECT_VERSION_MAJOR} )

  add_library( ${target} ${lib_type} ${source} )

  set_target_properties( ${target} PROPERTIES VERSION ${LIB_VERSION}
                         SOVERSION ${LIB_SOVERSION} DEBUG_POSTFIX "-d" )
  target_link_libraries( ${target} ${external_deps} )

  if( PLATFORM_OSX AND FRAMEWORK )

    # Create target.framework
    set_target_properties(  ${target} PROPERTIES
                            FRAMEWORK TRUE
                            MACOSX_FRAMEWORK_INFO_PLIST
                            "${CMAKE_TEMPLATE_PATH}/Info.plist.in"
                            MACOSX_FRAMEWORK_NAME
                            "${target}"
                            MACOSX_FRAMEWORK_BUNDLE_VERSION
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
                            MACOSX_FRAMEWORK_SHORT_VERSION_STRING
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                            MACOSX_FRAMEWORK_IDENTIFIER
                            "net.i8degrees.${target}"
                            # PUBLIC_HEADER
                            # "${source}"
    )
  endif( PLATFORM_OSX AND FRAMEWORK )

  # Copy target's library file to $CMAKE_INSTALL_PREFIX/lib
  install(  TARGETS ${target}
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            LIBRARY FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX} )

endfunction(nom_add_library)

# Copy resource files for engine examples and tests.
# nom_install_resources(spath, dpath, options)
function(install_resource_file spath dpath)
  install(
    FILES "${spath}"
    DESTINATION "${dpath}")
    #PATTERN ".*" EXCLUDE )
endfunction()

function(install_resource_dir spath dpath)
  install(
    DIRECTORY "${spath}"
    DESTINATION "${dpath}")
    #PATTERN ".*" EXCLUDE )
endfunction()

#
# target parameter is not implemented; reserved for future implementation.
#
# dest parameter is not implemented; reserved for future implementation.
# macro(nom_install_dep target external_deps dest)

#   # Bundle the appropriate external dependencies
#   foreach( dep ${external_deps} )

#     if( IS_DIRECTORY ${dep} )

#       # Bundle frameworks we depend on that are not system library bundles
#       install(  DIRECTORY ${dep}
#                 DESTINATION "nomlib.framework/Frameworks"
#                 PATTERN ".*" EXCLUDE )

#     else( NOT IS_DIRECTORY ${dep} )

#       # if( IS_SYMLINK ${dep} )
#       #   # Resolve real file path when symbolic so CMake's install command
#       #   # copies the real file
#       #   get_filename_component( dep ${dep} REALPATH )
#       # endif( IS_SYMLINK ${dep} )
#       # message( STATUS "DEP IS A FILE: ${dep}" )

#       # Bundle dynamic libraries (*.dylib) that we depend on
#       install(  FILES ${dep}
#                 DESTINATION "nomlib.framework/Frameworks"
#                 PATTERN ".*" EXCLUDE )

#       endif( IS_DIRECTORY ${dep} )
#     endforeach( dep ${external_deps} )

# endmacro(nom_install_dep target external_deps dest)
