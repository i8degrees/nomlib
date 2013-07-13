/******************************************************************************

    nomlib Audio Playback Demo

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include "audio.hpp"
#include "math.hpp"
#include "system.hpp"

#define dump_var(var) NOMLIB_DUMP_VAR(var)

int main ( int argc, char* argv[] )
{
  nom::OpenAL::AudioDevice dev; // this must be declared first
  nom::OpenAL::AudioDevice dev2; // this must be declared first

  //ALCdevice* dev = alcOpenDevice ( nullptr );
  //ALCcontext* ctx = alcCreateContext ( dev, nullptr );
  //alcMakeContextCurrent ( ctx );

  nom::OpenAL::Listener listener;
  nom::OpenAL::SoundBuffer buffer;
  nom::Timer loops;
  std::string path = nom::OSXFS::getDirName(argv[0]) + "/";

  dump_var ( dev.getDeviceName() );
  dump_var ( dev2.getDeviceName() );

  listener.setVolume ( MAX_VOLUME );
  dump_var ( listener.getVolume() );

  bool ret = false;
  if ( argv[1] != nullptr )
  {
    ret = buffer.loadFromFile ( argv[1] );
  }
  else
  {
    ret = buffer.loadFromFile ( path + "data/cursor_wrong.wav" );
    //ret = buffer.loadFromFile ( "/Users/jeff/Projects/hax/ttcards.git/ttcards/data/audio/15_ShuffleOrBoogie.ogg" );
  }

  if ( ! ret )
  {
NOMLIB_LOG_ERR ( "Buffer loading err" );
    return EXIT_FAILURE;
  }

  //nom::OpenAL::Sound snd ( buffer );
  nom::OpenAL::Music snd ( buffer );

  snd.setPitch ( 1.0 );
  snd.setVolume ( MAX_VOLUME );
  snd.setPosition ( nom::Vector3f ( 0.0, 0.0, 0.0 ) );
  snd.setVelocity ( nom::Vector3f ( 0.0, 0.0, 0.0 ) );
  snd.setLooping ( true );

  snd.setPlayPosition ( 8.0 );

  if ( snd.getStatus() != nom::SoundStatus::Playing )
    snd.Play();

  nom::uint32 duration = buffer.getDuration();
  float duration_seconds = duration / 1000.0f;
  dump_var ( duration_seconds );

  loops.Start();

  //float step = 1.0;
  // volume / seconds = step

  float step = snd.getVolume();
  float step_by = step / 4; // 4s or 4000ms

  float pos = snd.getPlayPosition();

  snd.fadeOut ( 4 );

  while ( ( loops.getTicks() <= duration * 2 ) && ( snd.getStatus() != nom::SoundStatus::Paused && snd.getStatus() != nom::SoundStatus::Stopped ) )
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

  loops.Stop();
  dump_var ( loops.getTicks() );

  //alcMakeContextCurrent ( nullptr );
  //alcDestroyContext ( ctx );
  //alcCloseDevice ( dev );

  //std::cout << "Sample Count: " << sndfile.getSampleCount() << std::endl;
  //std::cout << "Channel Count: " << sndfile.getChannelCount() << std::endl;
  //std::cout << "Sample Rate: " << sndfile.getSampleRate() << std::endl;

  return EXIT_SUCCESS;
}
