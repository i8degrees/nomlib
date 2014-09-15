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
#include "nomlib/core/ConsoleOutput.hpp"

#if defined( NOM_PLATFORM_POSIX )
  #include "nomlib/core/UnixConsoleOutput.hpp"
#elif defined( NOM_PLATFORM_WINDOWS )
  #include "nomlib/core/WindowsConsoleOutput.hpp"
#else
  #pragma message ( "ConsoleOutput is not implemented for this platform!" )
#endif

// FIXME: The disabled logging macros are necessary in order to prevent
// crashing upon attempted usage of said macro.
#undef NOM_LOG_MESSAGE
#undef NOM_LOG_TRACE_PRIO

#define NOM_LOG_MESSAGE( cat, prio, ... )
#define NOM_LOG_TRACE_PRIO( cat, prio )

namespace nom {

ConsoleOutput::ConsoleOutput() :
  out_{ std::cerr }
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_SYSTEM, NOM_LOG_PRIORITY_VERBOSE );
}

ConsoleOutput::ConsoleOutput( std::ostream& os ) :
  out_{ os }
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_SYSTEM, NOM_LOG_PRIORITY_VERBOSE );

  #if defined( NOM_PLATFORM_POSIX )
    this->impl_.reset( new UnixConsoleOutput() );

    // This is not yet implemented for Windows
    if( this->use_color() == false )
    {
      // Effectively disables color output
      this->impl_.reset(nullptr);
    }
  #elif defined( NOM_PLATFORM_WINDOWS )
    this->impl_.reset( new WindowsConsoleOutput() );
  #else
    NOM_ASSERT( this->impl_ != nullptr );
  #endif
}

ConsoleOutput::~ConsoleOutput()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_SYSTEM, NOM_LOG_PRIORITY_VERBOSE );
}

ConsoleOutput::Color ConsoleOutput::color() const
{
  return this->lcolor_;
}

bool ConsoleOutput::use_color() const
{
  std::string val = this->shell.env("NOM_COLOR");

  if( val == "TRUE" || val == "true" )
  {
    return true;
  }

  return false;
}

void ConsoleOutput::set_color( ConsoleOutput::Color color )
{
  if( this->impl_ != nullptr )
  {
    this->impl_->set_color( color );
    this->lcolor_ = color;
  }
}

void ConsoleOutput::set_style( uint32 style )
{
  if( this->impl_ != nullptr )
  {
    this->impl_->set_style( style );
  }
}

void ConsoleOutput::endl()
{
  if( this->impl_ != nullptr )
  {
    this->impl_->endl();
  }
}

} // namespace nom
