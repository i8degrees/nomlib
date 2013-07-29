XCODE 4 TEMPLATES
=================

These templates help you to easily create a new nomlib project for Xcode 4.

Author & License
----------------

The templates were written by Marco Antognini <antognini.marco@gmail.com> and are provided under the terms of the zlib/png license : http://opensource.org/licenses/zlib-license.php

Due credit & many thanks goes to Marco Antognini and Laurent Gomila (See http://www.sfml-dev.org).
nomlib's Xcode 4 Templates, including this README text derive from the [SFML v2
public git repository](https://github.com/LaurentGomila/SFML).

Thanks you! Seriously. :-) I would have never gotten around to writing these
scripts otherwise.

Features
--------

 * You can choose between command line tool or bundle application, the latter will contains all nomlib dependencies so you can run your app on another computer without manually installing nomlib.
 * You can choose between using nomlib libraries as dylibs or frameworks.
 * You can choose your compiler and C++ standard library / dialect.
 * <strikethrough>You can choose which nomlib module(s) you want to use into your project.</strikethrough>
 * You automatically get a basic example to test nomlib quickly and easily.

Prerequisites
-------------

Before installing the template, make sure you have installed:

 - Xcode 4 (the templates were tested with Xcode 4.6.2)
 - nomlib, either as framework or dylibs binaries

* The frameworks distribution needs to be installed in either $HOME/Library/Frameworks or /Library/Frameworks and the dylibs into /usr/local/lib.

If both the dylibs and the Frameworks are installed, the Frameworks will be chosen by default; if you have the Frameworks installed at both installation target locations -- $HOME/Library/Frameworks and /Library/Frameworks -- your local Frameworks directory at $HOME/Library/Frameworks is chosen first.

Install
-------

If you are building nomlib from sources you can set CMake's INSTALL_XCODE4_TEMPLATES variable to TRUE to install the templates automatically.

Otherwise proceed as follows:

  1. Ensure that "$HOME/Library/Developer/Xcode/Templates/" directory exists

  2. Copy "nomlib" folder into the above directory.

Usage
-----

Once the templates are installed as per above, the templates should be available
upon starting a new XCode project.

Frequently Asked Questions
--------------------------

  * Can I use these templates with Xcode 3 ?
    > No, Xcode 3 and 4 don't have the same template system. Therefore these templates won't work with a older version of Xcode. Sorry!

  * Am I able to switch back and forth from using the dylibs and frameworks in my project?
    > Yes. The NOMLIB_BINARY_TYPE variable within your project's "Build Settings" can be set to either "DYLIBS" or "FRAMEWORKS".

  * How to switch back and forth from using the debug libraries?
    > Find the variable NOMLIB_LINK_DYLIBS_SUFFIX_DEBUG within your project's build settings and set this to "-d" for the debug libraries, or an empty string ( "" ) to use only the release binaries.
