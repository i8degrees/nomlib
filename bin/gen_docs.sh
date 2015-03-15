#!/bin/sh

# NOTE: This script is intended to be ran from the project's current build
# directory.

# See also: xcode_docs.sh

LN_BIN=/bin/ln # BSD ln
LN_ARGS="-sF"

# Relative directory path to the generated documentation files -- from the
# build tree
DOCS_INSTALL_PREFIX="docs"
# Default host name to use
DOCS_HOST_NAME="nomlib-docs"

function usage_info()
{
  echo "Usage: $(basename $0) <host_name>\n"
  echo "...where <host_name> is an optional parameter to allow customization "
  echo "of the DNS name used to refer to the generated documentation, i.e.:\n"
  echo "    http://<host_name>.dev"
  echo "(Defaults: http://${DOCS_HOST_NAME}.dev)"
  exit 0
}

function check_env_sanity()
{
  if [[ !(-d "${DOCS_INSTALL_PREFIX}") ]]; then
    echo "ERROR: The file path ${DOCS_INSTALL_PREFIX} does not exist!\n"
    echo "...Have you not yet generated the documentation set?"
    exit -1
  fi

  if [[ !(-d "${DOCS_INSTALL_PREFIX}/html") ]]; then
    echo "ERROR: The file path ${DOCS_INSTALL_PREFIX}/html does not exist!\n"
    echo "...Have you not yet generated the documentation set?"
    exit -1
  fi
}

if [[ ${1} == "-h" || ${1} == "--help" ]]; then
  usage_info
else
  check_env_sanity

  if [[ !( -z "$1") ]]; then
    # End-user override of the default host name
    DOCS_HOST_NAME=$1
  fi

  DOCS_INSTALL_PREFIX=$(pwd)/${DOCS_INSTALL_PREFIX}

  if [[ ! (-L ${DOCS_INSTALL_PREFIX}/public) ]]; then
    # Link 'docs/html' -> 'docs/public'
    ${LN_BIN} ${LN_ARGS} ${DOCS_INSTALL_PREFIX}/html ${DOCS_INSTALL_PREFIX}/public
    echo "A symbolic link from ${DOCS_INSTALL_PREFIX} to ${DOCS_INSTALL_PREFIX}/public has been created."
  fi

  if [[ ! (-L ${HOME}/.pow/${DOCS_HOST_NAME}) ]]; then
    # Link 'docs/public' -> '~/.pow/<host_name>'
    ${LN_BIN} ${LN_ARGS} ${DOCS_INSTALL_PREFIX} ${HOME}/.pow/${DOCS_HOST_NAME}
    echo "A symbolic link from ${DOCS_INSTALL_PREFIX} to ${HOME}/.pow/${DOCS_HOST_NAME} has been created."
  fi

  echo "The documentation set should be accessible now at http://${DOCS_HOST_NAME}.dev"
fi
