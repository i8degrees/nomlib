# Returns a version string from Git
#
# These functions force a re-configure on each git commit so that you can
# trust the values of the variables in your build system.
#
#  git_rev_number(<var> [<additional arguments to git rev-parse> ...])
#
# Returns the results of `git rev-parse HEAD` on the source tree, and adjusting
# the output so that it tests false if an error occurs.
#
# Requires CMake 2.6 or newer (uses the 'function' command)
#
# This script derives from GetGitRevisionDescription.cmake, found at
# [cmake-modules](https://github.com/rpavlik/cmake-modules).
#
# Author:
# Copyright (c) 2014, Jeffrey Carpenter <i8degrees@gmail.com>
# Distributed under the Simplified BSD License
# (See accompanying project file LICENSE.md or copy at
# http://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_.28.22Simplified_BSD_License.22_or_.22FreeBSD_License.22.29)
#
# Original Author:
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryand.net>
# http://academic.cleardefinition.com
# Iowa State University HCI Graduate Program/VRAC
#
# Copyright Iowa State University 2009-2010.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

if(__get_git_revision_number)
  return()
endif()
set(__get_git_revision_number YES)

function(git_rev_number _var)
  if(NOT GIT_FOUND)
    find_package(Git QUIET)
  endif()
  if(NOT GIT_FOUND)
    set(${_var} "GIT-NOTFOUND" PARENT_SCOPE)
    return()
  endif()

  # TODO sanitize
  #if((${ARGN}" MATCHES "&&") OR
  # (ARGN MATCHES "||") OR
  # (ARGN MATCHES "\\;"))
  # message("Please report the following error to the project!")
  # message(FATAL_ERROR "Looks like someone's doing something nefarious with git_describe! Passed arguments ${ARGN}")
  #endif()

  #message(STATUS "Arguments to execute_process: ${ARGN}")

  execute_process(COMMAND
    "${GIT_EXECUTABLE}"
    rev-parse
    ${ARGN} # additional args passed by end-user
    HEAD
    WORKING_DIRECTORY
    "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE
    res
    OUTPUT_VARIABLE
    out
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  if(NOT res EQUAL 0)
    # set(out "${out}-${res}-NOTFOUND")

    # Use only the set err message 'GIT-NOTFOUND' for the output variable,
    # instead of both the resulting output in addition to 'GIT-NOTFOUND'.
    set(out "${res}-NOTFOUND")
  endif()

  set(${_var} "${out}" PARENT_SCOPE)
endfunction()
