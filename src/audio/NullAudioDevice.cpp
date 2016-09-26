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
#include "nomlib/audio/NullAudioDevice.hpp"

#include "nomlib/audio/NullAudioDeviceCaps.hpp"

namespace nom {
namespace audio {

NullAudioDevice::NullAudioDevice()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);
}

NullAudioDevice::~NullAudioDevice()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);

  this->close();
}

bool NullAudioDevice::valid() const
{
  return this->initialized_;
}

// void* NullAudioDevice::device() const
// {
//   return nullptr;
// }

// void* NullAudioDevice::context() const
// {
//   return nullptr;
// }

std::string NullAudioDevice::device_name() const
{
  return this->device_name_;
}

// IOAudioEngine* NullAudioDevice::caps() const
// {
//   return this->impl_;
// }

IOAudioEngine* NullAudioDevice::open(const audio::AudioSpec* spec)
{
  IOAudioEngine* engine = nullptr;

  if(spec != nullptr) {
  }

  this->device_name_ = "NullAudioDevice";

  // this->impl_ = new NullAudioEngineCaps();
  // if(this->impl_ != nullptr) {
  //   this->initialized_ = true;
  // }

  // return(this->valid() == true);

  engine = new NullAudioEngineCaps();
  if(engine != nullptr) {
    this->initialized_ = true;
  }

  return engine;
}

void NullAudioDevice::suspend()
{

}

void NullAudioDevice::resume()
{

}

void NullAudioDevice::close()
{
  // NOM_DELETE_PTR(this->impl_);
}

IOAudioEngine* create_null_audio_device(const audio::AudioSpec* spec)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_VERBOSE);

  IOAudioEngine* engine = nullptr;
  NullAudioDevice* dev = new NullAudioDevice();

  if(dev != nullptr) {
    engine = dev->open(spec);
  }

  return engine;
}

} // namespace audio
} // namespace nom
