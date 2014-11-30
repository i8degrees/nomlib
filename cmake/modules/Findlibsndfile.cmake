# Locate libsndfile library
#
# This module defines:
# LIBSNDFILE_LIBRARY, the name of the library to link against
# LIBSNDFILE_INCLUDE_DIR, where to find development header files
# LIBSNDFILE_FOUND, if false, do not try to link to LIBSNDFILE_LIBRARY
#
# Written by Jeffrey Carpenter <i8degrees@gmail.com>

set( LIBSNDFILE_NAMES libsndfile libsndfile-1 sndfile )

set(  LIBSNDFILE_SEARCH_PATHS
      ~/Library/Frameworks
      /Library/Frameworks
      /usr/local/include # homebrew
      /sw # Fink
      /opt/local # DarwinPorts
      /opt/csw # Blastwave
      /opt
)

find_path(  LIBSNDFILE_INCLUDE_DIR sndfile.h
            HINTS
            $ENV{LIBSNDFILEDIR}
            PATH_SUFFIXES
            include include/libsndfile
            PATHS
            ${LIBSNDFILE_SEARCH_PATHS}
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(VC_LIB_PATH_SUFFIX lib/x64)
else()
  set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

find_library( LIBSNDFILE_LIBRARY
              NAMES ${LIBSNDFILE_NAMES}
              HINTS
              $ENV{LIBSNDFILEDIR}
              PATH_SUFFIXES
              lib64 lib ${VC_LIB_PATH_SUFFIX}
              PATHS
              ${LIBSNDFILE_SEARCH_PATHS}
)

# handle the QUIETLY and REQUIRED arguments and set LIBSNDFILE_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )

find_package_handle_standard_args(  libsndfile
                                    REQUIRED_VARS
                                    LIBSNDFILE_LIBRARY
                                    LIBSNDFILE_INCLUDE_DIR
)

mark_as_advanced( LIBSNDFILE_LIBRARY LIBSNDFILE_INCLUDE_DIR )
