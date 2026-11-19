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
#include "nomlib/actions/WaitForDurationAction.hpp"

#include "nomlib/core/unique_ptr.hpp"

namespace nom {

// Static initializations
const char* WaitForDurationAction::DEBUG_CLASS_NAME =
  "[WaitForDurationAction]:";

//! [creating_custom_actions]

WaitForDurationAction::WaitForDurationAction(real32 seconds)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_duration(seconds);
  this->elapsed_frames_ = 0.0f;
}

WaitForDurationAction::~WaitForDurationAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<IActionObject> WaitForDurationAction::clone() const
{
  return( nom::make_unique<self_type>( self_type(*this) ) );
}

IActionObject::FrameState WaitForDurationAction::next_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  if( this->timer_.started() == false ) {
    this->timer_.start();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "BEGIN at", delta_time );
  }

  // Clamp delta values that go beyond maximal duration
  if( delta_time > (this->duration() / this->speed() ) ) {
    delta_time = this->duration() / this->speed();
  }

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  if( delta_time < (this->duration() / this->speed() ) ) {

    ++this->elapsed_frames_;

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "delta_time:", delta_time,
                    "frame_time:", frame_time,
                    "[elapsed frames]:", this->elapsed_frames_ );

    this->set_status(FrameState::PLAYING);
  } else {
    this->set_status(FrameState::COMPLETED);
  }

  return this->status();
}

IActionObject::FrameState WaitForDurationAction::prev_frame(real32 delta_time)
{
  // NOTE: This action is not reversible
  return this->next_frame(delta_time);
}

void WaitForDurationAction::pause(real32 delta_time)
{
  this->timer_.pause();
}

void WaitForDurationAction::resume(real32 delta_time)
{
  this->timer_.unpause();
}

void WaitForDurationAction::rewind(real32 delta_time)
{
  this->elapsed_frames_ = 0.0f;
  this->timer_.stop();
  this->set_status(FrameState::PLAYING);
}

void WaitForDurationAction::release()
{
  // Nothing to free!
}

//! [creating_custom_actions]

} // namespace nom
