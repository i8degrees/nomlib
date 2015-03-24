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
#include "nomlib/actions/ScaleByAction.hpp"

// Private headers
#include "nomlib/core/helpers.hpp"
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/graphics/sprite/Sprite.hpp"

namespace nom {

// Static initializations
const char* ScaleByAction::DEBUG_CLASS_NAME = "[ScaleByAction]:";

ScaleByAction::ScaleByAction( const std::shared_ptr<Sprite>& drawable,
                              const Size2f& delta, real32 seconds ) :
  total_displacement_(delta)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_duration(seconds);
  this->elapsed_frames_ = 0.0f;
  this->drawable_ = drawable;
}

ScaleByAction::~ScaleByAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<IActionObject> ScaleByAction::clone() const
{
  return( nom::make_unique<self_type>( self_type(*this) ) );
}

IActionObject::FrameState
ScaleByAction::update(real32 t, const Size2i& b, const Size2f& c, real32 d)
{
  real32 delta_time = t;
  const real32 duration = d;

  // initial starting values
  const real32 b1 = b.w;
  const real32 b2 = b.h;

  // total change over time
  real32 c1 = c.w;
  real32 c2 = c.h;

  Size2f displacement(Size2f::zero);
  Size2i displacement_as_integer(Size2i::zero);

  // Clamp delta values that go beyond maximal duration
  if( delta_time > (duration / this->speed() ) ) {
    delta_time = duration / this->speed();
  }

  // Account for the translating of negative X and Y scale factors
  if( (b1 * c1 - b1) > 0 ) {
    c1 = (b1 * c1) - b1;
  } else {
    // resulting value is a positive number
    c1 = -( (b1 / c1) ) - b1;
  }

  if( (b2 * c2 - b2) > 0 ) {
    c2 = (b2 * c2) - b2;
  } else {
    // resulting value is a positive number
    c2 = -( (b2 / c2) ) - b2;
  }

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  NOM_ASSERT(this->timing_curve() != nullptr);

  displacement.w =
    this->timing_curve().operator()(frame_time, b1, c1, duration);

  displacement.h =
    this->timing_curve().operator()(frame_time, b2, c2, duration);

  if( this->drawable_ != nullptr ) {

    ++this->elapsed_frames_;

    // Convert floating-point value to integer; it is critical that we round
    // the values so that values like 254.999984741 are represented as 255.00
    displacement_as_integer.w =
      nom::round_float<int>(displacement.w);
    displacement_as_integer.h =
      nom::round_float<int>(displacement.h);

    displacement_as_integer.w = abs(displacement_as_integer.w);
    displacement_as_integer.h = abs(displacement_as_integer.h);

    this->drawable_->set_size(displacement_as_integer);
    this->size_ = displacement_as_integer;

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "delta_time:", delta_time,
                    "frame_time:", frame_time,
                    "[elapsed frames]:", this->elapsed_frames_ );

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION,
                    DEBUG_CLASS_NAME, "size (input):", this->drawable_->size(),
                    "displacement (output):", displacement_as_integer );
  }

  if( delta_time < (duration / this->speed() ) ) {
    this->set_status(FrameState::PLAYING);
  } else {
    this->last_frame(delta_time);
    this->set_status(FrameState::COMPLETED);
  }

  return this->status();
}

IActionObject::FrameState ScaleByAction::next_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  this->first_frame(delta_time);

  return this->update(  delta_time, this->initial_size_,
                        this->total_displacement_, this->duration() );
}

IActionObject::FrameState ScaleByAction::prev_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  this->first_frame(delta_time);

  Size2f delta(Size2f::zero);
  delta.w = -(this->total_displacement_.w);
  delta.h = -(this->total_displacement_.h);

  return this->update(delta_time, this->initial_size_, delta, this->duration() );
}

void ScaleByAction::pause(real32 delta_time)
{
  this->timer_.pause();
}

void ScaleByAction::resume(real32 delta_time)
{
  this->timer_.unpause();
}

void ScaleByAction::rewind(real32 delta_time)
{
  this->elapsed_frames_ = 0.0f;
  this->timer_.stop();
  this->set_status(FrameState::PLAYING);

  if( this->drawable_ != nullptr ) {
    this->drawable_->set_size(this->initial_size_);
  }
}

void ScaleByAction::release()
{
  if( this->drawable_ != nullptr ) {
    this->drawable_->release_texture();
  }

  this->drawable_.reset();
}

// Private scope

void ScaleByAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    this->timer_.start();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "BEGIN at", delta_time );

    if( this->drawable_ != nullptr ) {
      this->initial_size_ = this->drawable_->size();
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, "initial_size:",
                    this->initial_size_ );
  }
}

void ScaleByAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom
