/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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

// Audio playback usage example

#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include <nomlib/audio.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>

int main ( int argc, char* argv[] )
{
  nom::File dir;
  nom::OpenAL::AudioDevice dev; // this must be declared first
  nom::OpenAL::AudioDevice dev2; // this must be declared first

  //ALCdevice* dev = alcOpenDevice ( nullptr );
  //ALCcontext* ctx = alcCreateContext ( dev, nullptr );
  //alcMakeContextCurrent ( ctx );

  nom::OpenAL::Listener listener;
  nom::OpenAL::SoundBuffer buffer;
  nom::Timer loops;
  std::string path = dir.path(argv[0]) + "/";

//NOM_DUMP_VAR ( dev.getDeviceName() );
//NOM_DUMP_VAR ( dev2.getDeviceName() );

  listener.setVolume ( MAX_VOLUME );
NOM_DUMP_VAR ( listener.getVolume() );

  bool ret = false;
  if ( argv[1] != nullptr )
  {
    ret = buffer.load ( argv[1] );
  }
  else
  {

// FIXME
// This is a temporary fix until we get around to creating proper Resource 
// folders for each of our examples.
// Jeffrey Carpenter <jeffrey.carp@gmail.com> @ 2013-10-01
#if defined ( NOM_PLATFORM_WINDOWS )
  ret = buffer.load ( "cursor_wrong.wav" );
#else // assume POSIX platform
  ret = buffer.load ( path + "Resources/cursor_wrong.wav" );
#endif
    //ret = buffer.load ( "/Users/jeff/Projects/hax/ttcards.git/ttcards/Resources/audio/15_ShuffleOrBoogie.ogg" );
  }

  if ( ! ret )
  {
NOM_LOG_ERR ( NOM, "Buffer loading err" );
    return EXIT_FAILURE;
  }

  //nom::OpenAL::Sound snd ( buffer );
  nom::OpenAL::Music snd ( buffer );

  snd.setPitch ( 1.0 );
  snd.setVolume ( MAX_VOLUME );
  snd.setPosition ( nom::Point3f ( 0.0, 0.0, 0.0 ) );
  snd.setVelocity ( nom::Point3f ( 0.0, 0.0, 0.0 ) );
  snd.setLooping ( true );

  snd.setPlayPosition ( 8.0 );

  if ( snd.getStatus() != nom::SoundStatus::Playing )
    snd.Play();

  nom::uint32 duration = buffer.getDuration();
  float duration_seconds = duration / 1000.0f;
NOM_DUMP_VAR ( duration_seconds );

  loops.start();

  //float step = 1.0;
  // volume / seconds = step

  //float step = snd.getVolume();
  //float step_by = step / 4; // 4s or 4000ms

  float pos = snd.getPlayPosition();

    snd.fadeOut ( 4 );

  while ( ( loops.ticks() <= duration * 2 ) && ( snd.getStatus() != nom::SoundStatus::Paused && snd.getStatus() != nom::SoundStatus::Stopped ) )
  {
    // 0.455*2/4
    // ( duration * total_loops ) / milliseconds*2 where seconds is desired fade
    // out (over time)
    // duration / milliseconds*2 where milliseconds is desired fade out
    // ( over time)

    if ( snd.getPlayPosition() >= 1.0 )
    {
      // ...
    }

    if ( snd.getPlayPosition() >= ( pos + 2.5 ) )
    {
      // ...
    }
  }

  loops.stop();
NOM_DUMP_VAR ( loops.ticks() );

  //alcMakeContextCurrent ( nullptr );
  //alcDestroyContext ( ctx );
  //alcCloseDevice ( dev );

  //std::cout << "Sample Count: " << sndfile.getSampleCount() << std::endl;
  //std::cout << "Channel Count: " << sndfile.getChannelCount() << std::endl;
  //std::cout << "Sample Rate: " << sndfile.getSampleRate() << std::endl;

  return EXIT_SUCCESS;
}
