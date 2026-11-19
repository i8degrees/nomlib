#!/bin/bash
#
# shellcheck shell=bash
#
# Usage:
#
#   ./update-copyright.sh [--apply]
#

# TODO(JEFF): Add env vars for control of the files to be targeted?
# TODO(JEFF): Add env vars for control over the email address to be replaced?

[ -n "$DEBUG" ] && set -o errexit
[ -n "$DEBUG_TRACE" ] && set -o xtrace

for f in $(git status --porcelain=1 | grep -e "M " | cut -d' ' -f3) ; do
# Find all C++ header and source files in the repository
#for f in $(git ls-files 'src/*.cpp' 'include/nomlib/*.hpp' 2>/dev/null || find . -name 'src/*.cpp' -o -name 'include/nomlib/*.hpp'); do
    if [ ! -f "$f" ]; then
        continue
    fi

    counter=1
    tmpfile=$(mktemp)
    echo "$f"

    # Check if the file contains a Copyright notice for Jeffrey Carpenter
    if grep -q "Copyright" "$f" && grep -q "Jeffrey Carpenter" "$f"; then

        # Pass 1: Update copyright years
        if sed -e 's/Copyright (c) 2013, 2014 Jeffrey Carpenter/Copyright (c) 2013, 2014, 2015, 2016, 2024, 2025 and 2026 Jeffrey Carpenter/g' "$f" > "$tmpfile"; then
          counter=$((counter+1))
        fi
        
        # Pass 2: Update email address in a separate pass
        if sed -e 's/<i8degrees@gmail.com>/<jeff@479831.xyz>/g' "$tmpfile" > "${tmpfile}.2" && mv "${tmpfile}.2" "$tmpfile"; then
          counter=$((counter+1))
        fi
    else
        echo "  Copyright not found or not owned by Jeffrey Carpenter"
        rm -f "$tmpfile"
        continue
    fi

    # Check if changes were made and display a preview
    if ! cmp "$f" "$tmpfile" >/dev/null ; then
        head "$tmpfile" -n 6 | sed 's/^/  /'
        if [[ "$1" == "--apply" ]] ; then
            cat "$tmpfile" > "$f"
            echo "[PATCHED] $f"
        fi
    fi

    # TODO(JEFF): This branch has yet to be tested. We must do so before this
    # script is considered ready to commit.
    if [[ "$1" == "--apply" ]] ; then
      echo "INFO: A total of ${counter} files were found and updated."
      echo
    else 
      echo
      #echo "INFO: A total of ${counter} files were found and needs updating."
      #echo
    fi

    rm -f "$tmpfile"
done

