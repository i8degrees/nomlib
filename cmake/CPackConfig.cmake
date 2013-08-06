# This file controls the generation details of our packages through CPack
# variables set below.
#
# These variables should be set in the CMake list file of the project *BEFORE*
# the CPack module is included.

set ( CPACK_PACKAGE_NAME "${PROJECT_NAME}" )
set ( CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )
set ( CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )

set ( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )

# PackageMaker refuses to let us use the Markdown extension (MD) -_-
#
# TODO / Generate these text files on the fly from the Markdown sources
set ( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_TEMPLATE_PATH}/LICENSE.html" )
#set ( CPACK_RESOURCE_FILE_README "${CMAKE_TEMPLATE_PATH}/README.txt" )
#set ( CPACK_RESOURCE_FILE_WELCOME "${CMAKE_TEMPLATE_PATH}/Welcome.txt" )
#set ( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_TEMPLATE_PATH}/Description.txt" )

set ( CPACK_PACKAGE_VENDOR "org.i8degrees.${PROJECT_NAME}" )
set ( CPACK_PACKAGE_CONTACT "jeffrey.carp@gmail.com" )
set ( CPACK_PACKAGE_DESCRIPTION "C++11 cross-platform game engine" )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "nomlib - C++11 cross-platform game engine" )

# TODO / verify these package dependency names
set ( CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl1.2 libsdl-image1.2 libsdl-ttf2.0 libsndfile1 libopenal libmagic libgtk2.0" )
set ( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
set ( CPACK_DEBIAN_PACKAGE_SECTION "devel" )
set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )

set ( CPACK_STRIP_FILES false )

#set ( CPACK_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR} )
#set ( CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
