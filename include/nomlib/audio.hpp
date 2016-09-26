/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015, 2016 Jeffrey Carpenter <i8degrees@gmail.com>
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

#include <nomlib/audio/audio_defs.hpp>

#include <nomlib/audio/ISoundFileReader.hpp>
#include <nomlib/audio/SoundBuffer.hpp>
#include <nomlib/audio/SoundFile.hpp>

#if defined(NOM_USE_OPENAL)
  // #include <nomlib/audio/AL/OpenAL.hpp>
  #include <nomlib/audio/AL/ALAudioDevice.hpp>
  #include <nomlib/audio/AL/ALAudioDeviceCaps.hpp>
  #include <nomlib/audio/AL/SoundSource.hpp>
#endif

// TODO(jeff): Setup conditional compilation unit for this back-end in time to
// come..?
#if defined(NOM_USE_LIBSNDFILE)
  #include <nomlib/audio/libsndfile/SoundFileReader.hpp>
  #include <nomlib/audio/libsndfile/SoundFileWriter.hpp>
#endif

#endif // include guard defined
