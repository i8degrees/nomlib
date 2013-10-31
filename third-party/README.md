# Third-Party Dependencies #

[nomlib's](http://github.com/i8degrees/nomlib/) third-party dependencies. Pre-packaged libraries are available for the OS X and Windows platforms and may be found listed as nomlib_**\<PLATFORM\>-dependencies** at:

* [nomlib's sourceforge.net project files](http://sourceforge.net/projects/nomlib/files/)

## Installation ##

Download the package file for your platform and extract inside the root of your project folder for nomlib. You should now have several libraries populating third-party/**\<EXTRACTED_PLATFORM\>**. No other dependency hell to ensure -- yay!

## Packaged Libraries ##

Both the 64-bit and 32-bit official development binaries versions of:

* [SDL v2.0.1](http://libsdl.org/)
* [SDL_image v2.0.0](http://www.libsdl.org/projects/SDL_image/)
* [SDL_ttf v2.0.12](http://www.libsdl.org/projects/SDL_ttf/)
  - Packaged only for OS X
      - [libsndfile v1.0.24](http://www.mega-nerd.com/libsndfile/)
        * Distributed framework is copied from the [SFML2 master branch](https://github.com/LaurentGomila/SFML/tree/master/) with me moving the distribution's **sndfile.h** file to it under a new Headers directory.

  - Packaged only for Windows
    * [libsndfile v1.0.25](http://www.mega-nerd.com/libsndfile/)
    * [OpenAL Soft v1.15.1](http://kcat.strangesoft.net/openal.html)

## Licensing ##

Licensing files for each library can be found at the project root of your nomlib directory under LICENSE_**\<LIBRARY_NAME\>**.
