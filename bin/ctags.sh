#!/bin/sh
#
#   Generate project tag file (symbol reference mappings)
#
# NOTE: This script should be ran from the project's root directory, i.e.:
#       ~/Projects/nomlib.git/
#

SOURCE_DIR="src"
HEADER_DIR="include/nomlib/"

CTAGS_BIN="$(which ctags)"
CTAGS_FILENAME=".tags"

${CTAGS_BIN} -f ${CTAGS_FILENAME} -R ${SOURCE_DIR} ${HEADER_DIR}
