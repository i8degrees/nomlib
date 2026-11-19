#!/bin/bash
#
# Generate reference images for later comparison for the tests that support it.
#

TESTS_BIN_DIR=$1
if [ -z "$TESTS_BIN_DIR" ]; then
  #TESTS_BIN_DIR="$(pwd)/build"
  echo "CRITICAL: A path to the test executables must be given to this script."
  echo
  exit 22
fi

if [ ! -d "$TESTS_BIN_DIR" ]; then
  echo "CRITICAL: The given path at ${TESTS_BIN_DIR} must be a directory."
  echo
  exit 1
fi

# TODO(JEFF): Add a check in our Visual Unit Testing framework for this env
# variable and allow the end-user to override the hard-coded path.
REFERENCE_IMAGE_DIR="$HOME/Documents/nomlib/tests"

VISUAL_TESTS=(
  "${TESTS_BIN_DIR}/ActionTest"
  "${TESTS_BIN_DIR}/BitmapFontTest"
  "${TESTS_BIN_DIR}/BMFontTest"
  "${TESTS_BIN_DIR}/GradientTest"
  "${TESTS_BIN_DIR}/libRocketTest"
  "${TESTS_BIN_DIR}/libRocketDataGridTest"
  "${TESTS_BIN_DIR}/SpriteTest"
  "${TESTS_BIN_DIR}/TrueTypeFontTest"
  "${TESTS_BIN_DIR}/ImageDiffTest"
  "${TESTS_BIN_DIR}/VisualUnitTestFrameworkTest"
)

cmd=()
for test in ${VISUAL_TESTS[@]}; do
  if [ -x "${test}" ]; then
    cmd+=("${test} -r")
    echo "DRY_RUN: " "${cmd}"
  else
    echo "Skipping ${test}..."
  fi
done

