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
#include "nomlib/graphics/ScaleByAction.hpp"

// Private headers
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/graphics/sprite/Sprite.hpp"

namespace nom {

ScaleByAction::ScaleByAction( const std::shared_ptr<Sprite>& obj,
                              const Size2f& delta, real32 duration ) :
  total_displacement_(delta)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_duration(duration);
  this->curr_frame_ = 0.0f;
  this->drawable_ = obj;
}

ScaleByAction::~ScaleByAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<ScaleByAction::derived_type> ScaleByAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState
ScaleByAction::update(real32 t, const Size2i& b, const Size2f& c, real32 d)
{
  // The current frame to scale
  real32 delta_time = t;

  // Total duration of the animation's scaling
  const real32 duration = d;

  // Initial starting value; this is the first frame to scale from
  const Size2i initial_size(b);

  // Total displacement over time
  const Size2f total_displacement(c);

  // The computed scale value of the frame
  Size2f displacement(Size2f::zero);

  // Clamp delta values that go beyond the time duration bounds; this adds
  // stability to variable time steps
  if( delta_time > (duration / this->speed() ) ) {
    delta_time = duration / this->speed();
  }

  // initial starting value
  const real32 b1 = initial_size.w;
  const real32 b2 = initial_size.h;

  // total change in value (applied over time)
  real32 c1 = 0.0f;
  real32 c2 = 0.0f;

  // Account for the translating of negative X and Y scale factors
  if( (b1 * total_displacement.w - b1) > 0 ) {
    c1 = (b1 * total_displacement.w) - b1;
  } else {
    // resulting value is a positive number
    c1 = -( (b1 / total_displacement.w) ) - b1;
  }

  if( (b2 * total_displacement.h - b2) > 0 ) {
    c2 = (b2 * total_displacement.h) - b2;
  } else {
    // resulting value is a positive number
    c2 = -( (b2 / total_displacement.h) ) - b2;
  }

  NOM_ASSERT(this->timing_mode() != nullptr);

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  displacement.w =
    this->timing_mode().operator()(frame_time, b1, c1, duration);

  displacement.h =
    this->timing_mode().operator()(frame_time, b2, c2, duration);

  // Update our internal elapsed frames counter (diagnostics)
  ++this->curr_frame_;

  if( this->drawable_ != nullptr ) {

    // State diagnostics logging
    Size2i drawable_size = this->drawable_->size();

    // Convert floating-point value to integer; it is critical that we round
    // the values so that values like 254.999984741 are represented as 255.00
    Size2i displacement_as_integer(Size2i::zero);

    displacement_as_integer.w =
      nom::round_float<int>(displacement.w);
    displacement_as_integer.h =
      nom::round_float<int>(displacement.h);

    // TODO: Explain **why**!!
    displacement_as_integer.w = abs(displacement_as_integer.w);
    displacement_as_integer.h = abs(displacement_as_integer.h);
    this->drawable_->set_size(displacement_as_integer);

    // Record state for the unit tests
    this->size_ = displacement_as_integer;

    // Diagnostics
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION, "[ScaleByAction]",
                    "delta_time:", delta_time,
                    "frame_time:", frame_time,
                    "[elapsed frames]:", this->curr_frame_ );
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION,
                    "ScaleByAction:", "size (input):", drawable_size,
                    "displacement (output):", displacement_as_integer );
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

IActionObject::FrameState ScaleByAction::next_frame(real32 delta_time)
{
  // Initialize timer and initial alpha
  this->first_frame(delta_time);

  delta_time = this->timer_.ticks();

  return this->update(  delta_time, this->initial_size_,
                        this->total_displacement_, this->duration() );
}

IActionObject::FrameState ScaleByAction::prev_frame(real32 delta_time)
{
  // Initialize timer and initial alpha
  this->first_frame(delta_time);

  delta_time = this->timer_.ticks();

  Size2f delta(Size2f::zero);
  delta.w = -(this->total_displacement_.w);
  delta.h = -(this->total_displacement_.h);

  return this->update(delta_time, this->initial_size_, delta, this->duration() );
}

void ScaleByAction::pause(real32 delta_time)
{
  // Stub
  this->timer_.pause();
}

void ScaleByAction::resume(real32 delta_time)
{
  // Stub
  this->timer_.unpause();
}

void ScaleByAction::rewind(real32 delta_time)
{
  // Reset the starting frame
  this->curr_frame_ = 0.0f;

  // Reset frame timing
  this->timer_.stop();

  // Reset the initial size for scaling; this is also necessary for reversing
  // the animation, repeating it, etc.
  if( this->drawable_ != nullptr && this->initial_size_ != Size2i::null ) {
    this->drawable_->set_size(this->initial_size_);
  }
}

void ScaleByAction::release()
{
  this->drawable_.reset();
}

// Private scope

void ScaleByAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    // Start frame timing
    this->timer_.start();
    delta_time = this->timer_.ticks();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION,
                    "ScaleByAction::BEGIN at", delta_time );

    // Initialize the initial size scaling value; this is also necessary for
    // reversing the animation, repeating it, etc.
    if( this->drawable_ != nullptr ) {
      this->initial_size_ = this->drawable_->size();
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION,
                    "[ScaleByAction]", "initial_size:", this->initial_size_ );
  }
}

void ScaleByAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom
