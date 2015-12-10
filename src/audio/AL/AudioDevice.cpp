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
#include "nomlib/audio/AL/AudioDevice.hpp"

// FIXME:
#include "nomlib/audio/NullAudioDevice.hpp"

#include "ALAudioDeviceCaps.hpp"

// Forward declarations
#include "nomlib/audio/AL/OpenAL.hpp"

namespace nom {
  namespace priv {

void AL_FreeAudioDevice(ALCdevice* dev)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  if( dev != nullptr) {
    alcCloseDevice(dev);
  }

  dev = nullptr;
}

void AL_FreeAudioContext(ALCcontext* ctx)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  if( ctx != nullptr ) {
    // Disable context
    alcMakeContextCurrent(nullptr);

    // Release context
    alcDestroyContext(ctx);
  }

  ctx = nullptr;
}

  } // namespace priv
} // namespace nom

namespace nom {

// Static initializations
// bool ALAudioDevice::audio_initialized = false;

ALAudioDevice::ALAudioDevice()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);
}

ALAudioDevice::~ALAudioDevice()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  if( this->initialized() ) {
    this->free_device();
  }
}

void ALAudioDevice::free_device()
{
  // NOM_LOG_DEBUG(NOM_LOG_PRIORITY_DEBUG, "Audio device released");
  NOM_LOG_INFO(NOM, "Audio device released");

  nom::priv::AL_FreeAudioContext(this->audio_context);
  nom::priv::AL_FreeAudioDevice(this->audio_device);

  this->audio_initialized_ = false;
}

void* ALAudioDevice::device()
{
  return( NOM_SCAST(ALCdevice*, this->audio_device) );
}

bool ALAudioDevice::valid() const
{
  return( this->audio_device && this->audio_context &&
          this->initialized() == true );
}

IOAudioEngine* ALAudioDevice::caps() // const
{
  return this->impl_;
}

void ALAudioDevice::set_caps(IOAudioEngine* caps)
{
  this->impl_ = caps;
}

// bool ALAudioDevice::initialize(const ALCchar* device_name)
bool ALAudioDevice::initialize(const AudioSpec* spec)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  if( spec != nullptr ) {
    this->device_name_ = spec->device_name;
  }

  // audio device handle
  AL_CLEAR_ERR();
  this->audio_device = alcOpenDevice(this->device_name_.c_str());
  AL_CHECK_ERR_VOID();

  if( this->audio_device != nullptr ) {

    // NOTE: Ensure that we obtain a device name!
    if( this->device_name_.length() < 1 ) {
      AL_CLEAR_ERR();
      this->device_name_ =
        alcGetString(this->audio_device, ALC_DEFAULT_DEVICE_SPECIFIER);
      AL_CHECK_ERR_VOID();
    }

    // NOTE: attach a context (think: listener) to device
    AL_CLEAR_ERR();
    this->audio_context =
      alcCreateContext(this->audio_device, nullptr);
    AL_CHECK_ERR_VOID();

    if( this->audio_context != nullptr ) {
      AL_CLEAR_ERR();
      alcMakeContextCurrent(this->audio_context);
      AL_CHECK_ERR_VOID();
    } else {
      // Err; memory allocation failure!
      NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                    "Failed to create the audio context." );
      return false;
    }
  } else if( this->audio_device == nullptr ) {
    // Err; memory allocation failure!
    NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION,
                  "Failed to open the audio device." );
    return false;
  }

  this->impl_ = new ALAudioEngine();

  if( this->impl_ != nullptr ) {
    this->audio_initialized_ = true;
  }

  return(this->audio_initialized_ == true);
  // return this->valid();
}

bool ALAudioDevice::initialized() const
{
  return(this->audio_initialized_);
}

std::string ALAudioDevice::device_name() const
{
  return this->device_name_;
}

// bool AudioDevice::isExtensionSupported ( const std::string& extension ) const
// {
//   bool result = false;

//   AL_CLEAR_ERR();

//   if( ( extension.length() > 2 ) && ( extension.substr ( 0, 3 ) == "ALC" ) ) {

//     if( this->audio_device != nullptr ) {
//       result = alcIsExtensionPresent(this->audio_device, extension.c_str() );
//     }
//   } else {
//     result = alIsExtensionPresent( extension.c_str() );
//   }

//   return result;
// }

bool extension_available(const std::string& ext, IAudioDevice* dev)
{
  AL_CLEAR_ERR();

  if( ext.length() > 2 && ext.substr(0, 3) == "ALC" ) {
    if( alcIsExtensionPresent( NULL, ext.c_str() ) != AL_FALSE ) {
      AL_CHECK_ERR_VOID();
      return true;
    }
  } else {
    if( alIsExtensionPresent( ext.c_str() ) != AL_FALSE ) {
      AL_CHECK_ERR_VOID();
      return true;
    }
  }

  AL_CHECK_ERR_VOID();
  return false;
}

static void
displayDevices(const char *type, const char *list)
{
  ALCchar *ptr, *nptr;

  ptr = (ALCchar *)list;
  printf("list of all available %s devices:\n", type);
  if (!list)
  {
    printf("none\n");
  }
  else
  {
    nptr = ptr;
    while (*(nptr += strlen(ptr)+1) != 0)
    {
      printf("  %s\n", ptr);
      ptr = nptr;
    }
    printf("  %s\n", ptr);
  }
}

bool extension_availableT(const std::string& ext, IAudioDevice* dev)
{
  char *s = nullptr;

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

IAudioDevice* create_audio_device(const AudioSpec* spec)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  auto dev = new ALAudioDevice();

  if( dev != nullptr ) {
    dev->initialize(spec);
    dev->set_caps( new ALAudioEngine() );
  }

  return dev;
}
#if 0
IAudioDevice* create_null_audio_device(const AudioSpec* spec)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  // NullAudioDevice* dev = new NullAudioDevice();

  // if( dev != nullptr ) {
    // dev->initialize(spec);
    // dev->set_caps( new NullAudioEngine() );
  // }

  // device->audio_initialized = true;

  return dev;
}
#endif

void shutdown_audio(IAudioDevice* dev)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  if( dev != nullptr ) {
    dev->free_device();
  }

  // nom::priv::AL_FreeAudioContext(dev->audio_context);
  // nom::priv::AL_FreeAudioDevice(dev->audio_device);
}

std::string
audio_device_name(IAudioDevice* dev)
{
  std::string result = "\0";
  if( dev != nullptr ) {
    result = dev->device_name();
  }

  return result;
}

} // namespace nom
