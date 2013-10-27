/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#ifndef NOMLIB_SDL2_DIALOG_MESSAGEBOX_HPP
#define NOMLIB_SDL2_DIALOG_MESSAGEBOX_HPP

#include <string>

#include "SDL.h"

#include "nomlib/config.hpp"

namespace nom {

enum MessageBoxType
{
  NOM_DIALOG_ERROR = SDL_MESSAGEBOX_ERROR,
  NOM_DIALOG_WARNING = SDL_MESSAGEBOX_WARNING,
  NOM_DIALOG_INFORMATION = SDL_MESSAGEBOX_INFORMATION
};

/// Display a modal dialog message box.
///
/// \param    title     The title of the dialog message box
/// \param    message   Message on the dialog message box
/// \param    type      Type of dialog message box; see nom::MessageBoxType for
///                     the possible types to display.
/// \param    parent    Optional pointer to the parent window to spawn from
///
/// Returns zero (0) on success, -1 on err
///
/// \todo
/// Implement a fall-back mechanism for when video subsystem fails to initialize
/// Jeffrey Carpenter <jeffrey.carp@gmail.com> @ 2013-10-03
int32 DialogMessageBox  (
                          const std::string& title, const std::string& message,
                          enum MessageBoxType type = NOM_DIALOG_INFORMATION,
                          SDL_Window* parent = nullptr
                        );


} // namespace nom

#endif // include guard defined
