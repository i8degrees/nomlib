/******************************************************************************

    OpenAL Audio Device

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_AUDIO_DEVICE_HEADERS
#define NOMLIB_AL_AUDIO_DEVICE_HEADERS

#include <iostream>
#include <memory>

#include "config.hpp"
#include "sys/Logger.hpp"
#include "OpenAL.hpp"
#include "sdl/utils.hpp"

namespace nom {
  namespace OpenAL {

class AudioDevice
{
  public:
    /// Default constructor for initializing the default audio device
    AudioDevice ( void );

    /// Constructor variant for initializing a specific audio device
    AudioDevice ( const std::string& device_name );

    ~AudioDevice ( void );

    /// Obtain the initialized OpenAL audio device
    std::shared_ptr<ALCdevice> getAudioDevice ( void ) const;

    /// Obtain the initialized audio device name
    const std::string getDeviceName ( void ) const;

    /// Obtain support info regarding a particular extension
    bool isExtensionSupported ( const std::string& extension ) const;

    // frequency
    // Suspend context
    // Resume context

  private:
    /// This keeps OpenAL from all sorts of odd crashes by only allowing
    /// initialization to occur once
    static bool audio_initialized;
    bool initialize ( const ALCchar* device_name );
    /// ...
    bool initialized ( void ) const;
    /// Audio device handle
    std::shared_ptr<ALCdevice> audio_device;
    /// Audio device context
    std::shared_ptr<ALCcontext> audio_context;
    /// device name
    const ALCchar *device_name;
};

  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_AUDIO_DEVICE_HEADERS defined
