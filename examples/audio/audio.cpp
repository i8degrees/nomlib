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

// NOTE: Audio playback usage example

#include <nomlib/audio.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/version.hpp>

#include "tclap/CmdLine.h"

#include <thread>

using namespace nom;

const std::string APP_NAME = "nomlib: audio";

/// File path of the resources directory; this must be a relative to the parent
/// working directory.
const std::string APP_RESOURCES_DIR = "Resources";

/// The platform specific file delimiter; '/' on Posix and '\' on Windows.
const nom::Path p;

/// Sound effect resource file
const std::string RESOURCE_AUDIO_SOUND = APP_RESOURCES_DIR + p.native() +
  "cursor_wrong.wav";

/// \remarks See program usage by passing --help
struct AppFlags
{
  /// The input file source to play
  std::string audio_input = RESOURCE_AUDIO_SOUND;

  /// Test input source with the null audio back-end
  bool use_null_interface = false;

  /// Test input source with the music audio interface
  ///
  /// \fixme This interface is broken; the sound buffer memory is not properly
  /// deallocated.
  bool use_music_interface = false;

  real32 audio_volume = 100.0f;
};

int parse_cmdline(int argument_count, char* arguments[], AppFlags& opts)
{
  using namespace TCLAP;

  if( argument_count < 0 ) {
    return NOM_EXIT_FAILURE;
  }

  try
  {
    CmdLine cmd( APP_NAME, ' ', nom::NOM_VERSION.version_string() );

    std::string null_interface_desc =
      "Test the usage of the null interface (defaults to FALSE).";
    std::string music_interface_desc =
      "Test the usage of the music interface (defaults to FALSE).";

    SwitchArg use_null_interface_arg("n", "use-null", null_interface_desc,
                                     cmd, false);
    SwitchArg use_music_interface_arg("", "use-music", music_interface_desc,
                                      cmd, false);

    ValueArg<real32> audio_volume_arg("v", "volume",
                                         "Gain level of audio playback",
                                         false, opts.audio_volume,
                                         "A number between 0.0f .. 100.0f",
                                         cmd );
    ValueArg<std::string> audio_file_arg("i", "input",
                                         "File path to audio to play from",
                                         false, opts.audio_input,
                                         "Resources/audio/hello.wav", cmd );

    cmd.parse(argument_count, arguments);

    opts.use_null_interface = use_null_interface_arg.getValue();
    opts.use_music_interface = use_music_interface_arg.getValue();
    opts.audio_input = audio_file_arg.getValue();
    opts.audio_volume = audio_volume_arg.getValue();
  }
  catch(TCLAP::ArgException &e)
  {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  e.error(), "for arg", e.argId() );

    return NOM_EXIT_FAILURE;
  }

  return NOM_EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
  AppFlags args;

  nom::IAudioDevice* dev = nullptr; // this must be declared first
  nom::IListener* listener = nullptr; // Global audio volume control
  nom::SoundBuffer* buffer = nullptr;
  nom::Timer loops;

  nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_AUDIO,
                                        NOM_LOG_PRIORITY_DEBUG);
  nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TRACE_AUDIO,
                                        NOM_LOG_PRIORITY_DEBUG);

  if( parse_cmdline(argc, argv, args) != 0 ) {
    exit(NOM_EXIT_FAILURE);
  }

  // Fatal error; if we are not able to complete this step, it means that
  // we probably cannot rely on our resource paths!
  if( nom::init(argc, argv) == false ) {
    NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION,
                 "Could not initialize nomlib.");
    exit(NOM_EXIT_FAILURE);
  }
  atexit(nom::quit);

  // Quick and dirty method of testing the use of nomlib's audio subsystem
  // #undef NOM_USE_OPENAL

  // Initialize audio subsystem...
  if( args.use_null_interface == false ) {
    dev = nom::create_audio_device(nullptr);
    listener = new nom::Listener();
    buffer = nom::create_audio_buffer();
  }

  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "Audio device name:",
               nom::audio_device_name(dev) );

  // listener->set_volume(args.audio_volume);
  nom::set_audio_volume(dev, args.audio_volume);

  buffer = nom::create_audio_buffer(args.audio_input, dev);
  if( buffer == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION, "Could not load audio file: ",
                  RESOURCE_AUDIO_SOUND );
    return NOM_EXIT_FAILURE;
  }

  if( args.use_music_interface == true ) {
    // ...
  } else if( args.use_music_interface == false ) {
    // ...
  }

  nom::set_audio_volume(buffer, dev, args.audio_volume);
  nom::set_audio_pitch(buffer, dev, 1.00f);
  nom::set_audio_position(buffer, dev, nom::Point3f(0.0f, 0.0f, 0.0f) );
  nom::set_audio_velocity( buffer, dev, nom::Point3f(0.0f, 0.0f, 0.0f) );
  nom::set_audio_state(buffer, dev, AUDIO_STATE_LOOPING);

  NOM_DUMP( nom::audio_playback_position(buffer, dev) );

  auto queued_audio = [=]() {
    nom::play_audio(buffer, dev);
  };

  auto audio_thread = std::thread(queued_audio);

  auto duration = nom::audio_duration(buffer);

  // FIXME:
  real32 duration_seconds = duration / 1000.0f;

  Timer kill_timer;
  kill_timer.start();

  NOM_DUMP(duration_seconds);

  audio_thread.join();

  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Frame count:", nom::frame_count(buffer) );
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Sample count:", nom::sample_count(buffer) );
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Sample rate:", nom::sample_rate(buffer) );
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Channel count:", nom::channel_count(buffer) );
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Audio in bytes:", nom::audio_bytes(buffer) );

  while( kill_timer.ticks() < duration ) {}
  // while( nom::audio_state(buffer) != SOUND_STOPPED ) {}

  nom::free_buffer(buffer);
  NOM_DELETE_PTR(listener);
  nom::shutdown_audio(dev);
  NOM_DELETE_PTR(dev);

  return NOM_EXIT_SUCCESS;
}
