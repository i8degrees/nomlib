# Helper macros for CMake build scripts
#

# Add runtime search path to our application bundle so that we can find its
# dependencies at launch.
macro ( install_rpath rpath binary_path )

  foreach ( path ${rpath} )
    add_custom_command  ( TARGET ${PROJECT_NAME}
                          COMMAND ${CMAKE_INSTALL_NAME_TOOL}
                          -add_rpath "${path}" "${binary_path}"
                          COMMENT "\nAdding runtime search path: \n\n\t${path}\n\nto ${binary_path}\n\n"
                        )
  endforeach ( path ${rpath} )

endmacro ( install_rpath rpath binary_path )
