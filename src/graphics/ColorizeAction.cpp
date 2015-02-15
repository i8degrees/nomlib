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
#include "nomlib/graphics/ColorizeAction.hpp"

// Private headers
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/graphics/sprite/Sprite.hpp"

namespace nom {

ColorizeAction::ColorizeAction( const std::shared_ptr<Sprite>& action,
                                const Color4i& delta, BlendMode mode,
                                real32 duration ) :
  total_displacement_(delta)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ANIMATION, NOM_LOG_PRIORITY_VERBOSE);

  this->set_duration(duration);
  this->curr_frame_ = 0.0f;
  this->blend_mode_ = mode;
  this->drawable_ = action;
}

ColorizeAction::~ColorizeAction()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ANIMATION, NOM_LOG_PRIORITY_VERBOSE);
}

std::unique_ptr<ColorizeAction::derived_type> ColorizeAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState
ColorizeAction::update(real32 t, const Color4i& b, const Color4i& c, real32 d)
{
  real32 delta_time = t;

  // Total duration
  const real32 duration = d;

  // Initial starting value
  const Color4i initial_color(b);

  // Total color blend over time
  const Color4i total_displacement(c);

  // total change in value
  Color4f b1 = initial_color;

  // total change in value
  Color4f c1 = total_displacement;

  // The current computed color
  Color4f displacement(Color4f::Black);

  // Clamp delta values that go beyond the time duration bounds; this adds
  // stability to variable time steps
  if( delta_time > (duration / this->speed() ) ) {
    delta_time = duration / this->speed();
  }

  // Clamp to valid color values
  if( total_displacement > Color4i::White ) {
    c1 = Color4i::White;
  } else if( total_displacement < Color4i::Black ) {
    c1 = Color4i::Black;
  }

  NOM_ASSERT(this->timing_mode() != nullptr);

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  displacement.r =
    this->timing_mode().operator()(frame_time, b1.r, c1.r, duration);
  displacement.g =
    this->timing_mode().operator()(frame_time, b1.g, c1.g, duration);
  displacement.b =
    this->timing_mode().operator()(frame_time, b1.b, c1.b, duration);

  // Clamp to valid color values
  if( displacement.r > 255.0f ) {
    displacement.r = 255.0f;
  } else if( displacement.r < 0.0f ) {
    displacement.r = 0.0f;
  }

  if( displacement.g > 255.0f ) {
    displacement.g = 255.0f;
  } else if( displacement.g < 0.0f ) {
    displacement.g = 0.0f;
  }

  if( displacement.b > 255.0f ) {
    displacement.b = 255.0f;
  } else if( displacement.b < 0.0f ) {
    displacement.b = 0.0f;
  }

  // Update our internal elapsed frames counter (diagnostics)
  ++this->curr_frame_;

  if( this->drawable_ != nullptr ) {

    // State diagnostics
    Color4i drawable_color = this->drawable_->color();

    // Convert floating-point value to integer; it is critical that we round
    // the values so that values like 254.999984741 are represented as 255.00
    Color4i color_as_integer(Color4i::Black);

    color_as_integer.r =
      nom::round_float<int16>(displacement.r);
    color_as_integer.g =
      nom::round_float<int16>(displacement.g);
    color_as_integer.b =
      nom::round_float<int16>(displacement.b);

    this->drawable_->set_color(color_as_integer);

    // Diagnostics
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION, "[ColorizeAction]",
                    "delta_time:", delta_time, "frame_time:", frame_time,
                    "[elapsed frames]:", this->curr_frame_ );
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION, "[ColorizeAction]",
                    "color (input):", drawable_color,
                    "color (output):", color_as_integer );

    // Extended diagnostics
    NOM_LOG_VERBOSE(  NOM_LOG_CATEGORY_ANIMATION, "[ColorizeAction]",
                      "[b]:", b1, "[c1]:", c1 );
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

IActionObject::FrameState ColorizeAction::next_frame(real32 delta_time)
{
  // Initialize timer and initial position
  this->first_frame(delta_time);

  delta_time = this->timer_.ticks();

  return this->update(  delta_time, this->initial_color_,
                        this->total_displacement_, this->duration() );
}

IActionObject::FrameState ColorizeAction::prev_frame(real32 delta_time)
{
  // Initialize timer and initial position
  this->first_frame(delta_time);

  delta_time = this->timer_.ticks();

  Color4i delta;
  delta.r = -(this->total_displacement_.r);
  delta.g = -(this->total_displacement_.g);
  delta.b = -(this->total_displacement_.b);
  delta.a = -(this->total_displacement_.a);

  return this->update(  delta_time, this->initial_color_,
                        delta, this->duration() );
}

void ColorizeAction::pause(real32 delta_time)
{
  this->timer_.pause();
}

void ColorizeAction::resume(real32 delta_time)
{
  this->timer_.unpause();
}

void ColorizeAction::rewind(real32 delta_time)
{
  // Reset elapsed frame (diagnostics)
  this->curr_frame_ = 0.0f;

  // Reset frame timing
  this->timer_.stop();

  // Reset starting blending color
  if( this->drawable_ != nullptr &&
      this->initial_color_ != Color4i::null )
  {
    this->drawable_->set_color(this->initial_color_);
    this->drawable_->set_color_blend_mode(this->blend_mode_);
  }
}

void ColorizeAction::release()
{
  this->drawable_.reset();
}

// Private scope

void ColorizeAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    // Start frame timing
    this->timer_.start();
    delta_time = this->timer_.ticks();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION,
                    "ColorizeAction::BEGIN at", delta_time );

    // This is also necessary for reversing the animation, repeating it, etc.
    if( this->drawable_ != nullptr ) {

      Color4i color = this->drawable_->color();
      if( color != Color4i::null ) {
        this->initial_color_ = this->drawable_->color();
        this->drawable_->set_color_blend_mode(this->blend_mode_);
      } else {
        NOM_LOG_WARN( NOM_LOG_CATEGORY_APPLICATION,
                      "Failed to set the color blend mode of the action.");
      }
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION, "[ColorizeAction]",
                    "[initial_color]:", this->initial_color_,
                    "[blend_mode]:", this->blend_mode_ );
  }
}

void ColorizeAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom
