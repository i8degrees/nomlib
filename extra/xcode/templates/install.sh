#!/bin/sh
#
# Install nomlib templates for Xcode 4 into $HOME/Library/Developer/Xcode/Templates/

mkdir -p $HOME/Library/Developer/Xcode/Templates/
cp -R "$(dirname $0)/nomlib" $HOME//Library/Developer/Xcode/Templates/
