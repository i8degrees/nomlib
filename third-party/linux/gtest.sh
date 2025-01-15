#!/bin/bash
#
#

if [ -n "$INIT_BUILD" ]; then
  cd /home/jeff/tmp/nomlib/
  git clone "https://github.com/google/googletest.git"
  cd googletest
  git checkout v1.8.x
fi
