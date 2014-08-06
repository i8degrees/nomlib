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
#include "nomlib/core/WindowsConsoleOutput.hpp"

// FIXME: The disabled logging macros are necessary in order to prevent
// crashing upon attempted usage of said macro.
#undef NOM_LOG_MESSAGE
#undef NOM_LOG_TRACE_PRIO

#define NOM_LOG_MESSAGE( cat, prio, ... )
#define NOM_LOG_TRACE_PRIO( cat, prio )

namespace nom {

WindowsConsoleOutput::WindowsConsoleOutput()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_SYSTEM, NOM_LOG_PRIORITY_VERBOSE );

  this->console_ = GetStdHandle( STD_OUTPUT_HANDLE );

  // Save the current text colors in use so we can restore the screen buffer to
  // its original state when we are through with it.
  GetConsoleScreenBufferInfo( this->console_, &this->buffer_info_ );
  this->color_attrs_ = this->buffer_info_.wAttributes;
}

WindowsConsoleOutput::~WindowsConsoleOutput()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_SYSTEM, NOM_LOG_PRIORITY_VERBOSE );

  this->endl();
}

void WindowsConsoleOutput::set_color( ConsoleOutput::Color color )
{
  if( color == ConsoleOutput::Color::FG_WHITE )
  {
    SetConsoleTextAttribute( this->console_, this->fg_white_ );
  }
  else if( color == ConsoleOutput::Color::FG_RED )
  {
    SetConsoleTextAttribute( this->console_, this->fg_red_ );
  }
  else if( color == ConsoleOutput::Color::FG_GREEN )
  {
    SetConsoleTextAttribute( this->console_, this->fg_green_ );
  }
  else if( color == ConsoleOutput::Color::FG_BLUE )
  {
    SetConsoleTextAttribute( this->console_, this->fg_blue_ );
  }
  else if( color == ConsoleOutput::Color::FG_MAGENTA )
  {
    SetConsoleTextAttribute( this->console_, this->fg_magenta_ );
  }
  else if( color == ConsoleOutput::Color::FG_YELLOW )
  {
    SetConsoleTextAttribute( this->console_, this->fg_yellow_ );
  }
  else if( color == ConsoleOutput::Color::FG_ORANGE )
  {
    // TODO
  }
  else if( color == ConsoleOutput::Color::FG_PURPLE )
  {
    // TODO
  }
}

void WindowsConsoleOutput::set_style( uint32 style )
{
  if( style == ConsoleOutput::STYLE_NONE )
  {
    this->endl();
  }

  if( style & ConsoleOutput::STYLE_BOLD )
  {
    // ...
  }

  if( style & ConsoleOutput::STYLE_UNDERLINED )
  {
    // ...
  }
}

void WindowsConsoleOutput::endl()
{
  // TODO (handle screen buffer state properly, AKA save & restore):

  GetConsoleScreenBufferInfo( this->console_, &this->buffer_info_ );

  // Restores the text colors ... hopefully to their state before we modified
  // the buffer!
  SetConsoleTextAttribute( this->console_, this->color_attrs_ );
}

} // namespace nom
