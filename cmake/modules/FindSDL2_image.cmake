#.rst:
# FindSDL2_image
# -------------
#
# Locate SDL2_image library
#
# This module defines:
#
# ::
#
#   SDL2_IMAGE_LIBRARIES, the name of the library to link against
#   SDL2_IMAGE_INCLUDE_DIRS, where to find the headers
#   SDL2_IMAGE_FOUND, if false, do not try to link against
#   SDL2_IMAGE_VERSION_STRING - human-readable string containing the version of SDL_image
#
# $SDL2IMAGEDIR is an environment variable that would correspond to the
# ./configure --prefix=$SDL2IMAGEDIR used in building SDL2_image.
#
# Created by Eric Wing.  This was influenced by the FindSDL.cmake
# module, but with modifications to recognize OS X frameworks and
# additional Unix paths (FreeBSD, etc).

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
# Copyright 2012 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

set(SDL2_IMAGE_SEARCH_PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
)

find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h
        HINTS
        $ENV{SDL2IMAGEDIR}
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_IMAGE_SEARCH_PATHS}
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(VC_LIB_PATH_SUFFIX lib/x64)
else()
  set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

find_library(SDL2_IMAGE_LIBRARY
        NAMES SDL2_image
        HINTS
        $ENV{SDL2IMAGEDIR}
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib ${VC_LIB_PATH_SUFFIX}
        PATHS ${SDL2_IMAGE_SEARCH_PATHS}
)

set(SDL2_IMAGE_LIBRARIES ${SDL2_IMAGE_LIBRARY})
set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(  SDL2_image REQUIRED_VARS
                                    SDL2_IMAGE_LIBRARY
                                    SDL2_IMAGE_INCLUDE_DIR
)
