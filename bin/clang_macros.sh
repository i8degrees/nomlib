#!/bin/sh
#
# Snippet for dumping all the compiler macros (clang) available 
# to us at build time.
#

echo | clang -dM -E -

