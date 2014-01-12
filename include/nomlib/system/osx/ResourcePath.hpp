/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_OSX_RESOURCE_PATH_HEADERS
#define NOMLIB_OSX_RESOURCE_PATH_HEADERS

#include <string>
#include <cstring>

#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>

#include "nomlib/config.hpp"

namespace nom {

/// Returns the path to the Resources folder within a bundle
///
/// If no identifier argument is passed, it is assumed you are looking for the
/// main application bundle's Resources path, otherwise the identifier is used
/// to locate a particular bundle from within another bundle.
///
/// The bundle's identifier can be found within the Info.plist file of said
/// bundle, and typically is in reverse DNS notation. The string might look
/// something like this:
///
///   org.dev.nomlib
///
///   or, as per the official API documentation,
///
///   com.apple.Finder.MyGetInfoPlugIn
///
/// The one restriction you have in using this function call is that the bundle
/// whose identifier you have passed must already be loaded into the application
/// bundle, or else this function call will fail.
///
/// Returns a null terminated string on err
const std::string getBundleResourcePath ( const std::string& identifier = "\0" );

/// Obtain the path to the logged in user's Documents folder
///
///     $HOME/Documents/
///
/// These are standard folders that may be used for saving user data under.
///
/// \fixme FSFindFolder and FSRefMakePath are deprecated in OS X v10.8+
const std::string user_documents_path ( void );

/// Obtain the path to the logged in user's Application Support folder
///
///     $HOME/Library/Application\ Support/
///
/// These are standard folders that may be used for saving user data under.
///
/// \fixme FSFindFolder and FSRefMakePath are deprecated in OS X v10.8+
const std::string user_app_support_path ( void );

} // namespace nom

#endif // include guard
