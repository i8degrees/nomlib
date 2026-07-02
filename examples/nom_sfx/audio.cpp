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

// NOTE: Audio playback usage example

#include <nomlib/audio.hpp>
#include <nomlib/actions.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/serializers.hpp>
#include <nomlib/version.hpp>
#include "tclap/CmdLine.h"

#include <thread>

using namespace nom;

const std::string APP_NAME = "nomlib: audio";

// File resource paths
SearchPath res;

/// \remarks See program usage by passing --help
struct AppFlags
{
  /// The input file source to play
  std::string audio_input = "\0";

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
  auto audio_thread = std::thread();
  Timer elapsed;
  EventHandler evt_handler;

  audio::AudioSpec request = {};
  audio::IOAudioEngine* dev = nullptr;
  audio::SoundBuffer* buffer = nullptr;

  const char* RES_FILENAME = "audio.json";
  real32 master_gain = 100.0f;
  const real32 pitch = 1.0f;
  const Point3f audio_pos = {0.0f, 0.0f, 0.0f};
  const Point3f audio_velocity = {0.0f, 0.0f, 0.0f};

  // TODO(jeff): command-line switches for these variables
NOM_IGNORED_VARS();
  const auto ACTION_FADE_DISPLACEMENT = 100.0f;
  const auto ACTION_DURATION = 1.0f;
  const auto ACTION_TIMING_CURVE =
    nom::make_timing_curve_from_string("linear_ease_in");
  const auto ACTION_SPEED = 1.0f;
NOM_IGNORED_VARS_ENDL();
  // TODO(jeff): command-line switches for these variables..?
  nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_AUDIO,
                                        NOM_LOG_PRIORITY_DEBUG);

  nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TRACE_AUDIO,
                                        NOM_LOG_PRIORITY_DEBUG);

  nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION,
                                        NOM_LOG_PRIORITY_DEBUG);
#if defined(NOM_DEBUG)
  nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TEST,
                                        NOM_LOG_PRIORITY_DEBUG);
#else
  nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TEST,
                                        NOM_LOG_PRIORITY_WARN);
#endif

  ActionPlayer audio_player;

  if(res.load_file(RES_FILENAME, "resources") == false) {
    NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION,
                 "Could not resolve the resources path from given input:",
                 RES_FILENAME);
    exit(NOM_EXIT_FAILURE);
  }

  if(parse_cmdline(argc, argv, args) != 0) {
    exit(NOM_EXIT_FAILURE);
  }

  if(args.audio_input.length() < 1) {
    args.audio_input = res.path() + "sinewave_1s-900.wav";
  }

  // Fatal error; if we are not able to complete this step, it means that
  // we probably cannot rely on our resource paths!
  if(nom::init(argc, argv) == false) {
    NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION,
                 "Could not initialize nomlib.");
    exit(NOM_EXIT_FAILURE);
  }
  atexit(nom::quit);

  // Quick and dirty method of testing the use of nomlib's audio subsystem
  // #undef NOM_USE_OPENAL

  // Initialize audio subsystem...
  request.engine = "openal";
  // TEST CODE: REMOVE ME
#if 1
  request.sample_rate = 48000;
  request.num_mono_sources = 32;
  request.num_stereo_sources = 16;
#endif
  if(args.use_null_interface == false) {
    audio::AudioSpec spec = {};
    dev = audio::init_audio(&request, &spec);
    if(dev == nullptr) {
      NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION,
                   "Failed to create audio device");
      exit(NOM_EXIT_FAILURE);
    }
    const char* audio_dev_name = spec.name;
    if(audio_dev_name == nullptr) {
      audio_dev_name = "Unknown device";
    }

    NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "Audio device name:",
                 audio_dev_name);
  }

  master_gain = args.audio_volume;
  audio::set_volume(master_gain, dev);

  buffer = audio::create_buffer(args.audio_input, dev);
  if(audio::valid_buffer(buffer, dev) == false) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Could not load audio samples from:", args.audio_input);
    return NOM_EXIT_FAILURE;
  }

  // TODO(jeff): Implement per-buffer gain level passing via command line
  // audio::set_volume(buffer, dev, args.audio_volume);

  audio::set_pitch(buffer, dev, pitch);
  audio::set_position(buffer, dev, audio_pos);
  audio::set_velocity(buffer, dev, audio_velocity);
  // audio::set_state(buffer, dev, audio::AUDIO_STATE_LOOPING);
#if 1
  auto playback_action =
    nom::create_action<PlayAudioSource>(dev, args.audio_input.c_str());
