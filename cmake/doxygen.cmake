# Include support for generation of documentation via doxygen
#
# This script depends on the following CMake variables being set before
# inclusion of said script:
#
#   CMAKE_TEMPLATE_PATH
#   PROJECT_BINARY_DIR
#

# Control whether or not to see warnings regarding incomplete documentation
# from doxygen.
# Valid values are "YES" or "NO" with a default value of "NO".
set ( BUILD_DOCS_DEV "YES" )

# Control whether or not to see messages generated by doxygen
# Valid values are "YES" or "NO" with a default value of "NO".
set ( BUILD_DOCS_QUIET "NO" )

find_package ( Doxygen )

if ( NOT DOXYGEN_FOUND )
  message ( FATAL_ERROR "Doxygen & graphviz is required to generate the documentation.")
endif ( NOT DOXYGEN_FOUND )

set ( DOXYFILE_IN ${CMAKE_TEMPLATE_PATH}/Doxyfile.in )
set ( DOXYFILE ${PROJECT_BINARY_DIR}/Doxyfile )
set ( DOXY_HTML_INDEX_FILE ${PROJECT_BINARY_DIR}/docs/html/index.html )

#set ( DOXY_EXTRA_FILES "" )

# Template Doxyfile
configure_file  ( ${DOXYFILE_IN} ${DOXYFILE} @ONLY )

# Generate docs only when dependencies change
add_custom_command  ( OUTPUT ${DOXY_HTML_INDEX_FILE}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
                      MAIN_DEPENDENCY ${DOXYFILE} ${DOXYFILE_IN}
                      #DEPENDS project_targets ${DOXY_EXTRA_FILES}
                      COMMENT "Generating HTML documentation"
                    )

# Add 'make docs' target
add_custom_target ( docs ALL DEPENDS ${DOXY_HTML_INDEX_FILE} )
