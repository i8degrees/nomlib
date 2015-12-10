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
#include "nomlib/audio/NullSoundBuffer.hpp"

namespace nom {

NullSoundBuffer::NullSoundBuffer()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);
}

NullSoundBuffer::~NullSoundBuffer()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);
}

uint32 NullSoundBuffer::buffer_id() const
{
  return 0;
}

const std::vector<int16>&
NullSoundBuffer::samples() const
{
  return this->samples_;
}

void NullSoundBuffer::set_buffer(const std::vector<int16>& samples)
{
  this->samples_ = samples;
}

int64 NullSoundBuffer::duration() const
{
  return 0;
}

uint32 NullSoundBuffer::channel_count() const
{
  return 0;
}

uint32 NullSoundBuffer::channel_format() const
{
  return 0;
}

uint32 NullSoundBuffer::sample_rate() const
{
  return 0;
}

nom::size_type NullSoundBuffer::sample_count() const
{
  return 0;
}

nom::size_type NullSoundBuffer::audio_bytes() const
{
  return 0;
}

bool NullSoundBuffer::load_file(const std::string& filename)
{
  return true;
}

// void NullSoundBuffer::attach(Sound* sound) const
// {
//   // Do nothing
// }

// void NullSoundBuffer::detach(Sound* sound) const
// {
//   // Do nothing
// }

} // namespace nom
