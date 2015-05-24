@echo off

REM Automated Windows build script for nomlib -- windev.local

REM NOTE: This script is intended to be ran from the project's root
REM directory, i.e.: %HOME%/Projects/nomlib.git

REM virgo.local source repository
unison nomlib

IF NOT EXIST build mkdir build
REM pushd build
cd build

if errorlevel 0 call ..\bin\msbuild_gen.bat Debug
if errorlevel 0 call ..\bin\msbuild_build.bat Debug
if errorlevel 0 call ..\bin\msbuild_install.bat Debug

REM popd
