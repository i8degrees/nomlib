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
#ifndef NOMLIB_AUDIO_HEADERS
#define NOMLIB_AUDIO_HEADERS

// Public header file

#include <nomlib/config.hpp>

#include "nomlib/audio/IAudioDevice.hpp"
#include "nomlib/audio/IListener.hpp"
#include "nomlib/audio/ISoundBuffer.hpp"
#include "nomlib/audio/ISoundSource.hpp"
#include "nomlib/audio/NullAudioDevice.hpp"
#include "nomlib/audio/NullListener.hpp"
#include "nomlib/audio/NullSoundBuffer.hpp"
#include "nomlib/audio/NullSoundSource.hpp"
#include "nomlib/audio/NullSound.hpp"
#include "nomlib/audio/NullMusic.hpp"
#include "nomlib/audio/AudioDeviceLocator.hpp"

#if defined(NOM_USE_OPENAL)
  #include "nomlib/audio/AL/AudioDevice.hpp"
  #include "nomlib/audio/AL/Listener.hpp"
  #include "nomlib/audio/AL/Music.hpp"
  #include "nomlib/audio/AL/Sound.hpp"
  #include "nomlib/audio/AL/SoundBuffer.hpp"
  #include "nomlib/audio/AL/SoundFile.hpp"
  #include "nomlib/audio/AL/SoundSource.hpp"
#endif

#endif // include guard defined
