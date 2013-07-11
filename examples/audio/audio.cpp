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
  nom::OpenAL::Listener listener;
  nom::OpenAL::SoundBuffer buffer;
  nom::Timer loops;

  std::string path = nom::OSXFS::getDirName ( argv[0] ) + "/";

  dump_var ( dev.getDeviceName() );

  listener.setMasterVolume ( 1.0 );
  dump_var ( listener.getMasterVolume() );

  if ( argv[1] != nullptr )
  {
    buffer.loadFromFile ( argv[1] );
  }
  else
  {
    buffer.loadFromFile ( path + "data/cursor_wrong.wav" );
  }

  ALuint source;
  alGenSources ( 1, &source );

  assert ( source != AL_INVALID_VALUE );

  ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };
  ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };

  alSourcei ( source, AL_BUFFER, buffer.get() );
  alSourcef ( source, AL_PITCH, 1.0 );
  alSourcef ( source, AL_GAIN, 1.00 );
  alSourcefv ( source, AL_POSITION, sourcePos );
  alSourcefv ( source, AL_VELOCITY, sourceVel );
  alSourcei ( source, AL_LOOPING, AL_TRUE );

  nom::uint32 duration = buffer.getDuration();
  float duration_seconds = duration / 1000.0f;
  dump_var ( duration_seconds );

  loops.Start();

  while ( loops.getTicks() <= duration * 2 )
  {
    ALint state;
    alGetSourcei ( source, AL_SOURCE_STATE, &state );

    if ( state != AL_PLAYING )
      alSourcePlay ( source );
  }

  loops.Stop();
  dump_var ( loops.getTicks() );

  assert ( alGetError() == AL_NO_ERROR );

  //std::cout << "Sample Count: " << sndfile.getSampleCount() << std::endl;
  //std::cout << "Channel Count: " << sndfile.getChannelCount() << std::endl;
  //std::cout << "Sample Rate: " << sndfile.getSampleRate() << std::endl;

  return EXIT_SUCCESS;
}
