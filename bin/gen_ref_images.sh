#!/bin/sh
TESTS_BIN_DIR=build/tests/Debug

${TESTS_BIN_DIR}/GradientTest -r
${TESTS_BIN_DIR}/BitmapFontTest -r
${TESTS_BIN_DIR}/TrueTypeFontTest -r
${TESTS_BIN_DIR}/BMFontTest -r
${TESTS_BIN_DIR}/libRocketTest -r
${TESTS_BIN_DIR}/libRocketDataGridTest -r
