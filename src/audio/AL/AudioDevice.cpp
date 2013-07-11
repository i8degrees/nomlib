/******************************************************************************

    OpenAL Audio Device

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "AudioDevice.hpp"

namespace nom {
  namespace OpenAL {

AudioDevice::AudioDevice ( void )
{
NOMLIB_LOG_INFO;

  // audio device handle
  this->audio_device = std::shared_ptr<ALCdevice> ( alcOpenDevice ( nullptr ), alcCloseDevice );

  // attach a context (think: listener) to device
  if ( this->audio_device )
  {
    // Store the audio device name now that it is confirmed valid
    this->device_name = alcGetString ( this->audio_device.get(), ALC_DEFAULT_DEVICE_SPECIFIER );

    this->audio_context = std::shared_ptr<ALCcontext> ( alcCreateContext ( this->audio_device.get(), nullptr ), alcDestroyContext );

    if ( this->audio_context )
    {
      alcMakeContextCurrent ( this->audio_context.get() );
    }
    else
    {
NOMLIB_LOG_ERR ( "Failed to create the audio context." );
    }
  }
  else
  {
NOMLIB_LOG_ERR ( "Failed to open the audio device." );
  }
  alcSuspendContext( audio_context.get() );

  // Initialize Listener properties here
}

AudioDevice::~AudioDevice ( void )
{
NOMLIB_LOG_INFO;

  //alcMakeContextCurrent ( nullptr );

  // Clean up instance variables
}

std::shared_ptr<ALCdevice> AudioDevice::getAudioDevice ( void ) const
{
  return this->audio_device;
}

const std::string AudioDevice::getDeviceName ( void ) const
{
  return this->device_name;
}


  } // namespace OpenAL
} // namespace nom
