#.rst:
# FindSDL2_ttf
# -----------
#
# Locate SDL2_ttf library
#
# This module defines:
#
# ::
#
#   SDL2_TTF_LIBRARIES, the name of the library to link against
#   SDL2_TTF_INCLUDE_DIRS, where to find the headers
#   SDL2_TTF_FOUND, if false, do not try to link against
#
# $SDL2TTFDIR is an environment variable that would correspond to the
# ./configure --prefix=$SDL2TTFDIR used in building SDL2_ttf.
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

set(SDL2_TTF_SEARCH_PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
)

find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
        HINTS
        $ENV{SDL2TTFDIR}
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_TTF_SEARCH_PATHS}
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(VC_LIB_PATH_SUFFIX lib/x64)
else()
  set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

find_library(SDL2_TTF_LIBRARY
        NAMES SDL2_ttf
        HINTS
        $ENV{SDL2TTFDIR}
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib ${VC_LIB_PATH_SUFFIX}
        PATHS ${SDL2_TTF_SEARCH_PATHS}
)

set(SDL2_TTF_LIBRARIES ${SDL2_TTF_LIBRARY})
set(SDL2_TTF_INCLUDE_DIRS ${SDL2_TTF_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(  SDL2_ttf REQUIRED_VARS
                                    SDL2_TTF_LIBRARY
                                    SDL2_TTF_INCLUDE_DIR
)
