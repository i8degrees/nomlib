#!/bin/sh

# Full path to make executable
MAKE_BIN=$(which make)

# Unconditionally make all targets while staying quiet about it
MAKE_PARAMS="-B --quiet"

$MAKE_BIN $MAKE_PARAMS test # target

if [[ $? -eq 0 ]]; then
    ./system
    ./math
    ./graphics
fi
