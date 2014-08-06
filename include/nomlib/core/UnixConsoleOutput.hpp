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
#ifndef NOMLIB_CORE_UNIX_CONSOLE_OUTPUT_HPP
#define NOMLIB_CORE_UNIX_CONSOLE_OUTPUT_HPP

#include <memory>

#include "nomlib/config.hpp"

#include "nomlib/core/ConsoleOutput.hpp"

namespace nom {

/// \see http://misc.flogisoft.com/bash/tip_colors_and_formatting
/// \see http://en.wikipedia.org/wiki/ANSI_escape_code#Colors
/// \see http://pubs.opengroup.org/onlinepubs/009695399/functions/isatty.html
class UnixConsoleOutput: public ConsoleOutput
{
  public:
    UnixConsoleOutput();

    /// \brief Destructor; if a TTY device is detected, an attempt to reset the
    /// colors will be made.
    virtual ~UnixConsoleOutput();

    /// \brief Implements ConsoleOutput::set_color.
    void set_color( ConsoleOutput::Color color );

    /// \brief Implements ConsoleOutput::set_style.
    void set_style( uint32 style );

    /// \brief Implements ConsoleOutput::reset_colors.
    void endl();

  private:
    /// \brief Check to see if we are running inside a TTY device (terminal).
    ///
    /// \remarks Color output for logging is enabled if we are in a TTY device.
    bool tty() const;

    /// \brief Reset colors
    const std::string COLOR_ENDL = "\e[0m";

    const std::string COLOR_FG_RED = "\033[31m";

    const std::string COLOR_FG_BLUE = "\033[34m";
    const std::string COLOR_FG_YELLOW = "\033[93m";
    const std::string COLOR_FG_MAGENTA = "\033[35m";

    const std::string COLOR_FG_GREEN = "\033[0;32m";
    const std::string COLOR_FG_ORANGE = "\033[0;33m";
    const std::string COLOR_FG_PURPLE = "\033[0;35m";

    const std::string COLOR_STYLE_BOLD = "\e[1;1m";
    const std::string COLOR_STYLE_UNDERLINED = "\e[1;4m";
};

} // namespace nom

#endif // include guard defined
