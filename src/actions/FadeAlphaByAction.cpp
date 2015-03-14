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
#include "nomlib/core/helpers.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/graphics/sprite/Sprite.hpp"

namespace nom {

// Static initializations
const char* FadeAlphaByAction::DEBUG_CLASS_NAME = "[FadeAlphaByAction]:";

FadeAlphaByAction::FadeAlphaByAction( const std::shared_ptr<Sprite>& drawable,
                                      real32 delta, real32 seconds ) :
  total_displacement_(delta)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_duration(seconds);
  this->elapsed_frames_ = 0.0f;
  this->drawable_ = drawable;
}

FadeAlphaByAction::~FadeAlphaByAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<IActionObject> FadeAlphaByAction::clone() const
{
  return( nom::make_unique<self_type>( self_type(*this) ) );
}

IActionObject::FrameState
FadeAlphaByAction::update(real32 t, real32 b, real32 c, real32 d)
{
  real32 delta_time = t;
  const real32 duration = d;

  // initial starting value
  const real32 b1 = b;

  // Total change over time
  real32 c1 = c;

  real32 displacement = 0.0f;
  real32 displacement_as_rgba = 0.0f;

  // Clamp delta values that go beyond maximal duration
  if( delta_time > (duration / this->speed() ) ) {
    delta_time = duration / this->speed();
  }

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  NOM_ASSERT(this->timing_curve() != nullptr);

  displacement =
    this->timing_curve().operator()(frame_time, b1, c1, duration);

  if( this->drawable_ != nullptr ) {

    ++this->elapsed_frames_;

    // Convert the floating-point value to an unsigned 8-bit RGBA value
    displacement_as_rgba =
      nom::absolute_float<real32>( (displacement / 255) * 255);

    this->drawable_->set_alpha(displacement_as_rgba);
    this->alpha_ = displacement_as_rgba;

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "delta_time:", delta_time, "frame_time:", frame_time,
                    "[elapsed frames]:", this->elapsed_frames_ );

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION,
                    "alpha (input):",
                    NOM_SCAST(int, this->drawable_->alpha() ),
                    "displacement (output):", displacement_as_rgba );

    NOM_ASSERT(displacement_as_rgba <= Color4i::ALPHA_OPAQUE);
    NOM_ASSERT(displacement_as_rgba >= Color4i::ALPHA_TRANSPARENT);
  }

  if( delta_time < (duration / this->speed() ) ) {
    this->set_status(FrameState::PLAYING);
  } else {
    this->last_frame(delta_time);
    this->set_status(FrameState::COMPLETED);
  }

  return this->status();
}

IActionObject::FrameState FadeAlphaByAction::next_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  this->first_frame(delta_time);

  return this->update(  delta_time, this->initial_alpha_,
                        this->total_displacement_, this->duration() );
}

IActionObject::FrameState FadeAlphaByAction::prev_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  this->first_frame(delta_time);

  return this->update(  delta_time, this->initial_alpha_,
                        -(this->total_displacement_), this->duration() );
}

void FadeAlphaByAction::pause(real32 delta_time)
{
  this->timer_.pause();
}

void FadeAlphaByAction::resume(real32 delta_time)
{
  this->timer_.unpause();
}

void FadeAlphaByAction::rewind(real32 delta_time)
{
  this->elapsed_frames_ = 0.0f;
  this->alpha_ = 0;
  this->timer_.stop();
  this->set_status(FrameState::PLAYING);

  if( this->drawable_ != nullptr ) {
    this->drawable_->set_alpha(this->initial_alpha_);
  }
}

void FadeAlphaByAction::release()
{
  if( this->drawable_ != nullptr ) {
    this->drawable_->release_texture();
  }

  this->drawable_.reset();
}

// Private scope

void FadeAlphaByAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    this->timer_.start();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "BEGIN at", delta_time );

    if( this->drawable_ != nullptr ) {
      this->initial_alpha_ = this->drawable_->alpha();
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, "initial_alpha:",
                    NOM_SCAST(int, this->initial_alpha_) );
  }
}

void FadeAlphaByAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom
