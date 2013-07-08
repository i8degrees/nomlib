/******************************************************************************

    OpenAL Audio Device

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "OpenAL_AudioDevice.hpp"

namespace nom {

OpenAL_AudioDevice::OpenAL_AudioDevice ( void )
{
NOMLIB_LOG_INFO;

  // audio device handle
  this->audio_device = std::shared_ptr<ALCdevice> ( alcOpenDevice ( nullptr ), &alcCloseDevice );

  // attach a context (think: listener) to device
  if ( this->audio_device )
  {
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
}

OpenAL_AudioDevice::~OpenAL_AudioDevice ( void )
{
NOMLIB_LOG_INFO;

  // Clean up instance variables
}

const std::string OpenAL_AudioDevice::getDeviceName ( void ) const
{
  return this->device_name;
}


} // namespace nom
