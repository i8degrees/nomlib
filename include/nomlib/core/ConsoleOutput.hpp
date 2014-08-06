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
#ifndef NOMLIB_CORE_CONSOLE_OUTPUT_HPP
#define NOMLIB_CORE_CONSOLE_OUTPUT_HPP

#include <memory>

#include "nomlib/config.hpp"

namespace nom {

/// \brief Platform-independent color output interface
///
/// \remarks This is intended to be a minimal interface for debug logging aid.
///
/// \see nom::SDL2Logger
class ConsoleOutput
{
  public:
    enum Color
    {
      FG_WHITE,     /// \todo Implement in UnixConsoleOutput
      FG_RED,
      FG_GREEN,
      FG_BLUE,
      FG_MAGENTA,
      FG_YELLOW,
      FG_ORANGE,    /// \todo Implement in WindowsConsoleOutput
      FG_PURPLE,    /// \todo Implement in WindowsConsoleOutput
    };

    enum: uint32
    {
      /// \brief Resets the text colors
      STYLE_NONE = 0,
      STYLE_BOLD = 0x1,
      STYLE_UNDERLINED = 0x2
    };

    /// \brief Default constructor; initializes the output stream to use
    /// std::cerr (the standard error stream).
    ConsoleOutput();

    /// \brief Construct a console output object with a custom output stream.
    ///
    /// \param os std::cerr, std::cout, std::ofstream, etc.
    ConsoleOutput( std::ostream& os );

    virtual ~ConsoleOutput();

    /// \brief Get the last color that was set.
    ///
    /// \remarks The last color used is updated upon every successful call to
    /// ::set_color.
    ConsoleOutput::Color color() const;

    virtual void set_color( ConsoleOutput::Color color );

    virtual void set_style( uint32 style );

    /// \brief Reset the colors that have been set.
    ///
    /// \remarks The intention of this call is to restore the screen buffer
    /// back to its previous state (before modification from our process).
    virtual void endl();

  protected:
    /// \brief The output stream to use; i.e.: std::cout or std::cerr.
    std::ostream& out_;

    /// \brief The last color used.
    ///
    /// \remarks Used in SDL2Logger::log_message for rotating the two colors
    /// used for a message -- the logging priority and the message contents.
    ConsoleOutput::Color lcolor_;

  private:
    /// \brief The platform-dependent implementation to use.
    ///
    /// \see nom::UnixConsoleOutput, nom::WindowsConsoleOutput
    std::unique_ptr<ConsoleOutput> impl_;
};

} // namespace nom

#endif // include guard defined
