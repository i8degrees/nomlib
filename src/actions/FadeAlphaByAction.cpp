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
#include "nomlib/actions/FadeAlphaByAction.hpp"

// Private headers
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/graphics/sprite/Sprite.hpp"

namespace nom {

FadeAlphaByAction::FadeAlphaByAction( const std::shared_ptr<Sprite>& obj,
                                      int16 delta, real32 duration ) :
  total_displacement_(delta)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_duration(duration);
  this->curr_frame_ = 0.0f;
  this->alpha_ = 0;
  this->initial_alpha_ = 0;
  this->drawable_ = obj;
}

FadeAlphaByAction::~FadeAlphaByAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<FadeAlphaByAction::derived_type>
FadeAlphaByAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState
FadeAlphaByAction::update(real32 t, uint8 b, int16 c, real32 d)
{
  // The current frame to alpha blend
  real32 delta_time = t;

  // Total duration of the animation's alpha blending
  const real32 duration = d;

  // Initial starting value; this is the first frame to alpha blend from
  const uint8 initial_alpha(b);

  // Total displacement over time
  const int16 total_displacement(c);

  // The computed alpha blending value of the frame
  real32 displacement = 0.0f;

  // Clamp delta values that go beyond the time duration bounds; this adds
  // stability to variable time steps
  if( delta_time > (duration / this->speed() ) ) {
    delta_time = duration / this->speed();
  }

  // initial starting value
  const real32 b1 = initial_alpha;

  // total change in value (applied over time)
  real32 c1 = 0.0f;

  // Account for the blending of negative alphas
  if( total_displacement >= b1 ) {
    c1 = total_displacement - b1;
  } else {
    c1 = total_displacement;
  }

  NOM_ASSERT(this->timing_curve() != nullptr);

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  displacement =
    this->timing_curve().operator()(frame_time, b1, c1, duration);

  // Update our internal elapsed frames counter (diagnostics
  ++this->curr_frame_;

  int16 displacement_as_integer = 0;

  if( this->drawable_ != nullptr ) {

    // Convert floating-point value to integer; it is critical that we round
    // the values so that values like 254.999984741 are represented as 255.00
    displacement_as_integer =
      nom::round_float<int>(displacement);
    // TODO: Explain **why** abs
    this->drawable_->set_alpha( abs(displacement_as_integer) );

    // Record state for the unit tests
    this->alpha_ = (uint8)abs(displacement_as_integer);

    // Diagnostics
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION,
                    "[FadeAlphaByAction]", "delta_time:", delta_time,
                    "frame_time:", frame_time,
                    "[elapsed frames]:", this->curr_frame_ );
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION,
                    "[FadeAlphaByAction]", "alpha (input):",
                    NOM_SCAST(int, this->drawable_->alpha() ),
                    "displacement (output):",
                    NOM_SCAST(int, displacement_as_integer) );
  }

  // Continue playing the animation only when we are inside our frame duration
  // bounds; this adds stability to variable time steps
  if( delta_time < (duration / this->speed() ) ) {
    this->status_ = FrameState::PLAY_NEXT_FRAME;
    return this->status_;
  } else {

    // Diagnostics
    this->last_frame(delta_time);

    this->status_ = FrameState::DONE;
    return this->status_;
  }
}

IActionObject::FrameState FadeAlphaByAction::next_frame(real32 delta_time)
{
  // Initialize timer and initial alpha
  this->first_frame(delta_time);

  delta_time = this->timer_.ticks();

  return this->update(  delta_time, this->initial_alpha_,
                        this->total_displacement_, this->duration() );
}

IActionObject::FrameState FadeAlphaByAction::prev_frame(real32 delta_time)
{
  // Initialize timer and initial alpha
  this->first_frame(delta_time);

  delta_time = this->timer_.ticks();

  return this->update(  delta_time, this->initial_alpha_,
                        -(this->total_displacement_), this->duration() );
}

void FadeAlphaByAction::pause(real32 delta_time)
{
  // Stub
  this->timer_.pause();
}

void FadeAlphaByAction::resume(real32 delta_time)
{
  // Stub
  this->timer_.unpause();
}

// Has not been tested
void FadeAlphaByAction::rewind(real32 delta_time)
{
  // Reset the starting frame
  this->curr_frame_ = 0.0f;

  // Reset last recorded alpha state
  this->alpha_ = 0;

  // Reset frame timing
  this->timer_.stop();

  // Reset the initial alpha blending value; this is also necessary for
  // reversing the animation, repeating it, etc.
  if( this->drawable_ != nullptr ) {
    this->drawable_->set_alpha(this->initial_alpha_);
  }
}

void FadeAlphaByAction::release()
{
  this->drawable_.reset();
}

// Private scope

void FadeAlphaByAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    // Start frame timing
    this->timer_.start();
    delta_time = this->timer_.ticks();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION,
                    "FadeAlphaByAction::BEGIN at", delta_time );

    // Initialize the initial alpha blending value; this is also necessary for
    // reversing the animation, repeating it, etc.
    if( this->drawable_ != nullptr ) {
      this->initial_alpha_ = this->drawable_->alpha();
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION,
                    "[FadeAlphaByAction]", "initial_alpha:",
                    NOM_SCAST(int, this->initial_alpha_) );
  }
}

void FadeAlphaByAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom
