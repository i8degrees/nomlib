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

// comment out static sample_rate in ALAudioDevice.cpp

static
real64 get_sample_rate(ALCdevice_struct* target)
{
  ALCint sample_rate = 0;
  real64 result = 0.0f;

  // FIXME(jeff): Implement type polymorphism here
  auto dev = NOM_SCAST(ALCdevice_struct*, target);
  alcGetIntegerv(dev, ALC_FREQUENCY, sizeof(ALCint), &sample_rate);

  // TODO(jeff): This function signature returns a 64-bit floating-point
  // number -- we ought to carefully consider the ramifications of this and
  // see about safeguarding against significant bit loss.
  result = NOM_SCAST(real64, sample_rate);

  return result;
}

real64 sample_rate(ALCdevice_struct* target)
{
  real64 sample_rate = 0.0f;
  bool osx_extension = false;

  osx_extension = audio::context_extension("ALC_EXT_MAC_OSX", target);
  if(osx_extension == true) {
    real64(*get_sample_rate_func)() = nullptr;
#if defined(NOM_PLATFORM_OSX) && defined(NOM_USE_APPLE_OPENAL)
    get_sample_rate_func =
      (osx_get_sample_rate) audio::process_addr("alcMacOSXGetMixerOutputRate");
#endif
    if(get_sample_rate_func != nullptr) {
      sample_rate = get_sample_rate_func();
    }
  } else if(osx_extension == false) {
    auto get_sample_rate_func =
      (openal_get_sample_rate_func) get_sample_rate;
    if(get_sample_rate_func != nullptr) {
      sample_rate = get_sample_rate_func(target);
    }
  }

  return sample_rate;
}

void set_sample_rate(real64 sample_rate)
{
#if defined(NOM_PLATFORM_OSX) && defined(NOM_USE_APPLE_OPENAL)
  auto set_sample_rate_func =
    (osx_set_sample_rate) audio::process_addr("alcMacOSXMixerOutputRate");
  if(set_sample_rate_func != nullptr) {
    set_sample_rate_func(sample_rate);
  }
#endif
}

int max_sources()
{
  int num_sources = 0;
#if defined(NOM_PLATFORM_OSX) && defined(NOM_USE_APPLE_OPENAL)
  auto get_max_sources_func =
    (osx_get_max_sources) audio::process_addr("alcMacOSXGetMixerMaxiumumBusses");
  if(get_max_sources_func != nullptr) {
    num_sources = get_max_sources_func();
  }
#endif
  return num_sources;
}

void set_max_sources(int num_sources)
{
#if defined(NOM_PLATFORM_OSX) && defined(NOM_USE_APPLE_OPENAL)
  auto set_max_sources_func =
    (osx_set_max_sources) audio::process_addr("alcMacOSXMixerMaxiumumBusses");
  if(set_max_sources_func != nullptr) {
    set_max_sources_func(num_sources);
  }
#endif
}

} // namespace audio
} // namespace nom
