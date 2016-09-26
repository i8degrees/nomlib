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
#include "nomlib/system/HighResolutionTimer.hpp"

// Private headers
#include <string>

#include <SDL.h>

#include "nomlib/core/clock.hpp"

namespace nom {

HighResolutionTimer::HighResolutionTimer() :
  paused_(false),
  started_(false),
  elapsed_ticks_(0),
  paused_ticks_(0)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);

  // TODO: This should probably be taken care of in nom::init or so??
  // if( SDL_WasInit( SDL_INIT_TIMER ) == false ) {
  //   if( SDL_InitSubSystem ( SDL_INIT_TIMER ) == -1 ) {
  //     NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
  //                   "Could not initialize timing subsystem:", SDL_GetError() );
  //   }
  // }
}

HighResolutionTimer::~HighResolutionTimer()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);
}

bool HighResolutionTimer::started() const
{
  return this->started_;
}

bool HighResolutionTimer::paused() const
{
  return this->paused_;
}

uint64 HighResolutionTimer::ticks() const
{
  if( this->started() == true ) {
    if( this->paused() == true ) {
      return this->paused_ticks_;
    } else {
      return nom::hires_ticks() - this->elapsed_ticks_;
    }
  }

  return 0; // Timer is not running
}

std::string HighResolutionTimer::ticks_str() const
{
  return std::to_string( this->ticks() );
}

void HighResolutionTimer::start()
{
  this->elapsed_ticks_ = nom::hires_ticks();
  this->started_ = true;
  this->paused_ = false;
}

void HighResolutionTimer::stop()
{
  this->elapsed_ticks_ = 0;
  this->started_ = false;
  this->paused_ = false;
}

void HighResolutionTimer::restart()
{
  this->start();
}

void HighResolutionTimer::pause()
{
  if( this->started() == true && this->paused() == false ) {
    this->paused_ = true;
    this->paused_ticks_ = nom::hires_ticks() - this->elapsed_ticks_;
  }
}

void HighResolutionTimer::unpause()
{
  if( this->paused() == true ) {
    this->paused_ = false;
    this->elapsed_ticks_ = nom::hires_ticks() - this->paused_ticks_;
    this->paused_ticks_ = 0;
  }
}

real64
HighResolutionTimer::elapsed_ticks( uint64 start_hires_ticks,
                                    uint64 end_hires_ticks )
{
  real64 result = ( (real32)(end_hires_ticks - start_hires_ticks) /
                    (real64)nom::hires_frequency() );

  return result;
}

real64 HighResolutionTimer::to_milliseconds(uint64 hires_ticks)
{
  real64 result = ( (1000.0f * (real64)(hires_ticks) /
                    (real64)nom::hires_frequency() ) );

  return result;
}

real64 HighResolutionTimer::to_seconds() const
{
  auto elapsed_ticks = this->ticks();

  real64 result = this->to_seconds(elapsed_ticks);

  return result;
}

real64 HighResolutionTimer::to_seconds(uint64 hires_ticks)
{
  real64 result =
    ( (HighResolutionTimer::to_milliseconds(hires_ticks) / 1000.0f) );

  return result;
}

} // namespace nom
