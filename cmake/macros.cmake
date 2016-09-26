# cmake/macros.cmake:jeff
#
# Helper macros for CMake build scripts
#

# Add runtime search path to our application bundle so that we can find its
# dependencies at launch.
macro ( add_rpath target rpath binary_path )

  foreach ( path ${rpath} )

    if ( CMAKE_VERBOSE_MAKEFILE )
      set ( COMMENT_TEXT "\nAdding runtime search path: \n\n\t${path}\n\nto ${binary_path}\n\n" )
    endif ( CMAKE_VERBOSE_MAKEFILE )

    add_custom_command  ( TARGET ${target}
                          COMMAND ${CMAKE_INSTALL_NAME_TOOL}
                          -add_rpath "${path}" "${binary_path}"
                          COMMENT ${COMMENT_TEXT}
                        )

  endforeach ( path ${rpath} )

endmacro ( add_rpath target rpath binary_path )

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

# Helper function for adding tests through CTest
#
# IMPORTANT: We cannot use the GTEST_ADD_TESTS macro here for adding tests that
# rely on the nom::VisualUnitTest framework because of the way that the macro
# breaks up the test run -- it ends up executing each individual test in a
# separate process, i.e.: 'SpriteTest.SpriteInterfaceWithTextureReference' and
# 'SpriteTest.SpriteInterfaceWithTextureRawPointer' are treated as two
# separated executable binaries.
#  This is bad for us because our screen-dumping creates new timestamped
# directories on every new instance of the framework, which normally is OK
# because this yields one directory, but in the case of multiple executable
# runs ... spawns an awful lot more than I'd prefer.
#   I hope to one day figure out a proper solution for this work flow issue,
# but in the mean time ... this is the best I can come up with.
macro( nom_add_visual_test test_name executable )
  add_test( ${test_name} ${executable}
            --gtest_filter=${test_name}.* ${ARGN} )
endmacro()

# Helper function for adding an engine unit test
#
# IMPORTANT: Avoid using the newer add_test syntax, i.e.:
# add_test(NAME <name> COMMAND <command>), because these tests are not
# added to the default test configuration! Using the newer add_test
# syntax leads me to this err message when running ctest from the project's
# build directory (CMake generated XCode project files):
#     "Test not available without configuration. (Missing "-C <config>"?)"
macro( nom_add_test test_name test_executable )
  add_test( ${test_name} ${test_executable} ${ARGN} )
endmacro()

# Copy resource files for engine examples and tests.
macro(nom_install_resources source_path destination_path)
  install(FILES ${source_path} DESTINATION "${destination_path}")
endmacro()

macro(NOM_LOG_INFO msg)
  message( STATUS "INFO: ${msg}" )
endmacro(NOM_LOG_INFO msg)

macro(NOM_LOG_WARN msg)
  message( WARNING "WARN: ${msg}" )
endmacro(NOM_LOG_WARN msg)

macro(NOM_LOG_CRIT msg)
  message( FATAL_ERROR "CRITICAL: ${msg}" )
endmacro(NOM_LOG_CRIT msg)
