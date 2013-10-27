# nomlib #

C++11 cross-platform game engine using SDL2 framework

## Projects Using nomlib ##

* [i8degrees/ttcards](https://github.com/i8degrees/ttcards)

## Building Instructions ##

### Dependencies ###

* [cmake v2.6+](http://www.cmake.org/)

* [SDL v2.0.0](http://libsdl.org/)
* [SDL_image v2.0.0](http://www.libsdl.org/projects/SDL_image/)
* [SDL_ttf v2.0.12](http://www.libsdl.org/projects/SDL_ttf/)

  - OpenAL
    * Windows: [OpenALSoft](http://kcat.strangesoft.net/openal.html)
    > I'd suggest trying the 32-bit bins first; I've had trouble with the 64-bit bins.
    * Linux: libopenal-dev
    * OS X: Creative Labs OpenAL SDK or [OpenALSoft](http://kcat.strangesoft.net/openal.html)

* [libsndfile](http://www.mega-nerd.com/libsndfile/)
> I'd suggest trying the 32-bit bins if you are on Windows!

* [doxygen v1.8.x](http://www.stack.nl/~dimitri/doxygen/) and [graphviz](http://www.graphviz.org/) are needed if you plan on generating the documentation.

* clang (v3.x) with libc++ libraries or recent GCC version (known to work with v4.6.x) with c++0x

### OS X v10.7+ ###

Home sweet home! Feel free to try installing one dependency via [homebrew](http://brew.sh/) and another via Framework bundle, it *may* work for you as it did for me. Once you have procured the necessary library dependencies, you may begin the fun:

```
git clone https://github.com/i8degrees/nomlib
cd nomlib
mkdir build && cd build
```

If you do not specify a install prefix using **-D CMAKE_INSTALL_PREFIX**, the libraries will be installed under the local cmake build directory.

```
cmake -D CMAKE_INSTALL_PREFIX=/usr/local ..
make
make install
```

  - Other options include
    * Documentation: **-D DOCS=on**
    * Library Demos: **-D EXAMPLES=on**
    * Debugging (my favorite =P): **-D DEBUG=on -D DEBUG_ASSERT=on -D DEBUG_TRACE=on**

A framework bundle will automatically be built if it is detected that you are on OS X. There is currently no way of overriding this (to build a POSIX style install) other than editing the root project **CMakeLists.txt** file directly: option **FRAMEWORK** must be **"OFF"**.

Uninstall is provided by executing **make uninstall** inside your local build directory; or manually doing so by looking at the **cmake_uninstall.cmake** list provided in the local cmake build directory.

### Linux ###

Until I get around to writing proper instructions, you may take a look at my
.travis.yml build script in the root of this repository for hints.

### Windows ###

A slow but sure work in progress.

#### Linux Build Status ####

[![Build Status](https://travis-ci.org/i8degrees/nomlib.png?branch=master,dev)](https://travis-ci.org/i8degrees/nomlib)
