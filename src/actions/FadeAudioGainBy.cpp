/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015, 2016 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/actions/FadeAudioGainBy.hpp"

// Private headers
#include "nomlib/core/unique_ptr.hpp"
#include "nomlib/math/math_helpers.hpp"
#include "nomlib/audio/audio_defs.hpp"

// Forward declarations
#include "nomlib/audio/SoundBuffer.hpp"
#include "nomlib/audio/SoundFile.hpp"
#include "nomlib/audio/AL/SoundSource.hpp"

namespace nom {

// Static initializations
const char* FadeAudioGainBy::DEBUG_CLASS_NAME = "[FadeAudioGainBy]:";

FadeAudioGainBy::
FadeAudioGainBy(audio::IOAudioEngine* dev, const char* filename, real32 delta,
                real32 duration)
  : total_displacement_(delta)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION,
                     nom::NOM_LOG_PRIORITY_VERBOSE);

  this->impl_ = dev;
  this->elapsed_frames_ = 0.0f;
  this->audible_ = audio::create_buffer(filename, this->impl_);
  // TODO(jeff): Validity check..?

  this->set_duration(duration);
  this->initial_volume_ = audio::volume(this->audible_, dev);
}

FadeAudioGainBy::
FadeAudioGainBy(audio::IOAudioEngine* dev, audio::SoundBuffer* buffer,
                real32 delta, real32 duration)
  : total_displacement_(delta)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION,
                     nom::NOM_LOG_PRIORITY_VERBOSE);

  this->impl_ = dev;
  this->elapsed_frames_ = 0.0f;
  this->audible_ = buffer;

  this->set_duration(duration);
  this->initial_volume_ = audio::volume(buffer, dev);
}

FadeAudioGainBy::~FadeAudioGainBy()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION,
                     nom::NOM_LOG_PRIORITY_VERBOSE);
}

std::unique_ptr<IActionObject> FadeAudioGainBy::clone() const
{
  return( nom::make_unique<self_type>( self_type(*this) ) );
}

IActionObject::FrameState
FadeAudioGainBy::update(real32 t, uint8 b, int16 c, real32 d)
{
  real32 delta_time = t;
  const real32 duration = d;
  auto status = this->status();
  const auto speed = this->speed();
  const auto timing_mode = this->timing_curve();

  real32 b1 = b;
  real32 c1 = c;

  // The current displacement value of this frame
  real32 gain = 0.0f;
  real32 displacement = 0.0f;

  // Clamp values to stay within bounds of the initial value
  if(c1 > b1) {
    c1 = c1 - b1;
  } else {
    c1 = -b1;
  }

  // Clamp delta values that go beyond the time duration bounds; this adds
  // stability to variable time steps
  if(delta_time > (duration / speed)) {
    delta_time = duration / speed;
  }

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * speed;

  NOM_ASSERT(timing_mode != nullptr);
  if(timing_mode != nullptr) {
    gain =
      timing_mode.operator()(frame_time, b1, c1, duration);
  }

  // Update our internal elapsed frames counter (diagnostics
  ++this->elapsed_frames_;

  if(this->audible_ != nullptr) {
    // FIXME(jeff): The external audio API (us) for gain accepts values between
    // 0..100 whereas the internal audio API (OpenAL) expects a value between
    // 0..1
    displacement =
      nom::absolute_real32((gain / 1.0f) * 0.01f);
    displacement *= 100.0f;
    audio::set_volume(this->audible_, this->impl_, displacement);

    // Diagnostics
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "delta_time:", delta_time, "frame_time:", frame_time,
                    "[elapsed frames]:", this->elapsed_frames_ );

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION,
                    "volume this frame:", gain,
                    "output gain:", displacement);

    NOM_ASSERT(displacement <= audio::MAX_VOLUME);
    NOM_ASSERT(displacement >= audio::MIN_VOLUME);
  }

  // Continue playing the animation only when we are inside our frame duration
  // bounds; this adds stability to variable time steps
  if(delta_time < (duration / speed)) {
    this->set_status(FrameState::PLAYING);
    status = this->status();
  } else {
    this->last_frame(delta_time);

    this->set_status(FrameState::COMPLETED);
    status = this->status();
  }

  return status;
}

IActionObject::FrameState FadeAudioGainBy::next_frame(real32 delta_time)
{
  delta_time = this->timer_.to_seconds();

  this->first_frame(delta_time);

  return this->update(delta_time, this->initial_volume_,
                      this->total_displacement_, this->duration());
}

IActionObject::FrameState FadeAudioGainBy::prev_frame(real32 delta_time)
{
  delta_time = this->timer_.to_seconds();

  this->first_frame(delta_time);

  return this->update(delta_time, this->initial_volume_,
                      -(this->total_displacement_), this->duration());
}

void FadeAudioGainBy::pause(real32 delta_time)
{
  // audio::pause(this->audible_, this->impl_);
  this->timer_.pause();
}

void FadeAudioGainBy::resume(real32 delta_time)
{
  // audio::resume(this->audible_, this->impl_);
  this->timer_.unpause();
}

void FadeAudioGainBy::rewind(real32 delta_time)
{
  // ...Reset the animation...
  this->elapsed_frames_ = 0.0f;
  this->timer_.stop();
  this->set_status(FrameState::PLAYING);

  if(this->audible_ != nullptr) {
    audio::set_volume(this->audible_, this->impl_, this->initial_volume_);
  }

  // audio::stop(this->audible_, this->impl_);
}

void FadeAudioGainBy::release()
{
  audio::free_buffer(this->audible_, this->impl_);
  this->audible_ = nullptr;
}

// Private scope

void FadeAudioGainBy::first_frame(real32 delta_time)
{
  if(this->timer_.started() == false) {
    this->timer_.start();

    NOM_LOG_INFO(NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                 "BEGIN at", delta_time);

    // ...Set the animation up...
    if(this->audible_ != nullptr) {
      this->initial_volume_ = audio::volume(this->audible_, this->impl_);

      // Diagnostics
      NOM_LOG_INFO(NOM_LOG_CATEGORY_ACTION,
                   "initial_volume:", this->initial_volume_);
    }

    // audio::play(this->audible_, this->impl_);
  }
}

void FadeAudioGainBy::last_frame(real32 delta_time)
{
  // audio::stop(this->audible_, this->impl_);
  this->timer_.stop();
}

} // namespace nom
