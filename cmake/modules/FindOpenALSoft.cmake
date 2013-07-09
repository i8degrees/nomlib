# 2013-07/09:jeff
#
# SOURCE: /usr/local/share/cmake/Modules/FindOpenAL.cmake
#
# Locate OpenAL-Soft
# This module defines
# OPENALSOFT_LIBRARY
# OPENALSOFT_FOUND, if false, do not try to link to OpenAL-Soft
# OPENALSOFT_INCLUDE_DIR, where to find the headers
#
# $OPENALSOFTDIR is an environment variable that would
# correspond to the ./configure --prefix=$OPENALSOFTDIR
# used in building OpenAL-Soft.
#
# Created by Eric Wing. This was influenced by the FindSDL.cmake module.

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
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

# This makes the presumption that you are include al.h like
# #include "al.h"
# and not
# #include <AL/al.h>
# The reason for this is that the latter is not entirely portable.
# Windows/OpenAL-Soft does not by default put their headers in AL/ and
# OS X uses the convention <OpenAL/al.h>.
#
# CMake should search for frameworks first in the following order:
#
# ~/Library/Frameworks/OpenAL-Soft.framework/Headers
# /Library/Frameworks/OpenAL-Soft.framework/Headers
# /System/Library/Frameworks/OpenAL-soft.framework/Headers
#
# On OS X, this will prefer the Framework version (if found) over others.
# People will have to manually change the cache values of
# OPENALSOFT_LIBRARY to override this selection or set the CMake environment
# CMAKE_INCLUDE_PATH to modify the search paths.

find_path(OPENALSOFT_INCLUDE_DIR al.h
  HINTS
    ENV OPENALSOFTDIR
  PATH_SUFFIXES include/AL include/OpenAL include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  #[HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
)

find_library(OPENALSOFT_LIBRARY
  NAMES OpenAL-Soft al openal OpenAL32
  HINTS
    ENV OPENALDIR
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw
  /opt/local
  /opt/csw
  /opt
  #[HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
)


# handle the QUIETLY and REQUIRED arguments and set OPENAL_FOUND to TRUE if
# all listed variables are TRUE
#include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenALSoft  DEFAULT_MSG  OPENALSOFT_LIBRARY OPENALSOFT_INCLUDE_DIR)

mark_as_advanced(OPENALSOFT_LIBRARY OPENALSOFT_INCLUDE_DIR)
