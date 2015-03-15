@echo off

REM NOTE: This script is intended to be ran from the project's current build
REM directory.

REM Default flags for msbuild
set MSBUILD_FLAGS=/fl1 /flp1:LogFile=nomlib_err.log;errorsonly /fl2 /flp2:LogFile=nomlib_warn.log;warningsonly /Verbosity:minimal

if [%1]==[] (
  REM Default build type
  set BUILD_TYPE=Debug
) else (
  set BUILD_TYPE=%1
)

echo "Build configuration: %BUILD_TYPE%..."

msbuild /t:build /p:Configuration=%BUILD_TYPE% %MSBUILD_FLAGS% ALL_BUILD.vcxproj
