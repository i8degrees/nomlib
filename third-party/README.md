# Third-Party Dependencies #

[nomlib's](http://github.com/i8degrees/nomlib/) third-party dependencies. Pre-packaged libraries are available for the OS X, iOS and Windows platforms and may be found listed as nomlib_**\<PLATFORM\>-dependencies** at:

* [nomlib's sourceforge.net project files](http://sourceforge.net/projects/nomlib/files/)

## Installation ##

Download the package file for your platform and extract inside the third-party folder inside the project root. You should now have several libraries populating third-party/**\<EXTRACTED_PLATFORM\>**. No other dependency hell to ensure -- yay!

## Packaged Libraries ##

Both the 64-bit and 32-bit official development binaries versions of:

* [SDL v2.0.3](http://libsdl.org/)
* [SDL_image v2.0.0](http://www.libsdl.org/projects/SDL_image/)
* [SDL_ttf v2.0.12](http://www.libsdl.org/projects/SDL_ttf/)
* [RapidXML v1.13](https://sourceforge.net/p/rapidxml) with [bug fix #16](https://sourceforge.net/p/rapidxml/bugs/16/) applied to rapidxml_print.hpp.
* [Google Test v1.7.0](https://code.google.com/p/googletest/)
* [JsonCpp v0.6.0-rc2](https://sf.net/p/jsoncpp) built with its distributed amalgamate.py script.
* [TCLAP headers on branch master, commit 12cee3](https://sourceforge.net/p/tclap/code/ci/master/tree/)

  * Packaged only under Mac OS X
      * [libsndfile v1.0.24](http://www.mega-nerd.com/libsndfile/)
        * Distributed framework is copied from the [SFML2 master branch](https://github.com/LaurentGomila/SFML/tree/master/) with me moving the distribution's **sndfile.h** file to it under a new Headers directory.

      * [SDL2_image.framework v2.0.0](http://libsdl.org/projects/SDL_image) built with SDL_IMAGE_USE_COMMON_BACKEND (in order to fix critical pixel mismatch issue; see also https://forums.libsdl.org/viewtopic.php?t=10013&highlight=perhaps+again+pixel+bug+mac).
        * [libpng v1.5.18](https://github.com/Homebrew/homebrew/commits/master/Library/Formula/libpng.rb) from Homebrew's repository with the --universal build flag.
        * [libtiff v4.0.3](https://github.com/Homebrew/homebrew/commits/master/Library/Formula/libtiff.rb) from Homebrew's repository with the --universal build flag.
        * [libjpeg 8d](https://github.com/Homebrew/homebrew/commits/master/Library/Formula/jpeg.rb) from Homebrew's repository with the --universal build flag.

      * [SDL2_ttf.framework v2.0.12](http://libsdl.org/projects/SDL_ttf) built with a [font kerning size patch](https://bugzilla.libsdl.org/show_bug.cgi?id=2572).
    
      * [libRocket v1.3.0](http://librocket.com) compiled with the FreeType v2.3.5 libraries from [homebrew](http://brew.sh).
      ```
      # dynamic libs
      cmake -DBUILD_SAMPLES=off -DBUILD_LUA_BINDINGS=on -DCMAKE_BUILD_TYPE=Debug -DROCKET_DEBUG=on -DCMAKE_VERBOSE_MAKEFILE=1 ..

      # static libs; Release target because Debug is way too large (+158MB)
      cmake -DBUILD_SAMPLES=off -DBUILD_LUA_BINDINGS=on -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=off -DROCKET_DEBUG=on -DCMAKE_VERBOSE_MAKEFILE=1 ..
      ```

  - Packaged only for Windows
    * [Google Test v1.7.0](https://code.google.com/p/googletest/) built with:
    ```
    cmake -DBUILD_SHARED_LIBS=on ..
    msbuild /t:build gtest.vcxproj
    ```

    * [libsndfile v1.0.25](http://www.mega-nerd.com/libsndfile/)
    * [OpenAL Soft v1.15.1](http://kcat.strangesoft.net/openal.html)
    * [libRocket v1.3.0](http://librocket.com) compiled with the FreeType v2.3.5 libraries from [GNU win32 FreeType](http://gnuwin32.sourceforge.net/packages/freetype.htm), 'binaries' release.
      * [GLEW v1.10.0](http://glew.sourceforge.net)

## Licensing ##

Licensing files for each library can be found at the project root of your nomlib directory under LICENSE_**\<LIBRARY_NAME\>**.
