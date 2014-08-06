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
#ifndef NOMLIB_CORE_WINDOWS_CONSOLE_OUTPUT_HPP
#define NOMLIB_CORE_WINDOWS_CONSOLE_OUTPUT_HPP

#include <memory>

#include <cstdio>
#include <windows.h>
#include <winnt.h>

#include "nomlib/config.hpp"

#include "nomlib/core/ConsoleOutput.hpp"

namespace nom {

/// \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms682088(v=vs.85).aspx#_win32_character_attributes
/// \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms686047%28v=vs.85%29.aspx
/// \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms683171%28v=vs.85%29.aspx
class WindowsConsoleOutput: public ConsoleOutput
{
  public:
    WindowsConsoleOutput();

    /// \brief Destructor; if a console is detected, an attempt to reset the
    /// colors will be made.
    virtual ~WindowsConsoleOutput();

    /// \brief Implements ConsoleOutput::set_color.
    void set_color( ConsoleOutput::Color color );

    /// \brief Implements ConsoleOutput::set_style.
    ///
    /// \remarks Only ConsoleOutput::STYLE_NONE is implemented.
    ///
    /// \todo Implement styles by adding FOREGROUND_INTENSITY mask onto the
    /// current color?
    void set_style( uint32 style );

    /// \brief Implements ConsoleOutput::reset_colors.
    void endl();

  private:
    /// \brief Resource handle for access into console API
    HANDLE console_;

    /// \brief Store the screen buffer state before we modify it, so we can
    // restore the original state when we are done.
    WORD color_attrs_;

    CONSOLE_SCREEN_BUFFER_INFO buffer_info_;

    /// \brief White foreground color for cmd.exe.
    WORD fg_white_ = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    /// \brief Bold red foreground color for cmd.exe.
    WORD fg_red_ = FOREGROUND_RED | FOREGROUND_INTENSITY;

    /// \brief Bold green foreground color for cmd.exe.
    WORD fg_green_ = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    /// \brief Bold blue foreground color for cmd.exe.
    WORD fg_blue_ = FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    /// \brief Bold magenta foreground color for cmd.exe.
    WORD fg_magenta_ = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    /// \brief Bold yellow foreground color for cmd.exe.
    WORD fg_yellow_ = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
};

} // namespace nom

#endif // include guard defined
