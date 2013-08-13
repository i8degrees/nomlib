# Helper macros for CMake build scripts
#

# Add runtime search path to our application bundle so that we can find its
# dependencies at launch.
macro ( add_rpath rpath binary_path )

  foreach ( path ${rpath} )
    add_custom_command  ( TARGET ${PROJECT_NAME}
                          COMMAND ${CMAKE_INSTALL_NAME_TOOL}
                          -add_rpath "${path}" "${binary_path}"
                          COMMENT "\nAdding runtime search path: \n\n\t${path}\n\nto ${binary_path}\n\n"
                        )
  endforeach ( path ${rpath} )

endmacro ( add_rpath rpath binary_path )

# Modify runtime search path for a library or application
macro ( change_rpath old_rpath new_rpath binary_path )

  add_custom_command  ( TARGET ${PROJECT_NAME}
                        COMMAND ${CMAKE_INSTALL_NAME_TOOL}
                        -change "${old_rpath}" "${new_rpath}" "${binary_path}"
                        COMMENT "\nModifying runtime search path for ${binary_path}: \n\n\t${old_rpath}\n\nto ${new_rpath}\n\n"
                      )

endmacro ( change_rpath rpath binary_path )

# Change the install name path of a library
macro ( install_name_rpath rpath binary_path )

  add_custom_command  ( TARGET ${PROJECT_NAME}
                        COMMAND ${CMAKE_INSTALL_NAME_TOOL}
                        -id "${rpath}" "${binary_path}"
                        COMMENT "\nModifying install name path for ${binary_path}: \n\n\t${rpath}\n\n"
                      )

endmacro ( install_name_rpath rpath binary_path )
