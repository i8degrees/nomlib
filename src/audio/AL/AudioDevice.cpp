/******************************************************************************

    OpenAL Audio Device

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "AudioDevice.hpp"

namespace nom {
  namespace priv {

void AL_FreeAudioDevice ( ALCdevice* dev )
{
NOMLIB_LOG_INFO;

  if ( dev != nullptr )
  {
    alcCloseDevice ( dev );
  }
}

void AL_FreeAudioContext ( ALCcontext* ctx )
{
NOMLIB_LOG_INFO;

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
  namespace OpenAL {

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

AudioDevice::~AudioDevice ( void )
{
NOMLIB_LOG_INFO;
}

bool AudioDevice::initialize ( const ALCchar* device_name )
{
NOMLIB_LOG_INFO;

  audio_device.reset();
  audio_context.reset();

  // audio device handle
  this->audio_device = std::shared_ptr<ALCdevice> ( alcOpenDevice ( device_name ), nom::priv::AL_FreeAudioDevice );

  // attach a context (think: listener) to device
  if ( this->audio_device )
  {
    // Store the audio device name now that it is confirmed valid
    this->device_name = alcGetString ( this->audio_device.get(), ALC_DEFAULT_DEVICE_SPECIFIER );

    this->audio_context = std::shared_ptr<ALCcontext> ( alcCreateContext ( this->audio_device.get(), nullptr ), nom::priv::AL_FreeAudioContext );

    if ( this->audio_context )
    {
      alcMakeContextCurrent ( this->audio_context.get() );
    }
    else
    {
NOMLIB_LOG_ERR ( "Failed to create the audio context." );
      return false;
    }
  }
  else
  {
NOMLIB_LOG_ERR ( "Failed to open the audio device." );
    return false;
  }

  this->audio_initialized = true;

  return true;
}

bool AudioDevice::initialized ( void ) const
{
  if ( this->audio_initialized )
    return true;
  else
    return false;
}

std::shared_ptr<ALCdevice> AudioDevice::getAudioDevice ( void ) const
{
  return this->audio_device;
}

const std::string AudioDevice::getDeviceName ( void ) const
{
  return this->device_name;
}

bool AudioDevice::isExtensionSupported ( const std::string& extension ) const
{
  if ( ( extension.length() > 2 ) && ( extension.substr ( 0, 3 ) == "ALC" ) )
  {
    if ( alcIsExtensionPresent ( this->audio_device.get(), extension.c_str() ) != AL_FALSE )
      return true;
  }
  else
  {
    if ( alIsExtensionPresent ( extension.c_str() ) != AL_FALSE )
      return true;
  }

  return false;
}


  } // namespace OpenAL
} // namespace nom
