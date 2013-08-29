# Locate hqx
#
# This module defines:
#
# HQX_LIBRARY
# HQX_FOUND          if false, do not try to link to hqx
# HQX_INCLUDE_DIR    where to find the headers
#
# Created by Jeffrey Carpenter. Feel free to use this module script however as
# you wish.
#
# CMake should search for OS X frameworks first in the following order:
#
# ~/Library/Frameworks/hqx.framework/Headers
# /Library/Frameworks/hqx.framework/Headers
# /System/Library/Frameworks/hqx.framework/Headers
#
# If found, this will prefer the Framework version over others.
#

find_path ( HQX_INCLUDE_DIR
            NAMES
            hqx
            PATH_SUFFIXES
            include #include/hqx
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

find_library  ( HQX_LIBRARY
                NAMES
                hqx # Release version
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


# handle the QUIETLY and REQUIRED arguments and set HQX_FOUND to TRUE if all
# listed variables are TRUE
find_package_handle_standard_args ( hqx DEFAULT_MSG HQX_LIBRARY HQX_INCLUDE_DIR )

mark_as_advanced ( HQX_LIBRARY HQX_INCLUDE_DIR )
