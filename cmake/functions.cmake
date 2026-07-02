# cmake/functions.cmake:jeff
#
# Helper functions for CMake build scripts
#

# Copy resource files for engine examples and tests.
# nom_install_resources(spath, dpath, options)
function(install_resource_file spath dpath)
  install(
    FILES "${spath}"
    DESTINATION "${dpath}")
endfunction()

function(install_resource_dir spath dpath)
  install(
    DIRECTORY "${spath}"
    DESTINATION "${dpath}"
    PATTERN ".*" EXCLUDE )
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

function(create_symlink target spath dest)
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${spath}
    ${dest}
  )
endfunction()

#function(install_dependency spath dest comp)
  #install(FILES ${spath} DESTINATION ${dest} COMPONENT comp)
#endfunction()
