# cmake/uninstall.cmake:jeff
#
# Helper script to create `uninstall` make target to undo the results
# of a `install` target run.
#
# This target is not available until the project has been configured by CMake.
#

set(UNINSTALL_FILE_TEMPLATE "${CMAKE_TEMPLATE_PATH}/uninstall.cmake.in")
set(UNINSTALL_FILE_DEST "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake")

# uninstall template file -> destination path (build dir)
configure_file( "${UNINSTALL_FILE_TEMPLATE}"
                "${UNINSTALL_FILE_DEST}"
                IMMEDIATE @ONLY)
add_custom_target( uninstall # new target name, i.e.: `make uninstall`
  COMMAND ${CMAKE_COMMAND} -P "${UNINSTALL_FILE_DEST}")

