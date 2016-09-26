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
#include "nomlib/audio/AL/osx/apple_extensions.hpp"

// Private headers
#include "nomlib/audio/audio_defs.hpp"
#include "nomlib/audio/AL/ALAudioDevice.hpp"

#include <OpenAL/MacOSX_OALExtensions.h>

// Forward declarations
#include "nomlib/audio/AL/OpenAL.hpp"

namespace nom {
namespace audio {

// Apple OpenAL v1.1-v54 extensions
// See http://www.opensource.apple.com/source/OpenAL/OpenAL-54/Source/OpenAL/
// typedef alcMacOSXGetMixerOutputRateProcPtr osx_get_sample_rate_func;
// typedef alcMacOSXMixerOutputRateProcPtr osx_set_sample_rate_func;
// typedef alcMacOSXGetMixerMaxiumumBussesProcPtr osx_get_max_sources_func;
// typedef alcMacOSXMixerMaxiumumBussesProcPtr osx_set_max_sources_func;

// typedef real64(*openal_get_sample_rate_func)();
// typedef real64(*openal_get_sample_rate_func)(ALCdevice_struct*);

// typedef void(*openal_set_sample_rate_func)(real64 sample_rate);
// typedef int(*openal_get_max_sources_func)();
// typedef void(*openal_set_max_sources_func)(int num_sources);

real64 osx_sample_rate()
{
  real64 sample_rate = 0.0f;

  ALdouble(*get_sample_rate_func)() =
    (alcMacOSXGetMixerOutputRateProcPtr) audio::process_addr("alcMacOSXGetMixerOutputRate");
  if(get_sample_rate_func != nullptr) {
    sample_rate = get_sample_rate_func();
  }

  return sample_rate;
}

void osx_set_sample_rate(real64 sample_rate)
{
  ALvoid(*set_sample_rate_func)(ALdouble) =
    (alcMacOSXMixerOutputRateProcPtr) audio::process_addr("alcMacOSXMixerOutputRate");
  if(set_sample_rate_func != nullptr) {
    set_sample_rate_func(sample_rate);
  }
}

int osx_max_sources()
{
  int num_sources = 0;

  ALint(*get_max_sources_func)() =
    (alcMacOSXGetMixerMaxiumumBussesProcPtr) audio::process_addr("alcMacOSXGetMixerMaxiumumBusses");
  if(get_max_sources_func != nullptr) {
    num_sources = get_max_sources_func();
  }

  return num_sources;
}

void osx_set_max_sources(int num_sources)
{
  ALvoid(*set_max_sources_func)(ALint) =
    (alcMacOSXMixerMaxiumumBussesProcPtr) audio::process_addr("alcMacOSXMixerMaxiumumBusses");
  if(set_max_sources_func != nullptr) {
    set_max_sources_func(num_sources);
  }
}

} // namespace audio
} // namespace nom
