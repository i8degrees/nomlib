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
#include "nomlib/graphics/SpriteBatchAction.hpp"

// Private headers
#include "nomlib/math/math_helpers.hpp"
// #include "nomlib/graphics/RenderWindow.hpp"

// Forward declarations
#include "nomlib/graphics/sprite/SpriteBatch.hpp"

namespace nom {

SpriteBatchAction::SpriteBatchAction( const std::shared_ptr<SpriteBatch>& sprite,
                                      real32 frame_interval )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->curr_frame_ = 0.0f;
  this->set_frame_interval(frame_interval);

  this->sprite_ = sprite;
  NOM_ASSERT(this->sprite_->frames() > 0);
  this->total_displacement_ = this->sprite_->frames();

  real32 interval_seconds =
    this->frame_interval_ / 1000.0f;

  real32 action_duration =
    ( interval_seconds  * this->sprite_->frames() ) / this->speed();

  this->set_duration(action_duration);

  this->last_delta_ = 0;
}

SpriteBatchAction::~SpriteBatchAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<SpriteBatchAction::derived_type>
SpriteBatchAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState
SpriteBatchAction::update(real32 t, real32 b, real32 c, real32 d)
{
  real32 frame_interval =
    this->frame_interval_ / this->speed();

  // The current frame to scale
  real32 delta_time = t;

  // Total duration of the action
  const real32 duration = d;

  // The computed texture frame to show next
  real32 displacement(0.0f);

  // Clamp delta values that go beyond the time duration bounds; this adds
  // stability to variable time steps
  if( delta_time > (duration / this->speed() ) ) {
    delta_time = duration / this->speed();
  }

  NOM_ASSERT(this->timing_mode() != nullptr);

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  displacement =
    this->timing_mode().operator()(frame_time, b, c, duration);

  this->curr_frame_ = displacement;

  if( delta_time >= (this->last_delta_ + frame_interval) ) {

    this->last_delta_ = delta_time;

    if( this->sprite_ != nullptr ) {
      this->sprite_->set_frame(displacement);
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION, "[SpriteBatchAction]",
                    "delta_time:", delta_time, "frame_time:", frame_time,
                    "[elapsed frames]:", this->curr_frame_,
                    "[fps]:", this->frame_interval_ );
  } // end if delta_time **greater than or equal to** elapsed interval frames

  // Continue playing the animation only when we are inside our frame duration
  // bounds; this adds stability to variable time steps
  if( delta_time < (duration / this->speed() ) ) {

    this->status_ = FrameState::PLAY_NEXT_FRAME;
    return this->status_;
  } else {
    this->last_frame(delta_time);

    this->status_ = FrameState::DONE;
    return this->status_;
  } // end if delta_time **less than** action's duration
}

IActionObject::FrameState SpriteBatchAction::next_frame(real32 delta_time)
{
  this->first_frame(delta_time);

  delta_time = this->timer_.ticks();

  real32 initial_frame = 0.0f;
  return( this->update( delta_time, initial_frame, this->total_displacement_,
          this->duration() ) );
}

IActionObject::FrameState SpriteBatchAction::prev_frame(real32 delta_time)
{
  this->first_frame(delta_time);

  delta_time = this->timer_.ticks();

  real32 initial_frame = this->sprite_->frames();
  return( this->update( delta_time, initial_frame, -(this->total_displacement_),
          this->duration() ) );
}

void SpriteBatchAction::pause(real32 delta_time)
{
  this->timer_.pause();
}

void SpriteBatchAction::resume(real32 delta_time)
{
  this->timer_.unpause();
}

void SpriteBatchAction::rewind(real32 delta_time)
{
  // Reset frame cycle back to initial value
  this->curr_frame_ = 0.0f;
  this->last_delta_ = 0;

  // Reset frame timing
  this->timer_.stop();
}

void SpriteBatchAction::release()
{
  this->sprite_.reset();
}

// Private scope

void SpriteBatchAction::set_frame_interval(real32 seconds)
{
  this->frame_interval_ = seconds * 1000.0f;
}

void SpriteBatchAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {

    // Start frame timing
    this->curr_frame_ = 0.0f;
    this->timer_.start();
    delta_time = this->timer_.ticks();
    this->last_delta_ = 0;

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION,
                    "SpriteBatchAction::BEGIN at", delta_time );

    if( this->sprite_ != nullptr ) {
      NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION, "[SpriteBatchAction]"
                      "[num_frames]:", this->sprite_->frames() );
    }
  }
}

void SpriteBatchAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom
