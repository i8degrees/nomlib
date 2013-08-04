nomlib
======

C++11 cross-platform game engine

Projects Using nomlib
---------------------

* [i8degrees/ttcards](https://github.com/i8degrees/ttcards)

Building Instructions
---------------------

### Dependencies ###

* cmake v2.6+
* SDL v1.2.15
* SDL_image
* SDL_ttf
* OpenAL
* libsndfile
* libmagic
* A recent compiler that has support for the latest C++11 extensions

### OSX ###

After installing the necessary dependencies listed above -- either through brew
or perhaps hunting down each individual dependency Framework:

```
  git clone https://github.com/i8degrees/nomlib
  cd nomlib
  mkdir build && cd build
```

* Dynamic library installed under default CMAKE_INSTALL_PREFIX (/usr or /usr/local):

```
  cmake ..
  make
  make install
```

* Framework bundle:

```
  cmake -D FRAMEWORK=on ..
  make
  make install && cp -av nomlib.framework $HOME/Library/Frameworks/.
```

Uninstall support is provided by running 'make uninstall' within the build directory.

### Linux ###

* Until I get around to writing proper instructions, you may take a look at my
.travis.yml build script in the root of this repository.

#### Linux Build Status ####

[![Build Status](https://travis-ci.org/i8degrees/nomlib.png?branch=master,dev)](https://travis-ci.org/i8degrees/nomlib)
