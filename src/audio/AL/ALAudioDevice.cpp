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
#include "nomlib/audio/AL/ALAudioDevice.hpp"

// Private headers
#include "nomlib/audio/AL/ALAudioDeviceCaps.hpp"
#include "nomlib/audio/audio_defs.hpp"
#include "nomlib/core/strings.hpp"
#include "nomlib/core/err.hpp"
// #include "nomlib/system/HighResolutionTimer.hpp"
#include "nomlib/system/Timer.hpp"
#include "nomlib/audio/AL/osx/apple_extensions.hpp"
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/audio/AL/OpenAL.hpp"

namespace nom {
namespace audio {

#if 0
enum ExtensionType: uint32
{
  EXTENSION_TYPE_UNDEFINED = 0x00000,

  EXTENSION_TYPE_CONTEXT = 0x10000,
  EXTENSION_TYPE_DEVICE = 0x20000,
};

// Get the type of extension query you are looking for by inspecting the first
// three characters of the parameter key.
static
uint32 extension_type(const char* key)
{
  uint32 ext_type = EXTENSION_TYPE_UNDEFINED;
  int ctx_ext = 0;
  int dev_ext = 0;

  if(key == nullptr) {
    return ext_type;
  }

  ctx_ext = nom::compare_cstr_insensitive("ALC_", key, 2);
  dev_ext = nom::compare_cstr_insensitive("AL_", key, 2);

  if(ctx_ext == 0) {
    ext_type = EXTENSION_TYPE_CONTEXT;
  } else if(dev_ext == 0) {
    ext_type = EXTENSION_TYPE_DEVICE;
  }

  return ext_type;
}
#endif

// static
ALCdevice_struct* current_device()
{
  ALCcontext_struct* ctx = audio::current_context();
  ALCdevice_struct* dev = nullptr;

  if(ctx != nullptr) {
    dev = alcGetContextsDevice(ctx);
  }

  return dev;
}

ALCcontext_struct* current_context()
{
  ALCcontext_struct* ctx = nullptr;

  ctx = alcGetCurrentContext();

  return ctx;
}

void free_audio_device(ALCdevice* dev)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);

  if(dev != nullptr) {
    alcCloseDevice(dev);
#if 0
    // TODO(jeff): Verify that this enumeration is available on Windows
    // Availability verified: Apple's OpenAL SDK and OpenAL-Soft.
    {
      auto enum_avail = audio::enum_available("ALC_CONNECTED");
      if(enum_avail != 0) {
        int dev_connected = 0;
  // NOTE(jeff): Defined in file at openal-soft.git/include/AL/alext.h
  #if defined(ALC_EXT_disconnect)
        alcGetIntegerv(dev, ALC_CONNECTED, sizeof(int), &dev_connected);
  #endif
        NOM_DUMP(dev_connected); // should be 1 when the extension is available
      }
    }
#endif
    dev = nullptr;
  }
}

void free_audio_context(ALCcontext* ctx)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);

  if(ctx != nullptr) {
    // Disable context; we must not free a current context as per OpenAL v1.1
    // API docs
    if(alcMakeContextCurrent(nullptr) == false) {
      // TODO(jeff): Use nom::set_error
      NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                  "Failed to stop the current context.");
    }

    // Release context
    AL_CLEAR_ERR();
    alcDestroyContext(ctx);
    ALenum error = alGetError();
    if(error != AL_NO_ERROR) {
      // TODO(jeff): Use nom::set_error
      NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Failed to destroy context.");
    }

    ctx = nullptr;
  }
}

void* process_addr(const char* token)
{
  auto token_addr = NOM_SCAST(const ALCchar*, token);
  void* result = alGetProcAddress(token_addr);

  return result;
}

bool cap(uint32 caps, uint32 key)
{
  uint32 result = CAPS_UNDEFINED;

  if(caps != CAPS_UNDEFINED) {
    result = ((caps & key) == key);
  }

  return result;
}

void set_cap(uint32* caps, uint32 format)
{
  *caps |= format;
}

static
const char* al_string(int param, ALCdevice_struct* target)
{
  const char* result = "\0";
  const char* dev_name = nullptr;

  dev_name = alcGetString(target, param);

  if(dev_name != nullptr) {
    result = dev_name;
  }

  return result;
}

bool extension(const char* key)
{
  bool result = false;
  ALboolean ext_avail = AL_FALSE;

  if(key == nullptr) {
    // TODO(jeff): Err handling
    return false;
  }

  ext_avail = alIsExtensionPresent(key);
  if(ext_avail == AL_TRUE) {
    result = true;
  }

  return result;
}

bool context_extension(const char* key, ALCdevice_struct* target)
{
  bool result = false;
  ALCboolean ext_avail = ALC_FALSE;

  if(key == nullptr) {
    // TODO(jeff): Err handling
    return false;
  }

  ext_avail = alcIsExtensionPresent(target, key);

  if(ext_avail == ALC_TRUE) {
    result = true;
  }

  return result;
}

uint32 enum_available(const char* key)
{
  uint32 result = 0;
  ALenum enum_avail = AL_NONE;

  if(key != nullptr) {

    enum_avail = alGetEnumValue(key);

    // IMPORTANT(jeff): Fixes a bug regarding Apple's OpenAL SDK API return
    // value of negative one (-1).
    if(enum_avail == AL_INVALID) {
      enum_avail = 0;
    }

    result = enum_avail;

  }

  return result;
}

const char*
default_output_device_name(ALCdevice_struct* target)
{
  const char* result = "\0";
  const char* dev_name = nullptr;
  ALCdevice_struct* dev = target;

  if(audio::context_extension("ALC_ENUMERATE_ALL_EXT", dev) == true) {
    dev_name = audio::al_string(ALC_DEFAULT_ALL_DEVICES_SPECIFIER, dev);
  } else if(audio::context_extension("ALC_ENUMERATION_EXT", dev) == true) {
    dev_name = audio::al_string(ALC_DEFAULT_DEVICE_SPECIFIER, dev);
  }

  if(dev_name != nullptr) {
    result = dev_name;
  }

  return result;
}

const char*
default_input_device_name(ALCdevice_struct* target)
{
  const char* result = "\0";
  const char* dev_name = nullptr;
  ALCdevice_struct* dev = target;

  // TODO(jeff): Verify if we need to check for extensions here
  dev_name = audio::al_string(ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER, dev);

  if(dev_name != nullptr) {
    result = dev_name;
  }

  return result;
}

// TODO(jeff): Verify if we have any way of querying the size of the device
// names from OpenAL
static
void parse_openal_device_strings(const char* dev_name, std::stringstream& os)
{
  uint32 found_null_term = 0;
  uint32 current_char = 0;
  for(uint32 char_pos = 0; char_pos < nom::MAX_STRING_LENGTH; ++char_pos) {

    current_char = dev_name[char_pos];
    os << NOM_SCAST(uint8, current_char);

    // A space, or... end of string
    if(current_char == 0x0) {
      ++found_null_term;
    }

    // End of string
    if(found_null_term == 2) {
      break;
    }
  }
}

device_name_list output_device_names(void* target)
{
  ALCdevice_struct* dev = nullptr;
  const char* output_devs = nullptr;
  device_name_list output_dev_list;

  if(target != nullptr) {
    // dev = target->dev;
    // FIXME(jeff): Implement polymorphism!
    dev = NOM_SCAST(ALCdevice_struct*, target);
  }

  if(audio::context_extension("ALC_ENUMERATE_ALL_EXT", dev) == AL_TRUE) {
    // IMPORTANT(jeff): Prefer the non-standard enumeration extension when it
    // is available to us -- a complete list of available audio devices will be
    // presented.
    output_devs = audio::al_string(ALC_ALL_DEVICES_SPECIFIER, dev);
  } else if(audio::context_extension("ALC_ENUMERATION_EXT", dev) == AL_TRUE) {
    // IMPORTANT(jeff): A complete list of audio devices will **not** be shown
    // to us -- this implementation is unlikely to show us more than the current
    // default audio interface.
    output_devs = audio::al_string(ALC_DEVICE_SPECIFIER, dev);
  }

  if(output_devs != nullptr) {
    std::stringstream os;
    audio::parse_openal_device_strings(output_devs, os);
    output_dev_list.push_back(os.str());
  }

  return output_dev_list;
}

// TODO(jeff): Verify that this is right
device_name_list input_device_names(void* target)
{
  ALCdevice_struct* dev = nullptr;
  const char* input_devs = nullptr;
  device_name_list input_dev_list;

  if(target != nullptr) {
    // dev = target->dev;
    // FIXME(jeff): Implement polymorphism!
    dev = NOM_SCAST(ALCdevice_struct*, target);
  }

  if(audio::context_extension("ALC_ENUMERATE_ALL_EXT", dev) == AL_TRUE) {
    // IMPORTANT(jeff): Prefer the non-standard enumeration extension when it
    // is available to us -- a complete list of available audio devices will be
    // presented.
    input_devs = audio::al_string(ALC_ALL_DEVICES_SPECIFIER, dev);
  } else if(audio::context_extension("ALC_ENUMERATION_EXT", dev) == AL_TRUE) {
    // IMPORTANT(jeff): A complete list of audio devices will **not** be shown
    // to us -- this implementation is unlikely to show us more than the current
    // default audio interface.
    input_devs = audio::al_string(ALC_CAPTURE_DEVICE_SPECIFIER, dev);
  }

  if(input_devs != nullptr) {
    std::stringstream os;
    audio::parse_openal_device_strings(input_devs, os);
    input_dev_list.push_back(os.str());
  }

  return input_dev_list;
}

static int*
create_openal_attributes(const AudioSpec* spec)
{
  const uint8 NUM_ATTRIBUTES = 11;

  ALCint* attrs = new ALCint[NUM_ATTRIBUTES];
  if(attrs == nullptr) {
    nom::set_error("Failed to allocate memory for attributes.");
    return attrs;
  }


  if(spec != nullptr) {
    int sync_ctx = 0;

    if(spec->sync_context == true) {
      sync_ctx = 1;
    } else {
      sync_ctx = 0;
    }

    attrs[0] = ALC_FREQUENCY;
    attrs[1] = spec->sample_rate;

    attrs[2] = ALC_REFRESH;
    attrs[3] = spec->refresh_rate;

    attrs[4] = ALC_SYNC;
    attrs[5] = sync_ctx;

    attrs[6] = ALC_MONO_SOURCES;
    attrs[7] = spec->num_mono_sources;

    attrs[8] = ALC_STEREO_SOURCES;
    attrs[9] = spec->num_stereo_sources;
  }

  attrs[10] = 0;

  return attrs;
}

static audio::AudioSpec
create_openal_attributes(ALCdevice_struct* dev)
{
  audio::AudioSpec spec = {};

  if(dev == nullptr) {
    nom::set_error("Target audio device was invalid (NULL).");
    return spec;
  }

  if(dev != nullptr) {
    spec.engine = "openal";
    spec.name = audio::default_output_device_name(dev);
    spec.sample_rate = audio::sample_rate(dev);
    spec.refresh_rate = audio::refresh_rate(dev);
    spec.sync_context = audio::sync_context(dev);
    spec.num_mono_sources = audio::max_mono_sources(dev);
    spec.num_stereo_sources = audio::max_stereo_sources(dev);
  }

  return spec;
}

// TODO(jeff): Finish implementation; split off from ::init_openal
//
// See also: http://stackoverflow.com/questions/5056986/openal-how-can-i-create-more-than-2-or-3-stereo-sources
static ALCcontext_struct*
init_openal_context(const audio::AudioSpec* request, void* driver)
{
  ALenum error_code = AL_NO_ERROR;
  int* attributes = nullptr;

  // FIXME(jeff): Implement polymorphism here
  auto dev = NOM_SCAST(ALCdevice_struct*, driver);
  // auto ctx = NOM_SCAST(ALCcontext_struct*, context);
  ALCcontext_struct* ctx = nullptr;

  nom::clear_error();
  attributes = audio::create_openal_attributes(request);
  if(nom::error_state() == true) {
    // TODO(jeff): Use nom::set_error
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, nom::error());
  }

  // NOTE: attach a context state to the audio device
  AL_CLEAR_ERR();
  ctx = alcCreateContext(dev, attributes);
  if(ctx == nullptr) {
    error_code = alGetError();
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to create the audio context.",
                "See OpenAL error code", error_code);
    return ctx;
  }

  if(alcMakeContextCurrent(ctx) == false) {
    // TODO(jeff): Use nom::set_error
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to make the context current.");
    return ctx;
  }

  AL_CLEAR_ERR();
  alcProcessContext(ctx);
  error_code = alGetError();
  if(error_code != AL_NO_ERROR) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to make the context current.",
                "See OpenAL error code", error_code);
    return ctx;
  }

#if defined(NOM_PLATFORM_OSX) && defined(NOM_USE_APPLE_OPENAL)
  if( audio::context_extension("ALC_EXT_MAC_OSX", dev) == true ) {
    int total_num_sources = audio::osx_max_sources();
    int half_total_num_sources = (total_num_sources / 2);
    int max_mono_sources = attributes[7];
    int max_stereo_sources = attributes[9];
    int max_sources = 0; // computed result

    // IMPORTANT(jeff): When using Apple's OpenAL distribution, we must use
    // the ALC_EXT_MAC_OSX extension for querying and setting the output audio
    // frequency rate; the ALC_FREQUENCY attribute is ignored during context
    // creation.
    audio::osx_set_sample_rate(attributes[1]);

    NOM_LOG_VERBOSE(NOM_LOG_CATEGORY_TEST,
                    "max_mono_sources (clamped):",
                    nom::clamp_min<int>(max_mono_sources, half_total_num_sources));
    NOM_LOG_VERBOSE(NOM_LOG_CATEGORY_TEST,
                    "max_stereo_sources (clamped):",
                    nom::clamp_min<int>(max_stereo_sources, half_total_num_sources));

    max_sources = (max_mono_sources + max_stereo_sources);
    NOM_ASSERT(max_sources <= total_num_sources);

    audio::osx_set_max_sources(max_sources);
  }
#endif // end if defined NOM_USE_APPLE_OPENAL

  return ctx;
}

int refresh_rate(void* target)
{
  ALCint refresh_rate = 0;
  // FIXME(jeff): Implement type polymorphism here
  auto dev = NOM_SCAST(ALCdevice_struct*, target);

  if(dev != nullptr) {
    alcGetIntegerv(dev, ALC_REFRESH, sizeof(ALCint), &refresh_rate);
  }

  return refresh_rate;
}

bool sync_context(void* target)
{
  ALCint sync_context = 0;
  bool result = false;
  // FIXME(jeff): Implement type polymorphism here
  auto dev = NOM_SCAST(ALCdevice_struct*, target);

  alcGetIntegerv(dev, ALC_SYNC, sizeof(ALCint), &sync_context);

  if(sync_context == 1) {
    result = true;
  }

  return result;
}

int sample_rate(void* target)
{
  ALCint sample_rate = 0;
  // FIXME(jeff): Implement type polymorphism here
  auto dev = NOM_SCAST(ALCdevice_struct*, target);

  bool osx_extension = audio::context_extension("ALC_EXT_MAC_OSX", dev);
  if(osx_extension == true) {
#if defined(NOM_PLATFORM_OSX) && defined(NOM_USE_APPLE_OPENAL)
    // TODO(jeff): This function signature returns a 64-bit floating-point
    // number -- we ought to carefully consider the ramifications of this and
    // see about safeguarding against significant bit loss.
    sample_rate = NOM_SCAST(int, audio::osx_sample_rate());
#endif
  } else if(osx_extension == false) {
    alcGetIntegerv(dev, ALC_FREQUENCY, sizeof(ALCint), &sample_rate);
  }

  return sample_rate;
}

#if 0
static
void set_sample_rate(int sample_rate, void* target)
{
  // FIXME(jeff): Implement type polymorphism here
  auto dev = NOM_SCAST(ALCdevice_struct*, target);

  bool osx_extension = audio::context_extension("ALC_EXT_MAC_OSX", dev);
  if(osx_extension == true) {
    audio::osx_set_sample_rate(sample_rate);
  }
}
#endif

int max_mono_sources(void* target)
{
  int num_mono_sources = 0;

  // FIXME: Implement polymorphism for target pointer
  auto dev = NOM_SCAST(ALCdevice_struct*, target);

  if(dev == nullptr) {
    return num_mono_sources;
  }

  alcGetIntegerv(dev, ALC_MONO_SOURCES, sizeof(num_mono_sources),
                 &num_mono_sources);

  return num_mono_sources;
}

int max_stereo_sources(void* target)
{
  int num_stereo_sources = 0;

  // FIXME: Implement polymorphism for target pointer
  auto dev = NOM_SCAST(ALCdevice_struct*, target);

  if(dev == nullptr) {
    return num_stereo_sources;
  }

  alcGetIntegerv(dev, ALC_STEREO_SOURCES, sizeof(num_stereo_sources),
                 &num_stereo_sources);

  return num_stereo_sources;
}
#if 1
int max_sources(void* target)
{
  int num_mono_sources = 0;
  int num_stereo_sources = 0;
  int total_num_sources = 0;

  // FIXME(jeff): Implement type polymorphism here
  auto dev = NOM_SCAST(ALCdevice_struct*, target);

  if(audio::context_extension("ALC_EXT_MAC_OSX", dev) == true) {
#if defined(NOM_PLATFORM_OSX) && defined(NOM_USE_APPLE_OPENAL)
    total_num_sources = audio::osx_max_sources();
#endif
  } else {
    // IMPORTANT(jeff): The Mac OS X extension [1] for querying the number of
    // sources only offers query of the total number of sources, and not the
    // individual number of mono and stereo sources. In interests of a
    // consistent API, we do the following math below to create a comparable
    // sum of the total number of sound sources.
    //
    // 1. We must go through the extension's API in order to determine the
    // correct number of sources.
    alcGetIntegerv(dev, ALC_MONO_SOURCES, sizeof(num_mono_sources),
                   &num_mono_sources);
    alcGetIntegerv(dev, ALC_STEREO_SOURCES, sizeof(num_stereo_sources),
                   &num_stereo_sources);

    total_num_sources =
      nom::clamp_min<int>( (num_mono_sources + num_stereo_sources),
                           TOTAL_NUM_SOURCES);
    NOM_ASSERT(total_num_sources <= TOTAL_NUM_SOURCES);
  }

  return total_num_sources;
}
#endif
#if 0
void set_max_sources(int num_sources, void* target)
{
  // FIXME(jeff): Implement type polymorphism here
  auto dev = NOM_SCAST(ALCdevice_struct*, target);

  if(audio::context_extension("ALC_EXT_MAC_OSX", dev) == true) {
#if defined(NOM_PLATFORM_OSX) && defined(NOM_USE_APPLE_OPENAL)
    audio::osx_set_max_sources(num_sources);
#endif
  }
  // TODO(jeff): Finish implementation; see also, ::create_spec,
  // ::init_openal_context

  // int* attributes = nullptr;
  audio::AudioSpec spec = {};

  spec.sample_rate = audio::sample_rate(dev);
  spec.refresh_rate = audio::refresh_rate(dev);
  spec.sync_context = audio::sync_context(dev);
  spec.num_mono_sources = num_sources;
  spec.num_stereo_sources = num_sources;

  // attributes = audio::create_openal_attributes(&spec);
  // ALCcontext* ctx = current_context();
  // if(ctx != nullptr && dev->valid() == true) {
  // }

  // ALCcontext_struct* ctx = nullptr;
  // ctx = audio::init_openal_context(&spec, dev);
}
#endif

static IOAudioEngine*
init_openal_output(const audio::AudioSpec* request, audio::AudioSpec* spec)
{
  ALAudioDevice* driver = new ALAudioDevice;
  IOAudioEngine* impl = nullptr;
  const char* req_audio_dev_name = nullptr;

  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_VERBOSE);

  if(driver == nullptr) {
    return impl;
  }

  auto dev = driver->dev;
  auto ctx = driver->ctx;

  if(request != nullptr) {
    if(request->name != nullptr) {
      req_audio_dev_name = request->name;
    }
  }

  // If this object instance has been used before; we must clean up its memory
  // first...
  // if(dev != nullptr) {
    // audio::shutdown_openal(driver);
  // }

  // TODO(jeff): Verify that this enumeration is available on Windows
  // Availability verified: Apple's OpenAL SDK and OpenAL-Soft.
  {
    if(audio::context_extension("ALC_EXT_disconnect", dev) == true) {
      auto enum_avail = audio::enum_available("ALC_CONNECTED");
      if(enum_avail != 0) {
        int dev_connected = 0;
  // NOTE(jeff): Defined in file at openal-soft.git/include/AL/alext.h
  #if defined(ALC_EXT_disconnect)
        alcGetIntegerv(nullptr, ALC_CONNECTED, sizeof(int), &dev_connected);
  #endif
        NOM_DUMP(dev_connected); // should be 1 when the extension is available
      }
    }
  }

  // create the audio device handle
  AL_CLEAR_ERR();
  dev = alcOpenDevice(req_audio_dev_name);
  if(dev == nullptr) {
    std::string al_err_str;

    ALenum error = alGetError();
    if(error != AL_NO_ERROR) {
      al_err_str = nom::integer_to_string(error);
    }

    std::string err_str = "Failed to open the audio device; " +
      std::string("OpenAL error code" + al_err_str);
    // Err; memory allocation failure?
    nom::set_error(err_str);
    return impl;
  }

  ctx = audio::init_openal_context(request, dev);

  // IAudioDevice* testme = new AudioDevice();
  // if(NOM_ISA(AudioDevice*, testme) == true) {
    // NOM_DUMP("!");
  // }

  // NOTE(jeff): Enumerate the audio hardware capabilities available to us via
  // the OpenAL API -- these are subject to change upon state jumps between
  // audio contexts.
  ALCint res_num_mono = 0;
  ALCint res_num_stereo = 0;
  // alcGetIntegerv(dev, ALC_FREQUENCY, sizeof(ALCint), &res_frequency);
  alcGetIntegerv(dev, ALC_MONO_SOURCES, sizeof(ALCint), &res_num_mono);
  alcGetIntegerv(dev, ALC_STEREO_SOURCES, sizeof(ALCint), &res_num_stereo);
  // NOM_DUMP(res_frequency);
  NOM_DUMP(res_num_mono);
  NOM_DUMP(res_num_stereo);
#if 0
  ALCint size;
  alcGetIntegerv(dev, ALC_ATTRIBUTES_SIZE, sizeof(ALCint), &size);
  NOM_DUMP(size);

  int attrs[size];
  alcGetIntegerv(dev, ALC_ALL_ATTRIBUTES, size, attrs);
  for(auto attr_index = 0; attr_index != size; ++attr_index) {

     if(attrs[attr_index] == ALC_FREQUENCY) {
       if(attr_index + 1 < size) {
        NOM_DUMP_VAR(NOM, "frequency:", attrs[attr_index+1]);
      }
    } else if(attrs[attr_index] == ALC_REFRESH) {
      if(attr_index + 1 < size) {
        NOM_DUMP_VAR(NOM, "refresh:", attrs[attr_index+1]);
      }
    } else if(attrs[attr_index] == ALC_SYNC) {
      if(attr_index + 1 < size) {
        NOM_DUMP_VAR(NOM, "sync:", attrs[attr_index+1]);
      }
    } else if(attrs[attr_index] == ALC_MONO_SOURCES) {
      if(attr_index + 1 < size) {
        NOM_DUMP_VAR(NOM, "max mono sources:", attrs[attr_index+1]);
      }
    } else if(attrs[attr_index] == ALC_STEREO_SOURCES) {
      if(attr_index + 1 < size) {
        NOM_DUMP_VAR(NOM, "max stereo sources:", attrs[attr_index+1]);
      }
    } else {
      // NOM_DUMP(attrs[attr_index]);
    }
  }
#endif

  std::vector<const char*> enum_extensions = {
    // 8-bit integers
    "AL_FORMAT_MONO8",
    "AL_FORMAT_STEREO8",
    "AL_FORMAT_QUAD8",
    "AL_FORMAT_51CHN8",
    "AL_FORMAT_61CHN8",
    "AL_FORMAT_71CHN8",
    // 16-bit integers
    "AL_FORMAT_MONO16",
    "AL_FORMAT_STEREO16",
    "AL_FORMAT_QUAD16",
    "AL_FORMAT_51CHN16",
    "AL_FORMAT_61CHN16",
    "AL_FORMAT_71CHN16",
    // 32-bit floating-point
    "AL_FORMAT_MONO_FLOAT32",
    "AL_FORMAT_STEREO_FLOAT32",
    "AL_FORMAT_MONO_DOUBLE_EXT",
    "AL_FORMAT_STEREO_DOUBLE_EXT",
    // 32-bit integer
    "AL_FORMAT_QUAD32",
    "AL_FORMAT_51CHN32",
    "AL_FORMAT_61CHN32",
    "AL_FORMAT_71CHN32"
  };

  {
    // bool enum_available = false;
    for(auto itr = enum_extensions.begin();
        itr != enum_extensions.end();
        ++itr)
    {
      if(audio::enum_available(*itr) != 0) {
        NOM_DUMP_VAR(NOM, "available extension:", *itr);
      } else {
        NOM_DUMP_VAR(NOM, "unavailable extension:", *itr);
      }
    } // end for loop
  }

  {
    if(audio::enum_available("AL_FORMAT_MONO8") != 0) {
      driver->capabilities |= CAPS_FORMAT_MONO_S8;
    }

    if(audio::enum_available("AL_FORMAT_STEREO8") != 0) {
      driver->capabilities |= CAPS_FORMAT_STEREO_S8;
    }

    if(audio::enum_available("AL_FORMAT_QUAD8") != 0) {
      driver->capabilities |= CAPS_FORMAT_QUAD_S8;
    }

    if(audio::enum_available("AL_FORMAT_51CHN_S8") != 0) {
      driver->capabilities |= CAPS_FORMAT_51CHN_S8;
    }

    if(audio::enum_available("AL_FORMAT_61CHN_S8") != 0) {
      driver->capabilities |= CAPS_FORMAT_61CHN_S8;
    }

    if(audio::enum_available("AL_FORMAT_71CHN_S8") != 0) {
      driver->capabilities |= CAPS_FORMAT_71CHN_S8;
    }

    if(audio::enum_available("AL_FORMAT_MONO16") != 0) {
      // driver->capabilities |= CAPS_FORMAT_MONO_S16;
      audio::set_cap(&driver->capabilities, CAPS_FORMAT_MONO_S16);
    }

    if(audio::enum_available("AL_FORMAT_STEREO16") != 0) {
      // driver->capabilities |= CAPS_FORMAT_STEREO_S16;
      audio::set_cap(&driver->capabilities, CAPS_FORMAT_STEREO_S16);
    }

    if(audio::enum_available("AL_FORMAT_QUAD16") != 0) {
      driver->capabilities |= CAPS_FORMAT_QUAD_S16;
    }

    if(audio::enum_available("AL_FORMAT_51CHN16") != 0) {
      driver->capabilities |= CAPS_FORMAT_51CHN_S16;
    }

    if(audio::enum_available("AL_FORMAT_61CHN16") != 0) {
      driver->capabilities |= CAPS_FORMAT_61CHN_S16;
    }

    if(audio::enum_available("AL_FORMAT_71CHN16") != 0) {
      driver->capabilities |= CAPS_FORMAT_71CHN_S16;
    }

// #if defined(NOM_USE_LIBSNDFILE)
//     driver->capabilities &= ~CAPS_FORMAT_MONO_S8;
//     driver->capabilities &= ~CAPS_FORMAT_STEREO_S8;
//     driver->capabilities &= ~CAPS_FORMAT_QUAD_S8;
//     driver->capabilities &= ~CAPS_FORMAT_51CHN_S8;
//     driver->capabilities &= ~CAPS_FORMAT_61CHN_S8;
//     driver->capabilities &= ~CAPS_FORMAT_71CHN_S8;

//     driver->capabilities &= ~CAPS_FORMAT_MONO_U8;
//     driver->capabilities &= ~CAPS_FORMAT_STEREO_U8;
//     driver->capabilities &= ~CAPS_FORMAT_QUAD_U8;
//     driver->capabilities &= ~CAPS_FORMAT_51CHN_U8;
//     driver->capabilities &= ~CAPS_FORMAT_61CHN_U8;
//     driver->capabilities &= ~CAPS_FORMAT_71CHN_U8;
// #endif
  }

  {
    bool ext = audio::extension("AL_EXT_float32");
    NOM_DUMP_VAR(NOM, "AL_EXT_float32 extension:", ext);
    {
      auto format = audio::enum_available("AL_FORMAT_MONO_FLOAT32");
      NOM_DUMP_VAR(NOM, "AL_FORMAT_MONO_FLOAT32:", format);
      if(format != 0) {
        driver->capabilities |= CAPS_FORMAT_MONO_FLOAT32;
      }
    }

    {
      auto format = audio::enum_available("AL_FORMAT_STEREO_FLOAT32");
      NOM_DUMP_VAR(NOM, "AL_FORMAT_STEREO_FLOAT32:", format);
      if(format != 0) {
        driver->capabilities |= CAPS_FORMAT_STEREO_FLOAT32;
      }
    }
  }

  {
    bool ext = audio::extension("AL_EXT_DOUBLE");
    NOM_DUMP_VAR(NOM, "AL_EXT_DOUBLE extension:", ext);

    {
      auto format = audio::enum_available("AL_FORMAT_MONO_DOUBLE_EXT");
      NOM_DUMP_VAR(NOM, "AL_FORMAT_MONO_DOUBLE_EXT:", format);
      if(format != 0) {
        driver->capabilities |= CAPS_FORMAT_MONO_FLOAT64;
      }
    }

    {
      auto format = audio::enum_available("AL_FORMAT_STEREO_DOUBLE_EXT");
      NOM_DUMP_VAR(NOM, "AL_FORMAT_STEREO_DOUBLE_EXT:", format);
      if(format != 0) {
        driver->capabilities |= CAPS_FORMAT_STEREO_FLOAT64;
      }
    }
  }

  NOM_DUMP_VAR(NOM, "caps:", driver->capabilities);

  {
    bool ext = audio::extension("AL_EXT_MCFORMATS");
    NOM_DUMP_VAR(NOM, "AL_EXT_MCFORMATS extension:", ext);
  }

  {
    bool ext = audio::extension("AL_LOKI_quadriphonic");
    NOM_DUMP_VAR(NOM, "AL_LOKI_quadriphonic extension:", ext);
  }

  // TODO(jeff): Verify that this enumeration is available on Windows
  // Availability verified: Apple's OpenAL SDK and OpenAL-Soft.
  {
    auto enum_avail = audio::enum_available("ALC_CONNECTED");
    if(enum_avail != 0) {
      int dev_connected = 0;
// NOTE(jeff): Defined in file at openal-soft.git/include/AL/alext.h
#if defined(ALC_EXT_disconnect)
      alcGetIntegerv(dev, ALC_CONNECTED, sizeof(int), &dev_connected);
#endif
      NOM_DUMP(dev_connected); // should be 1 when the extension is available
    }
  }

  // See http://openal.996291.n3.nabble.com/ALC-ENUMERATE-ALL-EXT-and-pluggable-sound-devices-tp5333p5337.html
  {
    auto previous_devices = driver->name;
    // ignore that C string funcs fail on the null-separated list for now.
    const char* current_devices = audio::default_output_device_name(dev);
    if(strcmp(current_devices, previous_devices) != 0) {
      // handle device list change here, if for some reason you need to.
      // free(previous_devices);
      previous_devices = strdup(current_devices);
    }

    NOM_DUMP(previous_devices);
    NOM_DUMP(current_devices);
  }


  // Check for EAX 2.0 support
  {
    bool ext = alIsExtensionPresent("EAX2.0");
    NOM_DUMP_VAR(NOM, "EAX2.0 extension:", ext);
  }
    // if(ext == AL_TRUE) {
      // auto eax_fn = (eax_extension)audio::process_addr("EAXSet");
      // if(eax_fn == NULL) {
        // ext = false;
      // }
    // }
  // }

  {
    bool ext = audio::context_extension("ALC_EXT_EFX", dev);
    NOM_DUMP_VAR(NOM, "ALC_EXT_EFX extension:", ext);
  }

  {
    bool ext = audio::context_extension("ALC_EXT_MAC_OSX", dev);
    NOM_DUMP_VAR(NOM, "ALC_EXT_MAC_OSX extension:", ext);
  }

  {
    int sample_rate = 0;
    int max_mono_sources = 0;
    int max_stereo_sources = 0;
    int max_sources = 0;

    sample_rate = audio::sample_rate(dev);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST,
                  "frequency:", sample_rate);

    max_mono_sources = audio::max_mono_sources(dev);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST,
                  "max_mono_sources:", max_mono_sources);

    max_stereo_sources = audio::max_stereo_sources(dev);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST,
                  "max_stereo_sources:", max_stereo_sources);

    max_sources = audio::max_sources(dev);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST,
                  "max_sources:", max_sources);
  }

  // device_name_list output_devs =
    // audio::output_device_names(dev);
  // driver->enumerate_output_devices = audio::output_device_names;

  // device_name_list input_devs =
    // audio::input_device_names(dev);
  // driver->enumerate_input_devices = audio::input_device_names;

  nom::clear_error();
  *spec = audio::create_openal_attributes(dev);
  if(spec == nullptr) {
    bool error_state = nom::error_state();
    if(error_state == true) {
      auto err = nom::error();
      // TODO(jeff): Use nom::set_error here
      NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, err);
      return impl;
    }
  }

  driver->name = spec->name;
  driver->dev = dev;
  driver->ctx = ctx;
  impl = new ALAudioEngine(driver);

  if(impl == nullptr) {
    nom::set_error("Failed to allocate memory for IOAudioEngine.");
    return impl;
  }

  return impl;
}

// TODO
static IOAudioEngine*
init_openal_input(const audio::AudioSpec* request, audio::AudioSpec* spec)
{
  return nullptr;
}

IOAudioEngine*
init_audio(const audio::AudioSpec* request, audio::AudioSpec* spec)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_VERBOSE);

  IOAudioEngine* driver = nullptr;
  if(request != nullptr && request->engine != nullptr) {
    if(nom::compare_cstr_insensitive(request->engine, "openal") == 0) {
      driver = audio::init_openal_output(request, spec);
    }

    // TODO(jeff): Implement general purpose code for NULL device
    // initialization
  }

  return driver;
}

void shutdown_audio(IOAudioEngine* impl)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);

  if(impl != nullptr && impl->valid() == true) {
    impl->close();
  }

  impl = nullptr;
}

// const char* audio_device_name(IOAudioEngine* target)
// {
//   const char* result = "\0";

//   // if(target != nullptr && target->valid() == true) {
//   {
//     // result = target->device_name();
//   }

//   return result;
// }

int num_audio_devices()
{
  int num_devices = -1;

  //

  return num_devices;
}
#if 0
NOM_IGNORED_VARS();
const char* audio_device(void* target)
{
  const char* result = "\0";
  const char* device_name = nullptr;
  ALAudioDevice* audio_dev = nullptr;

  if(target == nullptr) {
    return result;
  }

  // FIXME(jeff): Implement type polymorphism here
    ALAudioDevice* dev = NOM_SCAST(ALAudioDevice*, target);
#if 0
    if (alcIsExtensionPresent(NULL, "ALC_enumeration_EXT") == AL_TRUE) {

      if (alcIsExtensionPresent(NULL, "ALC_enumerate_all_EXT") == AL_FALSE) {
        s = (char *)alcGetString(NULL, ALC_DEVICE_SPECIFIER);
      } else {
        s = (char *)alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
      }
      displayDevices("output", s);

      s = (char *)alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
      displayDevices("input", s);
    }

    return(s != nullptr);
  }
#endif

  return result;
}
NOM_IGNORED_VARS_ENDL();
#endif
#if 0
real64 device_time(void* target)
{
  real64 result = 0.0f;
  // FIXME(jeff): Implement type polymorphism here
  auto dev = NOM_SCAST(ALAudioDevice*, target);

  if(dev == nullptr) {
    return result;
  }

  // result = HighResolutionTimer::to_seconds(dev->elapsed_ticks_);
  result = Timer::to_seconds(dev->elapsed_ticks_);

  return result;
}
#endif
} // namespace audio
} // namespace nom
