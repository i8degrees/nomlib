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
macro(nom_add_library target lib_type source headers external_deps)

  add_library( ${target} ${lib_type} ${source} )

  # The Application Binary Interface (ABI) version; PATCH level versions are
  # intended **not** to break the ABI version.
  set_target_properties(  ${target} PROPERTIES SOVERSION
                          "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
  )

  # The Application Programming Interface (API) version
  set_target_properties(  ${target} PROPERTIES VERSION
                          "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
  )

  set_target_properties( ${target} PROPERTIES DEBUG_POSTFIX "-d" )

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
                            # TODO?
                            # PUBLIC_HEADER
                            # "${source}"
    )
  endif( PLATFORM_OSX AND FRAMEWORK )

  # Copy target's library file to $CMAKE_INSTALL_PREFIX/lib
  install(  TARGETS ${target}
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            LIBRARY FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX} )

endmacro(nom_add_library)

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

macro(NOM_LOG_INFO msg)
  message( STATUS "INFO: ${msg}" )
endmacro(NOM_LOG_INFO msg)

macro(NOM_LOG_WARN msg)
  message( WARNING "WARN: ${msg}" )
endmacro(NOM_LOG_WARN msg)

macro(NOM_LOG_CRIT msg)
  message( FATAL_ERROR "CRITICAL: ${msg}" )
endmacro(NOM_LOG_CRIT msg)
