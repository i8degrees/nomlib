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
#include "nomlib/actions/PlayAudioSource.hpp"

// Private headers
#include "nomlib/core/unique_ptr.hpp"
#include "nomlib/math/math_helpers.hpp"
#include "nomlib/audio/audio_defs.hpp"
#include "nomlib/audio/IOAudioEngine.hpp"

#include <vector>

// Forward declarations
#include "nomlib/audio/libsndfile/SoundFileReader.hpp"
#include "nomlib/audio/SoundBuffer.hpp"
#include "nomlib/audio/AL/SoundSource.hpp"

namespace nom {

// Static initializations
const char* PlayAudioSource::DEBUG_CLASS_NAME = "[PlayAudioSource]:";

PlayAudioSource::
PlayAudioSource(audio::IOAudioEngine* dev, const char* filename)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION,
                     nom::NOM_LOG_PRIORITY_VERBOSE);

  audio::SoundInfo metadata = {};
  this->impl_ = dev;
  this->elapsed_frames_ = 0.0f;

  this->fp_ = new audio::SoundFileReader();
  NOM_ASSERT(this->fp_ != nullptr);
  if(this->fp_ != nullptr) {

    if(this->fp_->open(filename, metadata) == false) {
      return;
    }

    if(this->fp_->valid() == false) {
      return;
    }

    auto samples_per_second = metadata.sample_rate;
    auto num_channels = metadata.channel_count;
    auto channel_format = metadata.channel_format;

    this->audible_.resize(nom::audio::NUM_SOURCES);

    // NOTE(jeff): Create a RAM buffer that can hold one second (1s) of audio
    // samples
    for(auto buffer_idx = 0; buffer_idx != audio::NUM_SOURCES; ++buffer_idx) {
      audio::SoundBuffer* buffer =
        audio::create_buffer_memory(samples_per_second, num_channels, channel_format);
      if(buffer == nullptr) {
        return;
      }

      if(audio::write_info(buffer, metadata) == false) {
        return;
      }

      this->set_duration(buffer->duration);
      buffer->elapsed_seconds = this->timer_.ticks();

      this->audible_.push_back(*buffer);
    }

    // TODO(jeff): Validity check..?
    // if(audio::write_info(this->audible_, metadata) == false) {
      // return;
    // }

    // if(this->audible_ != nullptr) {
        // this->set_duration(&this->audible_[buffer_idx].duration);
    // }

    // this->audible_->elapsed_seconds = this->timer_.ticks();
  }
}

PlayAudioSource::
PlayAudioSource(audio::IOAudioEngine* dev, audio::SoundBuffer* buffer)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION,
                     nom::NOM_LOG_PRIORITY_VERBOSE);

  NOM_ASSERT_INVALID_PATH("TODO");

  this->impl_ = dev;
  this->elapsed_frames_ = 0.0f;
  // this->audible_ = buffer;

  if(buffer != nullptr) {
    this->set_duration(buffer->duration);
  }
}

PlayAudioSource::~PlayAudioSource()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION,
                     nom::NOM_LOG_PRIORITY_VERBOSE);
}

std::unique_ptr<IActionObject> PlayAudioSource::clone() const
{
  return( nom::make_unique<self_type>( self_type(*this) ) );
}

IActionObject::FrameState
PlayAudioSource::update(real32 t, uint8 b, int16 c, real32 d)
{
  real32 delta_time = t;
  auto status = this->status();
  const real32 duration = d;
  const auto speed = this->speed();
  auto current_frame = this->curr_frame_;

  uint32 audio_state =
    audio::state(&this->audible_[current_frame], this->impl_);

  // Clamp delta values that go beyond the time duration bounds; this adds
  // stability to variable time steps
  if(delta_time > (duration / speed)) {
    delta_time = duration / speed;
  }

  // TODO(jeff): Verify alignment of this variable in regards to reading at
  // random from the samples data; I'm pretty sure that this **DOES** matter!
  auto input_pos = this->input_pos_;
  int16* samples = NOM_SCAST(int16*, this->audible_[current_frame].samples);
  auto format = this->audible_[current_frame].channel_format;
  auto sample_count = (this->audible_[current_frame].sample_count);
  auto samples_per_second = (this->audible_[current_frame].sample_rate);

  // Cache up to two seconds (2s) worth of audio data

    if(this->audible_[current_frame].samples_read < sample_count) {
      this->audible_[current_frame].samples_read +=
        this->fp_->read(samples + input_pos, format, samples_per_second);
      this->input_pos_ += samples_per_second;

      NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "samples read:",
                   this->audible_[current_frame].samples_read);

      // FIXME(jeff): We shouldn't ever be calling from this pointer directly!
      if(this->impl_ != nullptr) {

        // if(this->audible_->samples != nullptr) {
          // this->release();
        // }

        // this->impl_->push_buffer(this->audible_);
        this->impl_->queue_buffer(&this->audible_[current_frame]);
        ++this->curr_frame_;
      }
    }
  // }
NOM_DUMP(curr_frame_);
  audio_state = audio::state(&this->audible_[current_frame], this->impl_);

  // Continue playing the animation only when we are inside our frame duration
  // bounds; this adds stability to variable time steps
  if(delta_time < (duration / speed) && this->audible_[current_frame].samples_read != 0)
  // if(delta_time < (duration / speed) &&
  // if(
     // audio_state != audio::AUDIO_STATE_STOPPED)
  {
    if(audio_state != audio::AUDIO_STATE_PLAYING &&
       this->audible_[curr_frame_].samples_read >= sample_count)
    {
      NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "play!");
      audio::play(&this->audible_[curr_frame_], this->impl_);
    }

    status = FrameState::PLAYING;
    this->set_status(status);
  } else {
    this->last_frame(delta_time);
    status = FrameState::COMPLETED;
    this->set_status(status);
  }

  return status;
}

IActionObject::FrameState PlayAudioSource::next_frame(real32 delta_time)
{
  delta_time = this->timer_.to_seconds();

  // if(this->audible_) {
    this->audible_[curr_frame_].elapsed_seconds = this->timer_.ticks();
  // }

  this->first_frame(delta_time);

  return this->update(delta_time, 0.0f, 0.0f, this->duration());
}

IActionObject::FrameState PlayAudioSource::prev_frame(real32 delta_time)
{
  delta_time = this->timer_.to_seconds();
  // if(this->audible_) {
    this->audible_[curr_frame_].elapsed_seconds = this->timer_.ticks();
  // }

  this->first_frame(delta_time);

  return this->update(delta_time, 0.0f, 0.0f, this->duration());
}

void PlayAudioSource::pause(real32 delta_time)
{
  this->timer_.pause();

  // if(this->audible_) {
  for(auto buffer_idx = 0; buffer_idx != audio::NUM_SOURCES; ++buffer_idx) {
    audio::pause(&this->audible_[buffer_idx], this->impl_);
    this->audible_[buffer_idx].elapsed_seconds = this->timer_.ticks();
  }
  // }
}

void PlayAudioSource::resume(real32 delta_time)
{
  this->timer_.unpause();

  // if(this->audible_) {
    // this->audible_->elapsed_seconds = this->timer_.ticks();
  // }
  for(auto buffer_idx = 0; buffer_idx != audio::NUM_SOURCES; ++buffer_idx) {
    this->audible_[buffer_idx].elapsed_seconds = this->timer_.ticks();
    audio::resume(&this->audible_[buffer_idx], this->impl_);
  }
}

void PlayAudioSource::rewind(real32 delta_time)
{
  // ...Reset the animation...
  this->elapsed_frames_ = 0.0f;
  this->timer_.stop();
  this->set_status(FrameState::PLAYING);

  // if(this->audible_ != nullptr) {
    // audio::stop(this->audible_[curr_frame_], this->impl_);
    // this->audible_->samples_read = 0;
  // }

  for(auto buffer_idx = 0; buffer_idx != audio::NUM_SOURCES; ++buffer_idx) {
    audio::stop(&this->audible_[buffer_idx], this->impl_);
    this->audible_[buffer_idx].samples_read = 0;
  }
}

void PlayAudioSource::release()
{
  auto num_buffers = this->audible_.size();
  for(auto buffer_idx = 0; buffer_idx != num_buffers; ++buffer_idx) {
    // audio::stop(this->audible_[buffer_idx], this->impl_);

    // audio::free_buffer(this->audible_, this->impl_);
    audio::free_samples(this->audible_[buffer_idx].channel_format,
                        this->audible_[buffer_idx].samples);
    // this->audible_[buffer_idx].buffer_id = 0;
  }
  // audio::free_buffer(this->audible_, this->impl_);

}

// Private scope

void PlayAudioSource::first_frame(real32 delta_time)
{
  if(this->timer_.started() == false) {
    this->timer_.start();

    NOM_LOG_INFO(NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                 "BEGIN at", delta_time);

    // audio::play(this->audible_, this->impl_);
  }
}

void PlayAudioSource::last_frame(real32 delta_time)
{
  NOM_LOG_INFO(NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
               "END at", delta_time);

  this->timer_.stop();

  for(auto buffer_idx = 0; buffer_idx != audio::NUM_SOURCES; ++buffer_idx) {
    audio::stop(&this->audible_[buffer_idx], this->impl_);
    this->audible_[buffer_idx].samples_read = 0;
  }
}

} // namespace nom
