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

/// \brief Audio playback usage example

#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include <nomlib/audio.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>

/// Name of our application.
const std::string APP_NAME = "Audio Playback";

/// File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources";

/// Relative file path name of our resource example
const nom::Path p;

/// Sound effect resource file
const std::string RESOURCE_AUDIO_SOUND = APP_RESOURCES_DIR + p.native() + "cursor_wrong.wav";

int main ( int argc, char* argv[] )
{
  nom::OpenAL::AudioDevice dev; // this must be declared first
  nom::OpenAL::Listener listener; // Global audio volume control
  nom::OpenAL::SoundBuffer buffer;
  nom::Timer loops;

  // Fatal error; if we are not able to complete this step, it means that
  // we probably cannot rely on our resource paths!
  if ( nom::init ( argc, argv ) == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not initialize nomlib." );
    exit ( NOM_EXIT_FAILURE );
  }
  atexit(nom::quit);

  NOM_DUMP_VAR ( dev.getDeviceName() );

  listener.setVolume ( MAX_VOLUME );
  bool ret = false;
  if ( argv[1] != nullptr )
  {
    ret = buffer.load ( argv[1] );
  }
  else
  {
    ret = buffer.load ( RESOURCE_AUDIO_SOUND );
  }

  if ( ! ret )
  {
    NOM_LOG_ERR ( NOM, "Buffer loading err" );
    return NOM_EXIT_FAILURE;
  }

  //nom::OpenAL::Sound snd ( buffer );
  nom::OpenAL::Music snd ( buffer );

  snd.setPitch ( 1.0 );
  snd.setVolume ( MAX_VOLUME );
  snd.setPosition ( nom::Point3f ( 0.0, 0.0, 0.0 ) );
  snd.setVelocity ( nom::Point3f ( 0.0, 0.0, 0.0 ) );
  snd.setLooping ( true );

  if ( snd.getStatus() != nom::SoundStatus::Playing ) snd.Play();

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

  //std::cout << "Sample Count: " << snd.getSampleCount() << std::endl;
  //std::cout << "Channel Count: " << snd.getChannelCount() << std::endl;
  //std::cout << "Sample Rate: " << snd.getSampleRate() << std::endl;

  return NOM_EXIT_SUCCESS;
}
