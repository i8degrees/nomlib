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
#include "nomlib/graphics/FadeAudioByAction.hpp"

// Private headers
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/audio/ISoundSource.hpp"

namespace nom {

FadeAudioByAction::FadeAudioByAction( const std::shared_ptr<ISoundSource>& obj,
                                      real32 delta, real32 duration ) :
  total_displacement_(delta)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->duration_ = duration * 1000; // milliseconds
  this->curr_frame_ = 0.0f;
  this->volume_ = 0;
  this->initial_volume_ = 0;
  this->drawable_ = obj;
}

FadeAudioByAction::~FadeAudioByAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<FadeAudioByAction::derived_type>
FadeAudioByAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState
FadeAudioByAction::update(real32 t, uint8 b, int16 c, real32 d)
{
  // The current frame to alpha blend
  real32 delta_time = t;

  // Total duration of the animation's alpha blending
  const real32 duration = d;

  // Initial starting value; this is the first frame to alpha blend from
  const uint8 initial_volume(b);

  // Total displacement over time
  const int16 total_displacement(c);

  // The computed alpha blending value of the frame
  real32 displacement = 0.0f;

  // Clamp delta values that go beyond the time duration bounds; this adds
  // stability to variable time steps
  if( delta_time > (duration / this->speed) ) {
    delta_time = duration / this->speed;
  }

  // initial starting value
  const real32 b1 = initial_volume;

  // total change in value (applied over time)
  real32 c1 = 0.0f;

  // Account for the blending of negative alphas
  if( total_displacement >= b1 ) {
    c1 = total_displacement - b1;
  } else {
    c1 = total_displacement;
  }

  NOM_ASSERT(this->timing_mode != nullptr);

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed;

  displacement =
    this->timing_mode.operator()(frame_time, b1, c1, duration);

  // Update our internal elapsed frames counter (diagnostics
  ++this->curr_frame_;

  if( this->drawable_ != nullptr ) {

    // Convert floating-point value to integer; it is critical that we round
    // the values so that values like 254.999984741 are represented as 255.00
    this->drawable_->setVolume(displacement);

    // Record state for the unit tests
    this->volume_ = displacement;

    // Diagnostics
    NOM_LOG_INFO( NOM_LOG_CATEGORY_ACTION, "delta_time:", delta_time,
                  "frame_time:", frame_time, "[elapsed frames]:", this->curr_frame_ );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_ACTION,
                  "volume (input):", this->drawable_->getVolume(),
                  "displacement (output):", displacement );
  }

  // Continue playing the animation only when we are inside our frame duration
  // bounds; this adds stability to variable time steps
  if( delta_time < (duration / this->speed) ) {
    this->status_ = FrameState::PLAY_NEXT_FRAME;
    return this->status_;
  } else {

    // Diagnostics
    this->last_frame(delta_time);

    this->status_ = FrameState::DONE;
    return this->status_;
  }
}

IActionObject::FrameState FadeAudioByAction::next_frame(real32 delta_time)
{
  // Initialize timer and initial alpha
  this->first_frame(delta_time);

  return this->update(  delta_time, this->initial_volume_,
                        this->total_displacement_, this->duration_ );
}

IActionObject::FrameState FadeAudioByAction::prev_frame(real32 delta_time)
{
  // Initialize timer and initial alpha
  this->first_frame(delta_time);

  return this->update(  delta_time, this->initial_volume_,
                        -(this->total_displacement_), this->duration_ );
}

void FadeAudioByAction::pause(real32 delta_time)
{
  // Stub
}

void FadeAudioByAction::resume(real32 delta_time)
{
  // Stub
}

// Has not been tested
void FadeAudioByAction::rewind(real32 delta_time)
{
  // Reset the starting frame
  this->curr_frame_ = 0.0f;

  // Reset last recorded alpha state
  this->volume_ = 0;

  // Reset frame timing
  this->timer_.stop();

  // Reset the initial alpha blending value; this is also necessary for
  // reversing the animation, repeating it, etc.
  if( this->drawable_ != nullptr ) {
    this->drawable_->setVolume(this->initial_volume_);
  }
}

void FadeAudioByAction::release()
{
  this->drawable_.reset();
}

// Private scope

void FadeAudioByAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    this->timer_.start();

    // Diagnostics
    NOM_LOG_INFO( NOM_LOG_CATEGORY_ACTION,
                  "FadeAudioByAction::BEGIN at", this->timer_.ticks() );

    // Initialize the initial alpha blending value; this is also necessary for
    // reversing the animation, repeating it, etc.
    if( this->drawable_ != nullptr ) {
      this->initial_volume_ = this->drawable_->getVolume();

      // Diagnostics
      NOM_LOG_INFO( NOM_LOG_CATEGORY_ACTION,
                    "initial_alpha:", NOM_SCAST(int, this->initial_volume_) );
    }
  }
}

void FadeAudioByAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom
