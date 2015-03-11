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
#include "nomlib/actions/AnimateTexturesAction.hpp"

// Private headers
#include "nomlib/core/helpers.hpp"
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/sprite/Sprite.hpp"

namespace nom {

enum FrameStateDirection
{
  NEXT_FRAME,
  PREV_FRAME
};

// Static initializations
const char* AnimateTexturesAction::DEBUG_CLASS_NAME =
  "[AnimateTexturesAction]:";

void AnimateTexturesAction::
initialize(const texture_frames& textures, real32 frame_interval_seconds)
{

  this->elapsed_frames_ = 0.0f;
  this->initial_frame_ = 0;
  this->frame_interval_ = frame_interval_seconds;
  this->last_delta_ = 0.0f;

  this->frames_ = textures;
  NOM_ASSERT(this->frames_.size() > 0);
  this->total_displacement_ = this->frames_.size();

  real32 action_duration_seconds =
    (this->frame_interval_ * this->total_displacement_);
  this->set_duration(action_duration_seconds);
}

AnimateTexturesAction::
AnimateTexturesAction(  const std::shared_ptr<Sprite>& drawable,
                        const texture_frames& textures,
                        real32 frame_interval_seconds )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->drawable_ = drawable;
  this->initialize(textures, frame_interval_seconds);
}

AnimateTexturesAction::~AnimateTexturesAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<IActionObject> AnimateTexturesAction::clone() const
{
  return( nom::make_unique<self_type>( self_type(*this) ) );
}

IActionObject::FrameState
AnimateTexturesAction::update(real32 t, real32 b, real32 c, real32 d)
{
  real32 frame_interval =
    this->frame_interval_ / this->speed();

  // The current frame to scale
  real32 delta_time = t;

  // Total duration of the action
  const real32 duration = d;

  // The computed texture frame to show next
  real32 displacement(0.0f);

  // Clamp delta values that go beyond maximal duration
  if( delta_time > (duration / this->speed() ) ) {
    delta_time = duration / this->speed();
  }

  FrameStateDirection direction;
  if( c >= 0 ) {
    direction = NEXT_FRAME;
  } else {
    direction = PREV_FRAME;
  }

  NOM_ASSERT(this->timing_curve() != nullptr);

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  displacement =
    this->timing_curve().operator()(frame_time, b, c, duration);
  NOM_ASSERT(displacement <= this->total_displacement_);
  NOM_ASSERT(displacement >= this->initial_frame_);

  this->elapsed_frames_ = displacement;

  if( delta_time >= (this->last_delta_ + frame_interval) &&
      delta_time < ( duration / this->speed() ) )
  {
    this->last_delta_ = delta_time;

    frame_iterator curr_frame;
    if( direction == NEXT_FRAME ) {
      ++this->next_frame_;
      if( this->next_frame_ == (this->frames_.end() - 1) ) {
        this->next_frame_ =
          (this->frames_.begin() + this->initial_frame_);
      }

      curr_frame = this->next_frame_;
    } else if( direction == PREV_FRAME ) {

      --this->last_frame_;
      if( this->last_frame_ == this->frames_.begin() ) {
        this->last_frame_ = (this->frames_.end() - 1);
      }

      curr_frame = this->last_frame_;
    }

    NOM_ASSERT( curr_frame != this->frames_.end() );
    if( curr_frame != this->frames_.end() ) {
      auto tex = curr_frame;

      if( this->drawable_ != nullptr && (*tex)->valid() == true ) {
        this->drawable_->set_texture( (*tex) );
      }
    }

    uint32 displacement_as_integer =
      nom::round_float_down<uint32>(displacement);
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "delta_time:", delta_time, "frame_time:", frame_time,
                    "[elapsed frames]:", this->elapsed_frames_,
                    "displacement (output):", displacement_as_integer );

  }

  if( delta_time < (duration / this->speed() ) ) {

    this->status_ = FrameState::PLAYING;
    return this->status_;
  } else {
    this->last_frame(delta_time);

    this->status_ = FrameState::COMPLETED;
    return this->status_;
  }
}

IActionObject::FrameState AnimateTexturesAction::next_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  this->first_frame(delta_time);

  return( this->update( delta_time, this->initial_frame_,
          this->total_displacement_, this->duration() ) );
}

IActionObject::FrameState AnimateTexturesAction::prev_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  this->first_frame(delta_time);

  return( this->update( delta_time, this->total_displacement_,
          -(this->total_displacement_), this->duration() ) );
}

void AnimateTexturesAction::pause(real32 delta_time)
{
  this->timer_.pause();
}

void AnimateTexturesAction::resume(real32 delta_time)
{
  this->timer_.unpause();
}

void AnimateTexturesAction::rewind(real32 delta_time)
{
  // Reset frame cycle back to initial value
  this->elapsed_frames_ = 0.0f;
  this->initial_frame_ = 0;
  this->next_frame_ = (this->frames_.begin() + this->initial_frame_);
  this->last_frame_ = (this->frames_.end() - 1);
  this->last_delta_ = 0.0f;

  // Reset frame timing
  this->timer_.stop();

  this->status_ = FrameState::PLAYING;

  auto curr_frame = this->next_frame_;
  NOM_ASSERT( curr_frame != this->frames_.end() );
  if( this->drawable_ != nullptr &&
      curr_frame != this->frames_.end() )
  {
    auto tex = curr_frame;
    if( this->drawable_ != nullptr && (*tex)->valid() == true ) {
      this->drawable_->set_texture( (*tex) );
    }
  }
}

void AnimateTexturesAction::release()
{
  if( this->drawable_ != nullptr ) {
    this->drawable_->release_texture();
  }

  this->drawable_.reset();
}

// Private scope

void AnimateTexturesAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    // Start frame timing
    this->elapsed_frames_ = 0.0f;
    this->initial_frame_ = 0;
    this->last_delta_ = 0.0f;
    this->next_frame_ = (this->frames_.begin() + this->initial_frame_);
    this->last_frame_ = (this->frames_.end() - 1);
    this->timer_.start();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "BEGIN at", delta_time );

    auto curr_frame = this->next_frame_;
    NOM_ASSERT( curr_frame != this->frames_.end() );
    if( curr_frame != this->frames_.end() ) {

      auto tex = curr_frame;
      // NOTE: Set the texture of the sprite immediately, so we do not have a
      // momentary gap in rendering
      if( this->drawable_ != nullptr && (*tex)->valid() == true ) {
        this->drawable_->set_texture( (*tex) );
      }
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION,
                    "[initial_frame]:", this->initial_frame_,
                    "[num_frames]:", this->total_displacement_,
                    "[frame_interval]:", this->frame_interval_ );
  }
}

void AnimateTexturesAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom
