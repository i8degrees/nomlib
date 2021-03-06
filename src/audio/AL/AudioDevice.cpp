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

// Forward declarations
#include "nomlib/audio/AL/OpenAL.hpp"

namespace nom {
  namespace priv {

void AL_FreeAudioDevice ( ALCdevice* dev )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  if ( dev != nullptr )
  {
    alcCloseDevice ( dev );
  }
}

void AL_FreeAudioContext ( ALCcontext* ctx )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  if ( ctx != nullptr )
  {
    // Disable context
    alcMakeContextCurrent ( nullptr );

    // Release context
    alcDestroyContext ( ctx );
  }
}


  } // namespace priv
} // namespace nom

namespace nom {

// Static initializations
bool AudioDevice::audio_initialized = false;

AudioDevice::AudioDevice ( void )
{
  if ( ! this->initialized() )
    initialize ( nullptr );
}

AudioDevice::AudioDevice ( const std::string& device_name )
{
  if ( ! this->initialized() )
    initialize ( device_name.c_str() );
}

AudioDevice::~AudioDevice( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );
}

bool AudioDevice::initialize ( const ALCchar* device_name )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  AL_CLEAR_ERR();

  // audio device handle
  this->audio_device = std::shared_ptr<ALCdevice> ( alcOpenDevice ( device_name ), nom::priv::AL_FreeAudioDevice );
  AL_CHECK_ERR_VOID();

  // attach a context (think: listener) to device
  if( this->audio_device != nullptr ) {

    // Store the audio device name now that it is confirmed valid
    if( device_name != nullptr ) {
      this->device_name_ = device_name;
    } else {
      AL_CLEAR_ERR();
      this->device_name_ =
        alcGetString(this->audio_device.get(), ALC_DEFAULT_DEVICE_SPECIFIER);
      AL_CHECK_ERR_VOID();
    }

    AL_CLEAR_ERR();
    this->audio_context = std::shared_ptr<ALCcontext> ( alcCreateContext ( this->audio_device.get(), nullptr ), nom::priv::AL_FreeAudioContext );
    AL_CHECK_ERR_VOID();

    if( this->audio_context != nullptr ) {
      AL_CLEAR_ERR();
      alcMakeContextCurrent( this->audio_context.get() );
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

  this->audio_initialized = true;

  return true;
}

bool AudioDevice::initialized ( void ) const
{
  // if ( this->audio_initialized )
    // return true;
  // else
    return false;
}

// std::shared_ptr<ALCdevice> AudioDevice::getAudioDevice ( void ) const
// {
//   return this->audio_device;
// }

std::string AudioDevice::getDeviceName() const
{
  return this->device_name_;
}

bool AudioDevice::isExtensionSupported ( const std::string& extension ) const
{
  AL_CLEAR_ERR();

  if( ( extension.length() > 2 ) && ( extension.substr ( 0, 3 ) == "ALC" ) ) {

    if( alcIsExtensionPresent ( this->audio_device.get(), extension.c_str() ) != AL_FALSE )
    {
      return true;
    }
  }
  else
  {
    if ( alIsExtensionPresent ( extension.c_str() ) != AL_FALSE )
      return true;
  }

  return false;
}

bool AudioDevice::extension_available(const std::string& ext)
{
  AL_CLEAR_ERR();

  if( ext.length() > 2 && ext.substr(0, 3) == "ALC" ) {
    if( alcIsExtensionPresent( NULL, ext.c_str() ) != AL_FALSE ) {
      return true;
    }
  } else {
    if( alIsExtensionPresent( ext.c_str() ) != AL_FALSE ) {
      return true;
    }
  }

  return false;
}

} // namespace nom
