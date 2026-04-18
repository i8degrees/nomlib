#!/bin/bash

# mkdir -p build
# cd build || exit 255
cmake -DDEBUG=on -DDEBUG_ASSERT=on -DEXAMPLES=on -DNOM_BUILD_TESTS=on \
	-DCMAKE_OSX_DEPLOYMENT_TARGET=10.15 \
	-DCMAKE_INSTALL_PREFIX=~/Library/Frameworks ..
make -j12
