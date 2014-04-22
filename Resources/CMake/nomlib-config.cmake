# Locate nomlib
#
# This module defines:
#
# NOMLIB_LIBRARY                  Absolute file path to dynamic or static
#                                 libraries.
#
# NOMLIB_FOUND                    Standard CMake env variable
#
# NOMLIB_INCLUDE_DIR              Absolute directory path to public library
#                                 header files.
#
# NOMLIB_THIRD_PARTY_INCLUDE_DIR  Absolute directory path to third-party library
#                                 dependency header files.
#
# Created by Jeffrey Carpenter. This was influenced by the FindOpenAL.cmake module.
#
# CMake should search for OSX frameworks first in the following order:
#
# ~/Library/Frameworks/nomlib.framework/Headers
# /Library/Frameworks/nomlib.framework/Headers
# /System/Library/Frameworks/nomlib.framework/Headers
#
# If found, this will prefer the Framework version over others.
#

find_path ( NOMLIB_INCLUDE_DIR
            NAMES
            nomlib
            PATH_SUFFIXES
            include/nomlib
            PATHS
            ~/Library/Frameworks
            /Library/Frameworks
            /usr/local/include # homebrew
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
            #[HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
          )

find_library  ( NOMLIB_LIBRARY
                NAMES
                nomlib # Release version
                nomlib-d # Debug version
                PATH_SUFFIXES
                lib64 lib libs64 libs libs/Win32 libs/Win64
                PATHS
                ~/Library/Frameworks
                /Library/Frameworks
                /usr/local/lib # homebrew
                /sw # Fink
                /opt/local # DarwinPorts
                /opt/csw # Blastwave
                /opt
                #[HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
              )

find_path (
            NOMLIB_THIRD_PARTY_INCLUDE_DIR
            NAMES
            third-party/common
            PATH_SUFFIXES
            include/third-party/common
            PATHS
            ~/Library/Frameworks
            /Library/Frameworks
            /usr/local/include # homebrew
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
            #[HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
          )

# handle the QUIETLY and REQUIRED arguments and set NOMLIB_FOUND to TRUE if
# all listed variables are TRUE
include ( FindPackageHandleStandardArgs )

FIND_PACKAGE_HANDLE_STANDARD_ARGS (
                                    nomlib
                                    DEFAULT_MSG
                                    NOMLIB_LIBRARY
                                    NOMLIB_INCLUDE_DIR
                                    NOMLIB_THIRD_PARTY_INCLUDE_DIR
                                  )

mark_as_advanced  (
                    NOMLIB_LIBRARY
                    NOMLIB_INCLUDE_DIR
                  )
