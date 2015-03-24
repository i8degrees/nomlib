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
#include "nomlib/actions/MoveByAction.hpp"

// Private headers
#include "nomlib/core/helpers.hpp"
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/graphics/sprite/Sprite.hpp"

namespace nom {

// Static initializations
const char* MoveByAction::DEBUG_CLASS_NAME = "[MoveByAction]:";

MoveByAction::MoveByAction( const std::shared_ptr<Sprite>& drawable,
                            const Point2i& delta, real32 seconds ) :
  total_displacement_(delta)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION, NOM_LOG_PRIORITY_VERBOSE);

  this->set_duration(seconds);
  this->elapsed_frames_ = 0.0f;
  this->drawable_ = drawable;
}

MoveByAction::~MoveByAction()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION, NOM_LOG_PRIORITY_VERBOSE);
}

std::unique_ptr<IActionObject> MoveByAction::clone() const
{
  return( nom::make_unique<self_type>( self_type(*this) ) );
}

IActionObject::FrameState
MoveByAction::update(real32 t, const Point2i& b, const Point2i& c, real32 d)
{
  real32 delta_time = t;
  const real32 duration = d;

  // Initial starting values
  const real32 b1 = b.x;
  const real32 b2 = b.y;

  // Total change over time
  real32 c1 = c.x;
  real32 c2 = c.y;

  Point2f displacement(Point2f::zero);
  Point2i displacement_as_integer(Point2i::zero);

  // Clamp delta values that go beyond maximal duration
  if( delta_time > (duration / this->speed() ) ) {
    delta_time = duration / this->speed();
  }

  // Account for the translating of negative X and Y displacements
  if( c1 >= b1 ) {
    c1 = c1 - b1;
  } else {
    // Use existing value
  }

  if( c2 >= b2 ) {
    c2 = c2 - b2;
  } else {
    // Use existing value
  }

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  NOM_ASSERT(this->timing_curve() != nullptr);

  displacement.x =
    this->timing_curve().operator()(frame_time, b1, c1, duration);
  displacement.y =
    this->timing_curve().operator()(frame_time, b2, c2, duration);

  if( this->drawable_ != nullptr ) {

    ++this->elapsed_frames_;

    // Convert floating-point value to integer; it is critical that we round
    // the values so that values like 254.999984741 are represented as 255.00
    displacement_as_integer.x =
      nom::round_float<int>(displacement.x);
    displacement_as_integer.y =
      nom::round_float<int>(displacement.y);

    this->drawable_->set_position(displacement_as_integer);

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "delta_time:", delta_time,
                    "frame_time:", frame_time,
                    "[elapsed frames]:", this->elapsed_frames_ );

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION,
                    "position (input):", this->drawable_->position(),
                    "displacement (output):", displacement_as_integer );

    NOM_LOG_VERBOSE(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                      "[b1]:", b1, "[c1]:", c1,
                      "[b2]:", b2, "[c2]:", c2 );
  }

  if( delta_time < (duration / this->speed() ) ) {
    this->set_status(FrameState::PLAYING);
  } else {
    this->last_frame(delta_time);
    this->set_status(FrameState::COMPLETED);
  }

  return this->status();
}

IActionObject::FrameState MoveByAction::next_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  this->first_frame(delta_time);

  return this->update(  delta_time, this->initial_position_,
                        this->total_displacement_, this->duration() );
}

IActionObject::FrameState MoveByAction::prev_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  this->first_frame(delta_time);

  return this->update(  delta_time, this->initial_position_,
                        -(this->total_displacement_), this->duration() );
}

void MoveByAction::pause(real32 delta_time)
{
  this->timer_.pause();
}

void MoveByAction::resume(real32 delta_time)
{
  this->timer_.unpause();
}

void MoveByAction::rewind(real32 delta_time)
{
  this->elapsed_frames_ = 0.0f;
  this->timer_.stop();
  this->set_status(FrameState::PLAYING);

  if( this->drawable_ != nullptr ) {
    this->drawable_->set_position(this->initial_position_);
  }
}

void MoveByAction::release()
{
  if( this->drawable_ != nullptr ) {
    this->drawable_->release_texture();
  }

  this->drawable_.reset();
}

// Private scope

void MoveByAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    this->timer_.start();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "BEGIN at", delta_time );

    if( this->drawable_ != nullptr ) {
      this->initial_position_ = this->drawable_->position();
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, "initial_position:",
                    this->initial_position_ );
  }
}

void MoveByAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom
