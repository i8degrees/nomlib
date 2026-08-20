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
#ifndef NOMLIB_AL_AUDIO_DEVICE_HEADERS
#define NOMLIB_AL_AUDIO_DEVICE_HEADERS

#include "export.hpp"
#include "nomlib/config.hpp"
#include "nomlib/core/IObject.hpp"
#include "nomlib/math/Point3.hpp"
#include "nomlib/audio/audio_defs.hpp"
#include "nomlib/audio/SoundFile.hpp"

#include <vector>
#include <functional>

// Forward declarations
#include "nomlib/audio/AL/priv_openal.hpp"

namespace nom {
namespace audio {

class IOAudioEngine;
struct AudioSpec;

NOM_EXPORT ALCdevice_struct* current_device();
NOM_EXPORT ALCcontext_struct* current_context();

NOM_EXPORT void free_audio_device(ALCdevice_struct* dev);
NOM_EXPORT void free_audio_context(ALCcontext_struct* ctx);

NOM_EXPORT void*
process_addr(const char* token);

// TODO(jeff): Verify if we should be using alIsExtensionPresent or
// alcIsExtensionPresent for querying extensions, etc.

/// Obtain support info regarding a particular extension
NOM_EXPORT bool extension_available(const char* key);

/// Obtain support info regarding a particular extension
// bool extension_available(const char* key, void* target);

// TODO(jeff): Rename function -- something along the lines of
// config_available..?
// if err, return zero; if successful, return the key's value, a number greater
// than zero.
NOM_EXPORT uint32 enum_available(const char* key);

NOM_EXPORT bool
context_extension(const char* key, ALCdevice_struct* target);

typedef std::vector<std::string> device_name_list;

NOM_EXPORT
const char* default_output_device_name(ALCdevice_struct* target);

NOM_EXPORT
const char* default_input_device_name(ALCdevice_struct* target);

// TODO(jeff): Try to factor out the use of std::vector here
NOM_EXPORT device_name_list output_device_names(void* target);

// TODO(jeff): Implement function
NOM_EXPORT device_name_list input_device_names(void* target);

NOM_EXPORT int refresh_rate(void* target);
NOM_EXPORT bool sync_context(void* target);

/// \brief Get the sampling rate of audio playback.
NOM_EXPORT int sample_rate(void* target);

/// \brief Set the sampling rate of audio playback.
// void set_sample_rate(int sample_rate, void* target);

NOM_EXPORT int max_mono_sources(void* target);
NOM_EXPORT int max_stereo_sources(void* target);

/// \brief Get the maximum number of simultaneous playing audio buffers.
NOM_EXPORT int max_sources(void* target);

/// \brief Set the maximum number of simultaneous playing audio buffers.
// void set_max_sources(int num_sources, void* target);

NOM_EXPORT bool cap(uint32 caps, uint32 key);
NOM_EXPORT void set_cap(uint32* caps, uint32 format);

struct NOM_EXPORT AudioDevice
{
  // ALAudioDevice
  void* impl = nullptr;
};

/// \see http://www.opensource.apple.com/source/OpenAL/OpenAL-54/Source/OpenAL/
// TODO(jeff): Private header..?
struct NOM_EXPORT ALAudioDevice
{
  /// Audio device handle
  ALCdevice_struct* dev = nullptr;

  /// Audio device context
  ALCcontext_struct* ctx = nullptr;

  // IOAudioEngine* impl = nullptr;

  const char* name = "ALAudioDevice";

  // uint32 max_channels = 0;

  uint32 capabilities = CAPS_UNDEFINED;

  // real64 elapsed_ticks_ = 0.0f;

  // typedef std::function<device_name_list()> input_devs_func;
  typedef std::function<device_name_list(ALAudioDevice*)> output_devs_func;

  output_devs_func enumerate_output_devices;
  // input_devs_func enumerate_input_devices;
};

NOM_EXPORT IOAudioEngine*
init_audio(const audio::AudioSpec* request, audio::AudioSpec* spec);

// void shutdown_openal(ALAudioDevice* impl);
NOM_EXPORT void shutdown_audio(IOAudioEngine* impl);

// const char* audio_device_name(IOAudioEngine* target);

/// \brief Get the number of audio input/output ports that are available.
///
/// \returns ...
NOM_EXPORT int num_audio_devices();

/// \brief
///
/// \param dev An audio device that has been created with audio::init_audio.
///
/// \returns ...
// const char* audio_device(void* target);

// real64 device_time(void* target);

} // namespace audio
} // namespace nom

#endif // NOMLIB_AL_AUDIO_DEVICE_HEADERS defined
