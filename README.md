# nomlib #

C++11 cross-platform game engine using the SDL2 framework.

## Projects Using nomlib ##

* [i8degrees/ttcards](https://github.com/i8degrees/ttcards)

## Building ##

First, ensure that you have the following core dependencies installed:

* [cmake v2.6+](http://www.cmake.org/)
* [git](http://git-scm.com/download/)
* [doxygen v1.8.x](http://www.stack.nl/~dimitri/doxygen/) and [graphviz](http://www.graphviz.org/) only if you want to generate developer documentation.

Next, you should visit the dependencies section for your platform. After these mundane chores are complete, you should be ready to start the building process for your platform!

### OS X ###

[![Build Status](https://travis-ci.org/i8degrees/nomlib.png?branch=dev,master)](https://travis-ci.org/i8degrees/nomlib)

After you have the dependencies taken care of, execute the following commands at your terminal prompt:

```
git clone https://github.com/i8degrees/nomlib
cd nomlib
mkdir build && cd build
cmake ..
make
make install
```

Upon a successful build, you should have the library built as a Framework bundle, under your current build directory, named **nomlib.framework**.

### Linux ###

Linux builds are broken at the moment.

~~Until I get around to writing the proper instructions, you may take a look at my
.travis.yml build script in the project root of nomlib for hints!~~

#### Linux Build Status #####

[![Build Status](https://travis-ci.org/i8degrees/nomlib.png?branch=master,dev)](https://travis-ci.org/i8degrees/nomlib)

### Windows ###

After you have the dependencies taken care of, execute the following commands at the DOS prompt:

```
git clone https://github.com/i8degrees/nomlib
cd nomlib
nomdev.rb gen --dev
nomdev.rb build
```

Upon a successful generation, you should have populated your current build directory with several Visual Studio project files: **nomlib.vcxproj** is the one you will want to work with.

You may also be interested in the functionality of **nomdev.rb build**. (Uses the MSVCPP build tools chain with MSBuild to compile the project via command line).

### Project Options ###

Build options are passed to cmake with the -D option. For example, to change the installation prefix:

```
cmake -D CMAKE_INSTALL_PREFIX=~/Library/Frameworks ..
```

  - Installation path: -D CMAKE_INSTALL_PREFIX=**\<DIRECTORY_PREFIX\>**
    * Defaults to your current **build** directory

  - Documentation: -D DOCS=**\<BOOLEAN\>**
    * Defaults to **OFF**
    * When built (**ON**), the resulting documentation will reside in a new directory named **docs**

  - Examples: -D EXAMPLES=**\<BOOLEAN\>**
    * Defaults to **OFF**
    * When built (**ON**), the resulting binaries will reside in a new directory named **examples**

  - Debugging: -D DEBUG=**\<BOOLEAN\>** -D DEBUG_ASSERT=**\<BOOLEAN\>**
    * Defaults to **OFF**

  - Universal binary: -D UNIVERSAL=**\<BOOLEAN\>**
    * Defaults to **OFF**

Removal is provided by executing **make uninstall** within your current build directory.

## OS X Dependencies ##

* See **third-party/README.md** for where to obtain pre-packaged libraries and how to install them.
* clang (v3.x) with libc++ libraries -- this *may* restrict compiling to OS X v10.7+

## Linux Dependencies ##

* SDL v2.0.1
* SDL_image v2.0.0
* SDL_ttf v2.0.12
* libsndfile v1.0.25
* OpenAL
* Recent version of GNU's GCC compiler with support for C++11 (known to work with v4.6.x)

## Windows Dependencies ##

* See **third-party/README.md** for where to obtain pre-packaged libraries and how to install them.
* [Ruby 1.9.3-p448](http://rubyinstaller.org/downloads/)

* [Microsoft Visual Studio Express 2013 for Windows](http://www.microsoft.com/visualstudio/eng#downloads)

  - Required software tools in your system PATH
    * MSVSCPP -- **\<MSVSCPP_INSTALL_PATH\>\bin**
    * CMake -- **\<CMAKE_INSTALL_PATH\>\bin**
    * Ruby -- **\<RUBY_INSTALL_PATH\>\bin**
    * Git -- **\<GIT_INSTALL_PATH\>\bin**

If this is your first time installing the tools, you might be happy to know that the official installers can do this task for you automatically.
