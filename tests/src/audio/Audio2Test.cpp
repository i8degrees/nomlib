/******************************************************************************

  nomlib - C++11 cross-platform game engine

  Copyright (c) 2013, 2014, 2015, 2016 Jeffrey Carpenter <i8degrees@gmail.com>
  All rights reserved.

  This test exercises the `nomlib-audio` OpenAL-backed audio subsystem.

******************************************************************************/
#include <iostream>
#include <string>

#include <nomlib/tests/UnitTest.hpp>

#include <nomlib/config.hpp>
#include <nomlib/system/init.hpp>
#include <nomlib/system/Timer.hpp>
#include <nomlib/core/clock.hpp>
#include <nomlib/audio/audio.hpp>

#ifndef NOM_PROJECT_SOURCE_DIR
  #define NOM_PROJECT_SOURCE_DIR "."
#endif

namespace audio = nom::audio;

std::string wav_resource_path(const std::string& filename)
{
  return std::string(NOM_PROJECT_SOURCE_DIR)
       + "/Resources/tests/audio/ALAudioTest/"
       + filename;
}

class Audio2Test: public ::testing::Test
{
  public:
    Audio2Test() = default;
    virtual ~Audio2Test() = default;

  protected:
    // Convenience: initialize the v2 audio subsystem with a modest source pool.
    void init_v2(nom::uint32 channels = 16)
    {
      ASSERT_TRUE(audio::init_audio(channels))
        << "audio::init_audio(" << channels << ") should succeed";

      audio::IAudioDevice* dev = audio::AudioDeviceLocator::instance();
      ASSERT_NE(nullptr, dev)
        << "AudioDeviceLocator::instance() should return a non-null device";

      dev->set_master_volume(0.0f);
    }

    void shutdown_v2()
    {
      audio::shutdown_audio();
    }
};

TEST_F(Audio2Test, InitAndShutdown)
{
  this->init_v2();

  audio::IAudioDevice* dev = audio::AudioDeviceLocator::instance();
  dev->set_master_volume(0.25f);

  this->shutdown_v2();
}

TEST_F(Audio2Test, SoundManagerLoadsWav)
{
  this->init_v2();

  const std::string path = wav_resource_path("4s.wav");
  audio::SoundManager::add_sound(path, /*streaming=*/false);

  audio::Sound* sound = audio::SoundManager::find_sound(path);
  ASSERT_NE(nullptr, sound)
    << "SoundManager::find_sound should return a loaded sound";

  EXPECT_GT(sound->total_bytes(), 0);
  EXPECT_GE(sound->channels(), 1);
  EXPECT_GT(sound->frequency(), 0);
  EXPECT_GT(sound->sample_count(), 0);
  EXPECT_GT(sound->duration(), 0.0);
  EXPECT_GT(sound->bitrate(), 0u);

  // shutdown_audio detaches renderer sources before freeing cached sounds.
  this->shutdown_v2();
}

TEST_F(Audio2Test, SoundManagerReturnsNullForUnknownName)
{
  this->init_v2();

  audio::Sound* sound = audio::SoundManager::find_sound(
    wav_resource_path("definitely_not_loaded.wav"));
  EXPECT_EQ(nullptr, sound)
    << "SoundManager::find_sound should return nullptr for an unknown name";

  this->shutdown_v2();
}

TEST_F(Audio2Test, RendererPlaysSoundAtZeroGain)
{
  this->init_v2();

  const std::string path = wav_resource_path("audio-channel-front-center.wav");
  audio::SoundManager::add_sound(path, /*streaming=*/false);

  audio::Sound* sound = audio::SoundManager::find_sound(path);
  ASSERT_NE(nullptr, sound)
    << "RendererPlaysSoundAtZeroGain: sound should be loaded";

  audio::IAudioDevice* dev = audio::AudioDeviceLocator::instance();
  ASSERT_NE(nullptr, dev);

  dev->set_master_volume(0.0f);
  dev->play_sound(sound, 0.0f);

  // Let the sound render briefly, including after it stops playing, so the
  // renderer can exercise its source-lifecycle cleanup path.
  nom::Timer timer;
  timer.start();

  nom::uint32 previous = timer.ticks();
  nom::uint32 elapsed = 0;
  do {
    nom::sleep(16);

    const nom::uint32 current = timer.ticks();
    elapsed = current;
    const nom::real32 delta = nom::Timer::to_seconds(current - previous);
    previous = current;

    dev->update(delta);
  } while(elapsed < 2000);

  // shutdown_audio detaches renderer sources before freeing cached sounds.
  this->shutdown_v2();
}

TEST_F(Audio2Test, StreamingOggPlaysForTwoSeconds)
{
  // Streaming sources are typically decoded in chunks, so use a larger
  // source pool like the nom_astream example.
  this->init_v2(32);

  const std::string path = wav_resource_path("laura_ff1-s16-mono.ogg");
  audio::SoundManager::add_sound(path, /*streaming=*/true);

  audio::Sound* media = audio::SoundManager::find_sound(path);
  ASSERT_NE(nullptr, media)
    << "StreamingOggPlaysForTwoSeconds: Ogg stream should be loaded";

  EXPECT_TRUE(media->streaming())
    << "A sound added with streaming=true should report streaming() == true";
  EXPECT_GT(media->duration(), 0.0)
    << "Ogg stream should have a positive duration";

  audio::IAudioDevice* dev = audio::AudioDeviceLocator::instance();
  ASSERT_NE(nullptr, dev);

  dev->set_master_volume(0.0f);
  dev->play_sound(media, 0.0f);

  // Run the renderer update loop for two seconds so the streaming source
  // decodes and queues additional buffers.
  nom::Timer timer;
  timer.start();

  nom::uint32 previous = timer.ticks();
  nom::uint32 elapsed = 0;
  do {
    nom::sleep(16);

    const nom::uint32 current = timer.ticks();
    elapsed = current;
    const nom::real32 delta = nom::Timer::to_seconds(current - previous);
    previous = current;

    dev->update(delta);
  } while(elapsed < 2000);

  // shutdown_audio detaches renderer sources before freeing cached sounds.
  this->shutdown_v2();
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  NOM_ASSERT(nom::init(argc, argv) == true);
  atexit(nom::quit);

  const int result = RUN_ALL_TESTS();

  // Clean up any sounds left by tests that did not call shutdown_v2.
  audio::shutdown_audio();

  return result;
}
