#!/bin/sh
#
#   Generate library documentation; prepare the generated documentation for
# locally viewing documentation via pow (http://pow.cx).
#
# NOTE: This script should be ran from the project's current build directory,
# i.e.: ~/Projects/nomlib.git/build
#
#   Usage example:
#
# $ cd ~/Projects/nomlib.git/build
# $ ../bin/gen_docs.sh
#

DOCS_INSTALL_PREFIX="docs"

# Helper scripts / binaries to use
LN_BIN=$(which ln)  # /usr/bin/ln (BSD LN)
LN_ARGS="-sF"

# Exit if err
function check_docs_paths()
{
  if [[ -d "${DOCS_INSTALL_PREFIX}" ]]; then
    continue # Success!
  else
    echo "ERROR: ${DOCS_INSTALL_PREFIX} file path does not exist!"
    echo "\n"
    echo "Have you generated documentation using 'make docs' yet?"
    exit
  fi

  if [[ -d "${DOCS_INSTALL_PREFIX}/html" ]]; then
    continue; # Success!
  else
    echo "ERROR: ${DOCS_INSTALL_PREFIX}/html file path does not exist!"
    echo "\n"
    echo "Have you generated documentation using 'make docs' yet?"
    exit
  fi
}

# Print usage info and quit
function usage_info()
{
  echo "Usage: ./$0"
  echo "\n"
  exit
}

if [[ ${1} == "-h" || ${1} == "--help" ]]; then
  usage_info
else
  # Sanity checks; exit if err
  check_docs_paths

  DOCS_INSTALL_PREFIX=$(pwd)/${DOCS_INSTALL_PREFIX}

  # Create a symbolic link from 'docs/html' to 'docs/public' for automatic
  # pow configuration. See also: http://pow.cx/
  ${LN_BIN} ${LN_ARGS} ${DOCS_INSTALL_PREFIX}/html ${DOCS_INSTALL_PREFIX}/public

  echo "Symbolic link created from ${DOCS_INSTALL_PREFIX} to ${DOCS_INSTALL_PREFIX}/public"
fi
