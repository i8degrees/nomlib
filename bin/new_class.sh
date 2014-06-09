#!/bin/sh
#
#   Start a new library class -- copies template files to destination group path
#
# NOTE: This script should be ran from the project's root directory, i.e.:
#       ~/Projects/nomlib.git/
#
#   Usage example:
#
# $ cd ~/Projects/nomlib.git
#
# $ # Valid syntax:
# $ bin/new_class.sh gui NewFancyDangledWidgetContainer
#
# $ # This is also valid:
# $ bin/new_class.sh graphics/fonts NewFancyDangledFontType
#
# $ # Not valid syntax:
# $ bin/new_class.sh system2 NewFancyDangledBuildUnit
#

# PROJECT_SOURCE_DIR/include/nomlib
HEADER_INSTALL_PREFIX="include/nomlib/"

# PROJECT_SOURCE_DIR/src/
SOURCE_INSTALL_PREFIX="src/"

# PROJECT_SOURCE_DIR/Resources/SharedSupport/Template.hpp
HEADER_TEMPLATE_PATH="Resources/SharedSupport/Template.hpp"

# PROJECT_SOURCE_DIR/Resources/SharedSupport/Template.cpp
SOURCE_TEMPLATE_PATH="Resources/SharedSupport/Template.cpp"

# Helper script / executable to use
CP_BIN=$(which cp)  # /bin/cp (BSD cp)
CP_ARGS="-av"

# Destination filing directory.
DEST_GROUP=$1

# Destination file name to use when copying our template class files over.
DEST_FILE=$2

# Exit if err
function check_script_paths()
{
  if [[ -d "${HEADER_INSTALL_PREFIX}" && "${SOURCE_INSTALL_PREFIX}" && "${HEADER_TEMPLATE_PATH}" && "${SOURCE_TEMPLATE_PATH}" ]]; then
    continue # Success!
  else
    echo "ERROR: One or more script variable file paths do not exist!"
    exit
  fi
}

# Exit if err
function check_dest_paths()
{
  # Destination file path is a directory, exists and is readable
  if [[ -d "${HEADER_INSTALL_PREFIX}/${DEST_GROUP}" && -r "${HEADER_INSTALL_PREFIX}/${DEST_GROUP}" ]]; then
    continue # Success!
  else
    echo "ERROR: ${HEADER_INSTALL_PREFIX}/${DEST_GROUP} does not exist, or is not readable!"
    exit
  fi

  if [[ -f "${HEADER_INSTALL_PREFIX}/${DEST_GROUP}/${DEST_FILE}.hpp" ]]; then
    echo "ERROR: ${HEADER_INSTALL_PREFIX}/${DEST_GROUP}/${DEST_FILE} already exists!"
    exit
  fi

  # Destination file path is a directory, exists and is readable
  if [[ -d "${SOURCE_INSTALL_PREFIX}/${DEST_GROUP}" && -r "${SOURCE_INSTALL_PREFIX}/${DEST_GROUP}" ]]; then
    continue # Success!
  else
    echo "ERROR: ${SOURCE_INSTALL_PREFIX}/${DEST_GROUP} does not exist, or is not readable!"
    exit
  fi

  if [[ -f "${SOURCE_INSTALL_PREFIX}/${DEST_GROUP}/${DEST_FILE}.cpp" ]]; then
    echo "ERROR: ${SOURCE_INSTALL_PREFIX}/${DEST_GROUP}/${DEST_FILE} already exists!"
    exit
  fi
}

# Print usage info and quit
function usage_info()
{
  echo "Usage: ./$0 <group> <new_class_name>"
  echo "\n"
  echo "i.e.: ./$0 gui DataViewColumn"
  exit
}

if [[ ${DEST_GROUP} != "" && ${DEST_FILE} != "" ]]; then

    # Sanity checks; exit if err
    check_dest_paths
    check_script_paths

    # Template.cpp -> <new_class_name>.hpp
    ${CP_BIN} ${CP_ARGS} ${HEADER_TEMPLATE_PATH} ${HEADER_INSTALL_PREFIX}/${DEST_GROUP}/${DEST_FILE}.hpp

    # Template.cpp -> <new_class_name>.cpp
    ${CP_BIN} ${CP_ARGS} ${SOURCE_TEMPLATE_PATH} ${SOURCE_INSTALL_PREFIX}/${DEST_GROUP}/${DEST_FILE}.cpp
else
  usage_info
fi
