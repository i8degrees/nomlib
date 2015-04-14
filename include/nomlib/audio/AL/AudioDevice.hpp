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
#ifndef NOMLIB_AL_AUDIO_DEVICE_HEADERS
#define NOMLIB_AL_AUDIO_DEVICE_HEADERS

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/audio/IAudioDevice.hpp"

// Forward declarations
struct ALCdevice_struct {};
struct ALCcontext_struct {};
typedef char ALCchar;

namespace nom {
  namespace priv {

/// Custom deleter for freeing an OpenAL audio device; debugging aid.
void AL_FreeAudioDevice ( ALCdevice_struct* );

/// Custom deleter for freeing an OpenAL audio context; debugging aid.
void AL_FreeAudioContext ( ALCcontext_struct* );

  } // namespace priv
} // namespace nom


namespace nom {

class AudioDevice: public IAudioDevice
{
  public:
    /// Default constructor for initializing the default audio device
    AudioDevice ( void );

    /// Constructor variant for initializing a specific audio device
    AudioDevice ( const std::string& device_name );

    virtual ~AudioDevice( void );

    /// Obtain the initialized OpenAL audio device
    // std::shared_ptr<ALCdevice> getAudioDevice ( void ) const;

    /// Obtain the initialized audio device name
    std::string getDeviceName() const override;

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
    std::shared_ptr<ALCdevice_struct> audio_device;
    /// Audio device context
    std::shared_ptr<ALCcontext_struct> audio_context;

    /// device name
    std::string device_name_;
};

} // namespace nom

#endif // NOMLIB_AL_AUDIO_DEVICE_HEADERS defined
