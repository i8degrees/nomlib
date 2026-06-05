# cmake/policy.cmake:jeff
#
# Policy file
#

# Enable the use of MACOSX_RPATH by default; this effectively allows plug 'n'
# play functionality, so to speak -- the resulting shared library files can
# simply be copied over into the end-user's application bundle or framework
# bundle. No mucking around with install_name_tool.
#
# (Minimum required CMake: v2.8.12)
#
#   See also:
#
# cmake --help-policy cmp0042
# http://www.kitware.com/blog/home/post/510
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif(POLICY CMP0042)

# CMake Environment

# `find_package()` uses upper-case `PACKAGENAME_ROOT` variables.
# cmake --help-policy CMP0144
# Minimum required CMake v3.27
if(POLICY CMP0144)
  cmake_policy(SET CMP0144 NEW)
endif(POLICY CMP0144)

# PythonInterp modules; acknowledgement
# cmake --help-policy CMP0148
if(POLICY CMP0148)
  cmake_policy(SET CMP0148 NEW)
endif(POLICY CMP0148)

# Opt out of using CMake v3.0 PROJECT_VERSION variables management for the
# project.
# http://www.cmake.org/cmake/help/v3.0/command/project.html#command:project
#if( POLICY CMP0048 )
#cmake_policy( SET CMP0048 OLD )
#endif( POLICY CMP0048 )

if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif(POLICY CMP0072)
