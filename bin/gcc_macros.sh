#!/bin/sh
#
# Snippet for dumping all the compiler macros for GNU GCC that are available
# to us at build time.
#

echo | gcc -dM -E -

