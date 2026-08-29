# cmake/functions.cmake:jeff
#
# Helper functions for CMake build scripts
#
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

# \brief Perform a copy of the target framework to its final destination
# (app bundle). This function is a NO-OP when the platform is not APPLE
# **and** CMAKE_FRAMEWORK has not been defined.
#
# \extra The public headers in each framework bundle are not (yet) present at
# the time of executing this function, and thus can only be used for run-time
# dependencies (not development).
#
# \param target - The executable target to copy to
# \param lib - One or more libraries to copy
function(copy_framework_dep target)
  #if(NOT APPLE AND NOT DEFINED CMAKE_FRAMEWORK)
  if(NOT APPLE)
    message(WARN
      "Apple and CMAKE_FRAMEWORK should be defined"
    )
    return()
  endif()

  foreach(library ${ARGN})
    add_custom_command(
      TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/../Frameworks"
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_BINARY_DIR}/lib/${library}.framework"
      "$<TARGET_FILE_DIR:${target}>/../Frameworks/${library}.framework"
      # ensure that arguments with spaces or special chars handle expansion
      # safely alongside generator expressions
      VERBATIM
    )
    # FIXME(JEFF): The generator expression version is ideal as the current
    # command above will fail if a multi-config generator project were to be
    # used.
    #add_custom_command(
      #TARGET ${target} POST_BUILD
      #COMMAND $<$:$<STREQUAL:$<PLATFORM_ID>,Darwin>:${CMAKE_COMMAND}> -E make_directory "$<TARGET_FILE_DIR:${target}>/../Frameworks"
      #COMMAND $<$:$<STREQUAL:$<PLATFORM_ID>,Darwin>:${CMAKE_COMMAND}> -E copy_directory
      #"${CMAKE_BINARY_DIR}/lib/${library}.framework"
        #"$<TARGET_FILE_DIR:${target}>/../Frameworks/${library}.framework"
        #VERBATIM
      #)
  endforeach()
endfunction()

# Set target's @rpath
#
# \param target - library or executable
# \param path - The run-time path to set on the target upon install (redist).
function(set_rpath target path)
  set_target_properties("${target}"
    PROPERTIES
      INSTALL_RPATH "${path}"
      INSTALL_RPATH_USE_LINK_PATH TRUE
  )
endfunction()

