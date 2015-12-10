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
#include "nomlib/audio/NullAudioDevice.hpp"

// FIXME:
#include "AL/ALAudioDeviceCaps.hpp"

namespace nom {

NullAudioDevice::NullAudioDevice()
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  this->impl_ = new NullAudioEngine();
}

NullAudioDevice::~NullAudioDevice()
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );
  this->free_device();
}

void NullAudioDevice::free_device()
{
  NOM_DELETE_PTR(this->impl_);
}

bool NullAudioDevice::valid() const
{
  return false;
}

bool NullAudioDevice::initialize(const AudioSpec* spec)
{
  return true;
}

IOAudioEngine* NullAudioDevice::caps()
{
  return this->impl_;
}

void NullAudioDevice::set_caps(IOAudioEngine* caps)
{
  this->impl_ = caps;
}

std::string NullAudioDevice::device_name() const
{
  return "NullAudioDevice";
}

// bool NullAudioDevice::isExtensionSupported( const std::string& extension ) const
// {
//   return false;
// }

} // namespace nom
