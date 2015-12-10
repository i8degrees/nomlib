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
#ifndef NOMLIB_AUDIO_NULL_AUDIO_DEVICE_HPP
#define NOMLIB_AUDIO_NULL_AUDIO_DEVICE_HPP

#include <string>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/audio/IAudioDevice.hpp"

namespace nom {

// Forward declarations
// struct SoundBuffer;
class IOAudioEngine;

// void null_set_audio_volume(SoundBuffer* target, real32 gain);

class NullAudioDevice: public IAudioDevice
{
  public:
    NullAudioDevice();
    virtual ~NullAudioDevice();

    virtual void free_device() override;
    virtual bool valid() const override;
    virtual bool initialize(const AudioSpec* spec) override;

    virtual IOAudioEngine* caps() override;
    virtual void set_caps(IOAudioEngine* caps) override;

    // std::shared_ptr<ALCdevice> getAudioDevice( void ) const;
    std::string device_name() const override;
    // bool isExtensionSupported( const std::string& extension ) const override;

  private:
    IOAudioEngine* impl_ = nullptr;
};

} // namespace nom

#endif // include guard defined
