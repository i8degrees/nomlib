#!/bin/sh

# NOTE: This script is intended to be non-interactively ran by the project's
# build scripts.

JSHINT_BIN=$(which jshint)

if [[ -x $JSHINT_BIN ]]; then
  # We only bother validating critical game files that would prevent us from
  # initialization.
  jshint ../Resources/cards.json
  jshint ../Resources/config_game.json
  jshint ../Resources/config_assets-low-res.json
  jshint ../Resources/config_assets-hi-res.json
else
  echo "${0} WARN: Skipping JSON validation; jshint not found!"
  echo "See installation instructions at http://jshint.com/install/"
  # exit 1
fi
