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
#include "nomlib/system/Timer.hpp"

// Private headers (third-party libs)
#include "SDL.h"

namespace nom {

Timer::Timer ( void )
{
//NOM_LOG_TRACE ( NOM );

  if ( SDL_WasInit( SDL_INIT_TIMER ) == false )
  {
    if ( SDL_InitSubSystem ( SDL_INIT_TIMER ) == -1 )
    {
NOM_LOG_ERR ( NOM, SDL_GetError() );
      return;
    }
  }

  this->timer_started = false;
  this->timer_paused = false;
  this->elapsed_ticks = 0;
  this->paused_ticks = 0;
}

Timer::~Timer ( void )
{
//NOM_LOG_TRACE ( NOM );

  SDL_QuitSubSystem ( SDL_INIT_TIMER );
}

void Timer::start ( void )
{
  this->elapsed_ticks = SDL_GetTicks();
  this->timer_started = true;
  this->timer_paused = false;
}

void Timer::stop ( void )
{
  this->elapsed_ticks = 0;
  this->timer_started = false;
  this->timer_paused = false;
}

void Timer::restart ( void )
{
  this->start();
}

void Timer::pause ( void )
{
  if ( ( this->timer_started == true ) && ( this->timer_paused == false ) )
  {
    this->timer_paused = true;
    this->paused_ticks = SDL_GetTicks() - this->elapsed_ticks;
  }
}

void Timer::unpause ( void )
{
  if ( this->timer_paused == true )
  {
    this->timer_paused = false;
    this->elapsed_ticks = SDL_GetTicks() - this->paused_ticks;
    this->paused_ticks = 0;
  }
}

uint32 Timer::ticks ( void ) const
{
  if ( this->timer_started == true )
  {
    if ( this->timer_paused == true )
    {
      return this->paused_ticks;
    }
    else
    {
      return SDL_GetTicks() - this->elapsed_ticks;
    }
  }

  // Timer is not running
  return 0;
}

bool Timer::started ( void ) const
{
  return this->timer_started;
}

bool Timer::paused ( void ) const
{
  return this->timer_paused;
}

const std::string Timer::ticksAsString ( void ) const
{
  return std::to_string ( static_cast<uint32> ( this->ticks() ) );
}

uint32 Timer::seconds ( float milliseconds ) const
{
  return static_cast<uint32> ( milliseconds * 1000.f );
}

} // namespace nom
