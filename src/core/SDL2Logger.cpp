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
#include "nomlib/core/SDL2Logger.hpp"

// Private headers
#include "nomlib/core/ConsoleOutput.hpp"

namespace nom {

namespace priv {

void log_message( void* ptr, int cat, SDL_LogPriority prio, const char* msg )
{
  nom::ConsoleOutput os( std::cout );

  if( prio < NOM_NUM_LOG_PRIORITIES && prio > 0 )
  {
    if( prio < SDL_LOG_PRIORITY_INFO )
    {
      os.set_color( ConsoleOutput::Color::FG_YELLOW );
    }
    else if( prio == SDL_LOG_PRIORITY_INFO )
    {
      os.set_color( ConsoleOutput::Color::FG_BLUE );
    }
    else if( prio == SDL_LOG_PRIORITY_WARN )
    {
      os.set_color( ConsoleOutput::Color::FG_MAGENTA );
    }
    else if( prio >= SDL_LOG_PRIORITY_ERROR )
    {
      os.set_color( ConsoleOutput::Color::FG_RED );
      os.set_style( ConsoleOutput::STYLE_BOLD | ConsoleOutput::STYLE_UNDERLINED );
    }
  }

  std::cout << nom::SDL2Logger::priority_prefixes_[prio] << ":";

  os.endl();

  std::cout << " ";

  if( os.color() == ConsoleOutput::Color::FG_RED )
  {
    os.set_color( ConsoleOutput::Color::FG_PURPLE );
  }
  else if( os.color() == ConsoleOutput::Color::FG_BLUE )
  {
    os.set_color( ConsoleOutput::Color::FG_GREEN );
  }
  else if( os.color() == ConsoleOutput::Color::FG_MAGENTA )
  {
    os.set_color( ConsoleOutput::Color::FG_YELLOW );
  }
  else if( os.color() == ConsoleOutput::Color::FG_YELLOW )
  {
    os.set_color( ConsoleOutput::Color::FG_BLUE );
  }

  std::cout << msg << std::endl;
}

} // namespace priv

// Static initializations
bool SDL2Logger::initialized_ = false;
const std::string SDL2Logger::priority_prefixes_[NOM_NUM_LOG_PRIORITIES] =
{
  "NULL",
  "VERBOSE",
  "DEBUG",
  "INFO",
  "WARN",
  "ERROR",
  "CRITICAL"
};

// Static
bool SDL2Logger::initialized( void )
{
  return SDL2Logger::initialized_;
}

// Static
void SDL2Logger::initialize( void )
{
  if( SDL2Logger::initialized() == false )
  {
    // Mimic the default logging category priorities as per SDL2 logging
    // categories
    SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_APPLICATION, LogPriority::NOM_LOG_PRIORITY_INFO );
    SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_ASSERT, LogPriority::NOM_LOG_PRIORITY_WARN );
    SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TEST, LogPriority::NOM_LOG_PRIORITY_VERBOSE );

    // Log all messages from the engine's default NOM category
    SDL2Logger::set_logging_priority( NOM, LogPriority::NOM_LOG_PRIORITY_VERBOSE );

    // Register custom log outpt function for optional colored message support
    void ( *log_output_function )( void*, int, SDL_LogPriority, const char* ) = priv::log_message;
    SDL_LogSetOutputFunction( log_output_function, nullptr );

    SDL2Logger::initialized_ = true;
  }
}

SDL2Logger::SDL2Logger() :
  category_{ NOM },
  priority_{ LogPriority::NOM_LOG_PRIORITY_INFO }
{
  SDL2Logger::initialize();
}

SDL2Logger::SDL2Logger  (
                          int cat,
                          nom::LogPriority prio
                        ) :
  category_{ cat },
  priority_{ prio }
{
  SDL2Logger::initialize();
}

SDL2Logger::~SDL2Logger()
{
  this->write( "\n" );

  std::string out( this->output_stream().str() );

  SDL_LogMessage( this->category(), SDL2Logger::SDL_priority( this->priority() ), "%s", out.c_str() );
}

void SDL2Logger::write()
{
  // Do not output anything when there are void arguments
}

int SDL2Logger::category() const
{
  return this->category_;
}

LogPriority SDL2Logger::priority() const
{
  return this->priority_;
}

std::ostringstream& SDL2Logger::output_stream()
{
  return this->os_;
}

// Static
LogPriority SDL2Logger::priority( SDL_LogPriority prio )
{
  return NOM_SCAST( LogPriority, prio );
}

// Static
SDL_LogPriority SDL2Logger::SDL_priority( LogPriority prio )
{
  return NOM_SCAST( SDL_LogPriority, prio );
}

// Static
void SDL2Logger::set_logging_priorities( LogPriority prio )
{
  // SDL2Logger::initialize();

  SDL_LogSetAllPriority( NOM_SCAST( SDL_LogPriority, prio ) );
}

// Static
void SDL2Logger::set_logging_priority( int cat, nom::LogPriority prio )
{
  // SDL2Logger::initialize();

  SDL_LogSetPriority( cat, NOM_SCAST( SDL_LogPriority, prio ) );
}

} // namespace nom
