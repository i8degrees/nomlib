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
#include "nomlib/system/SDL2Logger.hpp"

// Private headers
// #include "nomlib/system/init.hpp"

namespace nom {

// Static initializations
bool SDL2Logger::initialized_ = false;

bool SDL2Logger::initialized( void )
{
  return SDL2Logger::initialized_;
}

void SDL2Logger::initialize( void )
{
  if( SDL2Logger::initialized() == false )
  {
    // nom::init_third_party( InitHints::SDL2 );

    // Mimic the default logging category priorities as per SDL2 logging
    // categories
    SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO );
    SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_ASSERT, SDL_LOG_PRIORITY_WARN );
    SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TEST, SDL_LOG_PRIORITY_VERBOSE );

    // Log all messages from the engine's default NOM category
    SDL2Logger::set_logging_priority( NOM, SDL_LOG_PRIORITY_VERBOSE );

    SDL2Logger::initialized_ = true;
  }
}

SDL2Logger::SDL2Logger( void ) :
  category_{ NOM },
  priority_{ SDL_LOG_PRIORITY_INFO }
{
  SDL2Logger::initialize();
}

SDL2Logger::SDL2Logger  (
                          int cat,
                          SDL_LogPriority prio
                        ) :
  category_{ cat },
  priority_{ prio }
{
  SDL2Logger::initialize();
}

SDL2Logger::~SDL2Logger( void )
{
  this->write( "\n" );

  SDL_LogMessage( this->category(), this->priority(), "%s", this->output_cstring() );
}

void SDL2Logger::write( void )
{
  // Do not output anything when there are void arguments
}

int SDL2Logger::category( void ) const
{
  return this->category_;
}

SDL_LogPriority SDL2Logger::priority( void ) const
{
  return this->priority_;
}

std::ostringstream& SDL2Logger::output_stream( void )
{
  return this->os_;
}

std::string SDL2Logger::output_string( void ) const
{
  return this->os_.str();
}

const char* SDL2Logger::output_cstring( void )
{
  return this->output_string().c_str();
}

// void SDL2Logger::set_category( int cat )
// {
//   this->category_ = cat;
// }

// void SDL2Logger::set_priority( SDL_LogPriority p )
// {
//   this->priority_ = p;
// }

void SDL2Logger::set_logging_priorities( SDL_LogPriority prio )
{
  // SDL2Logger::initialize();

  SDL_LogSetAllPriority( prio );
}

void SDL2Logger::set_logging_priority( int cat, SDL_LogPriority prio )
{
  // SDL2Logger::initialize();

  SDL_LogSetPriority( cat, prio );
}

} // namespace nom
