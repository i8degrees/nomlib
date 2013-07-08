/******************************************************************************

    OpenAL Audio Device

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_OPENAL_AUDIO_DEVICE_HEADERS
#define NOMLIB_OPENAL_AUDIO_DEVICE_HEADERS

#include <iostream>
#include <memory>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "nomlib_config.hpp"

namespace nom {

class OpenAL_AudioDevice
{
  public:
    OpenAL_AudioDevice ( void );
    ~OpenAL_AudioDevice ( void );

    const std::string getDeviceName ( void ) const;

  private:
    /// Audio device handle
    std::shared_ptr<ALCdevice> audio_device;
    /// Audio device context
    std::shared_ptr<ALCcontext> audio_context;
    /// device name
    const ALCchar *device_name;
};

} // namespace nom

#endif // NOMLIB_OPENAL_AUDIO_DEVICE_HEADERS defined
