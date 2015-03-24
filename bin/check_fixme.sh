#!/bin/sh

GREP_BIN=$(which grep) # (BSD grep on OS X) 2.5.1-FreeBSD

if [[ !(-x $GREP_BIN) ]]; then
  echo "$0 ERROR: grep command not found"
  exit 1
fi

if [[ ! $1 ]]; then
  $GREP_BIN "FIXME" --color=auto -s -I -i -R --exclude-dir \*.git src
  $GREP_BIN "FIXME" --color=auto -s -I -i -R --exclude-dir \*.git include
else
  $GREP_BIN "FIXME" --color=auto -s -I -i -R --exclude-dir \*.git $@
fi
