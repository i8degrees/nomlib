# Locate json_spirit
#
# This module defines:
#
# JSON_SPIRIT_LIBRARY
# JSON_SPIRIT_FOUND, if false, do not try to link to json_spirit
# JSON_SPIRIT_INCLUDE_DIR, where to find the headers
#
# CMake should search for OSX frameworks first in the following order:
#
# ~/Library/Frameworks/json_spirit.framework/Headers
# /Library/Frameworks/json_spirit.framework/Headers
# /System/Library/Frameworks/json_spirit.framework/Headers
#
# If found, this will prefer the Framework version over others.
#
# Written by Jeffrey Carpenter <jeffrey.carp@gmail.com>.

find_path ( JSON_SPIRIT_INCLUDE_DIR
            NAMES
            json_spirit.h
            PATH_SUFFIXES
            include/json_spirit
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

find_library  ( JSON_SPIRIT_LIBRARY
                NAMES
                json_spirit
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

find_package_handle_standard_args ( json_spirit DEFAULT_MSG JSON_SPIRIT_LIBRARY JSON_SPIRIT_INCLUDE_DIR )

mark_as_advanced ( JSON_SPIRIT_LIBRARY JSON_SPIRIT_INCLUDE_DIR )
