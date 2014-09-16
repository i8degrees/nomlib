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

# Install files wrapper with support for list variables; install DIRECORIES
# syntax doesn't seem to support multiple directories separated by a semicolon.
#
# TODO: I'd like to eventually see this used all throughout the build process;
# needs more testing and consideration... (areas of use include example and unit
# test binaries, i.e.: dependencies to run the executables without cluttering up
# system-wide paths, or requiring administrative rights).
#
# TODO: Support CONFIGURATIONS and COMPONENT variables of the install command?
#
# http://www.cmake.org/cmake/help/v3.0/command/install.html?highlight=install
macro( install_dependencies dirs dest file_type )

  foreach( dir ${dirs} )

    install ( DIRECTORY
              "${dir}"
              DESTINATION
              "${dest}"
              FILES_MATCHING PATTERN ${file_type}
            )

  endforeach( dir ${dirs} )

endmacro( install_dependencies dirs file_type )