#else
  auto playback_action =
    nom::create_action<FadeAudioGainBy>(dev, buffer, ACTION_FADE_DISPLACEMENT,
                                        ACTION_DURATION);
#endif
  // auto playback_action =
  //   nom::create_action<FadeAudioGainBy>(dev, args.audio_input.c_str(),
  //                                       ACTION_FADE_DISPLACEMENT,
  //                                       ACTION_DURATION);
  playback_action->set_timing_curve(ACTION_TIMING_CURVE);
  playback_action->set_speed(ACTION_SPEED);
  playback_action->set_name("audio_playback");
  audio_player.run_action(playback_action);

#if 0
  if(args.use_music_interface == true) {
    // ...
  } else if(args.use_music_interface == false) {

    audio_thread =
    std::thread([=,&audio_player, &evt_handler, &elapsed, &buffer, &dev]() {
      elapsed.start();
      audio::play(buffer, dev);
#if 1
      uint32 last_delta = elapsed.ticks();
      uint32 playback_state = audio::AUDIO_STATE_PLAYING;
      while(playback_state != audio::AUDIO_STATE_STOPPED) {
        playback_state = audio::state(buffer, dev);

        uint32 end_delta = elapsed.ticks();
        uint32 elapsed_delta = end_delta - last_delta;
        last_delta = end_delta;
        // NOM_DUMP(elapsed_delta);

        audio_player.update(elapsed_delta);
      }
#else
      // sound state should be set to audio::AUDIO_STATE_PLAYING after issuing
      // the play command
      uint32 playback_state = audio::state(buffer, dev);
      while(elapsed.to_seconds() < 20.0f) {

        nom::Event evt;
        while(evt_handler.poll_event(evt) == true) {

          switch(evt.type) {
            default: break;

            case Event::QUIT_EVENT: {
              NOM_DUMP_VAR(NOM, "goodbye!\n");
              audio::free_buffer(buffer, dev);
              audio::shutdown_audio(dev);
              exit(NOM_EXIT_SUCCESS);
            } break;
          } // end switch
        } // end event polling loop

        auto dev_connected = dev->connected();
        if(dev_connected == 1) {
          // NOM_DUMP_VAR(NOM, "audio connected");
        } else {
          NOM_DUMP_VAR(NOM, "audio disconnected");
          break;
        }

        playback_state = audio::state(buffer, dev);
        if(playback_state == audio::AUDIO_STATE_STOPPED) {
          // break;
        }
      }
#endif
      NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
                   "Elapsed duration (seconds):", elapsed.to_seconds());
    });
  }
#endif

  audio::SoundInfo info = audio::info(buffer);
  auto playback_pos = audio::playback_position(buffer, dev);

  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Playback cursor (seconds):", playback_pos);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Duration (seconds):", info.duration);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Frame count:", info.frame_count);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Sample count:", info.sample_count);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Sample rate:", info.sample_rate);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Channel count:", info.channel_count);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Audio in bytes:", info.total_bytes);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Seekable:", info.seekable);
#if 0
  audio_thread.join();
#endif

  elapsed.start();
  uint32 last_delta = elapsed.ticks();
  uint32 playback_state = audio::AUDIO_STATE_PLAYING;

  bool playback_eof = false;
  while(playback_eof == false) {
    playback_state = audio::state(buffer, dev);

    nom::Event evt;
    while(evt_handler.poll_event(evt) == true) {

      switch(evt.type) {
        default: break;

        case Event::KEY_PRESS: {
          switch(evt.key.sym) {
            default: {
            } break;

            case SDLK_ESCAPE:
            case SDLK_q: {
              audio::stop(buffer, dev);
              playback_eof = true;
            } break;
          }
        } break;

        case Event::QUIT_EVENT: {
          audio::stop(buffer, dev);
          playback_eof = true;
        } break;
      } // end switch
    } // end event polling loop

    uint32 end_delta = elapsed.ticks();
    uint32 elapsed_delta = end_delta - last_delta;
    last_delta = end_delta;
    audio_player.update(elapsed_delta);

    auto elapsed_seconds = elapsed.to_seconds(end_delta);
    if(elapsed_seconds >= buffer->duration &&
       playback_state != audio::AUDIO_STATE_PLAYING)
    {
      audio::stop(buffer, dev);
      playback_eof = true;
    }

    nom::sleep(17); // Emulate 60 FPS
  }

  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION,
               "Elapsed duration (seconds):", elapsed.to_seconds());
  // audio::free_buffer(buffer, dev);
  playback_action->release();
  audio::shutdown_audio(dev);

  return NOM_EXIT_SUCCESS;
}
