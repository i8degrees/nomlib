# This file controls the generation details of our packages through CPack
# variables set below.
#
# These variables should be set in the CMake list file of the project *BEFORE*
# the CPack module is included.
#
# SEE ALSO
# 1. https://docs.appimage.org/packaging-guide/index.html
# 2. https://cmake.org/cmake/help/latest/cpack_gen/appimage.html

set ( CPACK_PACKAGE_NAME "${PROJECT_NAME}" )
set ( CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )
set ( CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )

set ( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )

set(CPACK_APPIMAGE_DESKTOP_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop")
#set(CPACK_APPIMAGE_DESKTOP_FILE "${CMAKE_TEMPLATE_PATH}/nomlib.desktop.in")
#set(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/Resources/nomlib.png")

# PackageMaker refuses to let us use the Markdown extension (MD) -_-
#
# TODO / Generate these text files on the fly from the Markdown sources
set ( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_TEMPLATE_PATH}/LICENSE.html" )
#set ( CPACK_RESOURCE_FILE_README "${CMAKE_TEMPLATE_PATH}/README.txt" )
#set ( CPACK_RESOURCE_FILE_WELCOME "${CMAKE_TEMPLATE_PATH}/Welcome.txt" )
#set ( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_TEMPLATE_PATH}/Description.txt" )

set ( CPACK_PACKAGE_VENDOR "com.i8degrees.${PROJECT_NAME}" )
set ( CPACK_PACKAGE_CONTACT "1329364+i8degrees@users.noreply.github.com")
set ( CPACK_PACKAGE_DESCRIPTION "C++11 cross-platform game engine" )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "nomlib - C++11 cross-platform game engine" )

# TODO / verify these package dependency names
set ( CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0.0 libsdl2-image-2.0.0 libsdl2-ttf-2.0.0 libsndfile1 libopenal1")
set ( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
set ( CPACK_DEBIAN_PACKAGE_SECTION "games" )
set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${PLATFORM_ARCH}" )

if(CMAKE_BUILD_TYPE MATCHES Rel)
  set ( CPACK_STRIP_FILES true )
endif()

#set ( CPACK_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR} )
#set ( CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )

include(CPack)

configure_file  ( "${CMAKE_TEMPLATE_PATH}/nomlib.desktop.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop"
                  @ONLY
                )
