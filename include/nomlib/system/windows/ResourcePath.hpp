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
#ifndef NOMLIB_WINDOWS_RESOURCE_PATH_HPP
#define NOMLIB_WINDOWS_RESOURCE_PATH_HPP

#include <string>
#include <cstring>

#include "nomlib/config.hpp"

namespace nom {

/// Obtain the path to the logged in user's Documents folder
///
///     $HOME/Documents
///
/// \remark This is a standard folder that may be used for saving user data
///
/// \note This function requires Windows Vista or above
///
/// \todo Support for when the user has changed the default save folder; see http://stackoverflow.com/a/12607759 and http://stackoverflow.com/questions/19553311/windows-8-how-to-read-the-user-documents-folder-path-programmatically-using-c
const std::string user_documents_path ( void );

/// Obtain the path to the logged in user's (local) Application Data folder
///
///     $HOME/AppData/Local
///
/// \remark This is a standard folder that may be used for saving user data
///
/// \note This function requires Windows Vista or above
///
/// \todo Support for when the user has changed the default save folder; see http://stackoverflow.com/a/12607759 and http://stackoverflow.com/questions/19553311/windows-8-how-to-read-the-user-documents-folder-path-programmatically-using-c
const std::string user_app_support_path ( void );

} // namespace nom

#endif // include guard
