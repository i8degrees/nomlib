# Helper macros for CMake build scripts
#

# Be quiet about what we are doing if verbose makefiles are toggled off
if ( NOT CMAKE_VERBOSE_MAKEFILE )
  set ( COMMENT_TEXT )
endif ( NOT CMAKE_VERBOSE_MAKEFILE )

# Add runtime search path to our application bundle so that we can find its
# dependencies at launch.
macro ( add_rpath rpath binary_path )

  foreach ( path ${rpath} )

    if ( CMAKE_VERBOSE_MAKEFILE )
      set ( COMMENT_TEXT "\nAdding runtime search path: \n\n\t${path}\n\nto ${binary_path}\n\n" )
    endif ( CMAKE_VERBOSE_MAKEFILE )

    add_custom_command  ( TARGET ${PROJECT_NAME}
                          COMMAND ${CMAKE_INSTALL_NAME_TOOL}
                          -add_rpath "${path}" "${binary_path}"
                          COMMENT ${COMMENT_TEXT}
                        )

  endforeach ( path ${rpath} )

endmacro ( add_rpath rpath binary_path )

# Modify runtime search path for a library or application
macro ( change_rpath old_rpath new_rpath binary_path )

  if ( CMAKE_VERBOSE_MAKEFILE )
    set ( COMMENT_TEXT "\nModifying runtime search path for ${binary_path}: \n\n\t${old_rpath}\n\nto ${new_rpath}\n\n" )
  endif ( CMAKE_VERBOSE_MAKEFILE )

  add_custom_command  ( TARGET ${PROJECT_NAME}
                        COMMAND ${CMAKE_INSTALL_NAME_TOOL}
                        -change "${old_rpath}" "${new_rpath}" "${binary_path}"
                        COMMENT ${COMMENT_TEXT}
                      )

endmacro ( change_rpath rpath binary_path )

# Change the install name path of a library
macro ( install_name_rpath rpath binary_path )

  if ( CMAKE_VERBOSE_MAKEFILE )
    set ( COMMENT_TEXT "\nModifying install name path for ${binary_path}: \n\n\t${rpath}\n\n" )
  endif ( CMAKE_VERBOSE_MAKEFILE )

  add_custom_command  ( TARGET ${PROJECT_NAME}
                        COMMAND ${CMAKE_INSTALL_NAME_TOOL}
                        -id "${rpath}" "${binary_path}"
                        COMMENT ${COMMENT_TEXT}
                      )

endmacro ( install_name_rpath rpath binary_path )
