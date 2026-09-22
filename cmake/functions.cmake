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
    #message(FATAL_ERROR
      #"Apple and CMAKE_FRAMEWORK must be defined")
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

# Setup a MacOS Framework bundle for the specified (library) target.
#
# \param target_name - the target library to set properties on.
# \param template - the template file is input to use for creating the
# Info.plist.
#
# \note This function is a NO-OP on non-APPLE platforms.
function(apple_setup_framework target_name template)
  set_target_properties(${target_name}
    PROPERTIES
      FRAMEWORK TRUE
      MACOSX_FRAMEWORK_INFO_PLIST
        ${template}
      MACOSX_FRAMEWORK_NAME
        "${target_name}"
      MACOSX_FRAMEWORK_BUNDLE_VERSION
        "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
      MACOSX_FRAMEWORK_SHORT_VERSION_STRING
        "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
      MACOSX_FRAMEWORK_IDENTIFIER
        "net.i8degrees.${target_name}"
      XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
        ""
      MACOSX_RPATH TRUE
  )
endfunction()

# MacOSX install framework headers for the specified (library) target
#
# \param target - the target library to copy the public header files to
# `target.framework/Headers`.
#
# \note This function is a NO-OP unless the platform branch is APPLE
# (cmake built-in variable)
macro(apple_install_framework_headers target_name)
  if(APPLE)
    install(TARGETS "${target_name}"
      EXPORT libtarget
      # ~/Library/Frameworks/target.framework
      FRAMEWORK DESTINATION "${CMAKE_INSTALL_PREFIX}"
        COMPONENT Runtime
        FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_PREFIX}/${target_name}.framework/Headers"
        COMPONENT Development
    )
  endif(APPLE)
  #endfunction()
endmacro()
