:: SOURCE: http://573.github.io/blog/2010/10/13/adopt-a-visual-c-plus-plus-express-edition-project-with-cmake/
::
::@echo off

:: Setup the development environment for CMake -- we need a compiler and linker.
@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin;%PATH%

@mkdir build
@cd build
:: Force a rebuild of the CMake cache
del /Q CMakeCache.txt
del /Q CMakeFiles

:: Commence build configuration of project ...
cmake -D DEBUG=on -D EXAMPLES=on ..
:: if all goes well, we ought to have generated a nomlib.vcxproj file!

:: Return to where we came from
@cd ..
pause
