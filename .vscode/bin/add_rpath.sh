#!/bin/sh

# This script is intended to be ran from within the CMake environment.
#
# See cmake/macros.cmake, man 1 install_name_tool

# install_name_tool
${1} -add_rpath ${2} ${3}

# We **must** exit with a successful return code for Xcode-generated project
# builds, or else the whole build fails on us
exit 0
