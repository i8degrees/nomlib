@echo off

REM NOTE: This script is intended to be ran from the project's current build
REM directory.

REM IF NOT EXIST build mkdir build
REM pushd build

REM Defaults for generating project files for a Debug build
set BUILD_TYPE=Debug
set BUILD_FLAGS=-DDEBUG=on -DDEBUG_ASSERT=on

if "%1" == "Release" set BUILD_TYPE=Release
if "%BUILD_TYPE%" == "Release" set BUILD_FLAGS=

echo "Generating %BUILD_TYPE% project files..."
echo "BUILD_FLAGS: %BUILD_FLAGS%"

REM Only 32-bit builds have been tested; we're probably missing library
REM dependencies for a complete 64-bit build ...
set CMAKE_FLAGS=-DARCH_32=on -DEXAMPLES=on -DNOM_BUILD_TESTS=on

if EXIST CMakeCache.txt del CMakeCache.txt

cmake -G "Visual Studio 12" %CMAKE_FLAGS% %BUILD_FLAGS% ..
REM popd
