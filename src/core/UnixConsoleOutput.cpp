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
#include "nomlib/core/UnixConsoleOutput.hpp"

// Private header files
#include <unistd.h>

// FIXME: The disabled logging macros are necessary in order to prevent
// crashing upon attempted usage of said macro.
#undef NOM_LOG_MESSAGE
#undef NOM_LOG_TRACE_PRIO

#define NOM_LOG_MESSAGE( cat, prio, ... )
#define NOM_LOG_TRACE_PRIO( cat, prio )

namespace nom {

UnixConsoleOutput::UnixConsoleOutput()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_SYSTEM, NOM_LOG_PRIORITY_VERBOSE );
}

UnixConsoleOutput::~UnixConsoleOutput()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_SYSTEM, NOM_LOG_PRIORITY_VERBOSE );

  if( this->tty() == true )
  {
    this->endl();
  }
}

void UnixConsoleOutput::set_color( ConsoleOutput::Color color )
{
  if( this->tty() == false ) return;

  if( color == ConsoleOutput::Color::FG_RED )
  {
    this->out_ << COLOR_FG_RED;
  }
  else if( color == ConsoleOutput::Color::FG_GREEN )
  {
    this->out_ << COLOR_FG_GREEN;
  }
  else if( color == ConsoleOutput::Color::FG_BLUE )
  {
    this->out_ << COLOR_FG_BLUE;
  }
  else if( color  == ConsoleOutput::Color::FG_MAGENTA )
  {
    this->out_ << COLOR_FG_MAGENTA;
  }
  else if( color == ConsoleOutput::Color::FG_YELLOW )
  {
    this->out_ << COLOR_FG_YELLOW;
  }
  else if( color == ConsoleOutput::Color::FG_ORANGE )
  {
    this->out_ << COLOR_FG_ORANGE;
  }
  else if( color == ConsoleOutput::Color::FG_PURPLE )
  {
    this->out_ << COLOR_FG_PURPLE;
  }
}

void UnixConsoleOutput::set_style( uint32 style )
{
  if( this->tty() == false ) return;

  if( style == ConsoleOutput::STYLE_NONE )
  {
    this->endl();
  }

  if( style & ConsoleOutput::STYLE_BOLD )
  {
    this->out_ << COLOR_STYLE_BOLD;
  }

  if( style & ConsoleOutput::STYLE_UNDERLINED )
  {
    this->out_ << COLOR_STYLE_UNDERLINED;
  }
}

void UnixConsoleOutput::endl()
{
  this->out_ << COLOR_ENDL;
}

// Private scope

bool UnixConsoleOutput::tty() const
{
  return isatty( fileno( stdout ) );
}

} // namespace nom
