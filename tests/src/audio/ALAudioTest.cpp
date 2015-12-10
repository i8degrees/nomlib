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
#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/config.hpp>
#include <nomlib/system/init.hpp>
#include <nomlib/system/Path.hpp>
#include <nomlib/audio.hpp>

using namespace nom;

class ALAudioTest: public ::testing::Test
{
  public:
    /// \remarks This method is called at the start of each unit test.
    ALAudioTest()
    {
      // NOM_LOG_TRACE( NOM );
      nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_AUDIO,
                                            NOM_LOG_PRIORITY_VERBOSE);
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~ALAudioTest()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase()
    {
      // NOM_LOG_TRACE( NOM );
    }

  protected:
    const std::string APP_RESOURCES_DIR = "Resources";
    const nom::Path p;
    const std::string RESOURCE_AUDIO_SOUND = APP_RESOURCES_DIR +
      p.native() + "cursor_wrong.wav";

    IAudioDevice* dev = nullptr;
    IListener* listener = nullptr;
};

namespace test {

} // namespace test

// FIXME: create audio_device_name() function
#if 0
TEST_F(ALAudioTest, NullAudioDevice)
{
  ASSERT_TRUE(dev == nullptr);

  EXPECT_EQ("NullAudioDevice", dev->device_name() );
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, dev->device_name() );

  nom::shutdown_audio(dev);
}
#endif

// FIXME: create listener audio functions
#if 0
TEST_F(ALAudioTest, NullAudioListener)
{
  ASSERT_TRUE(dev == nullptr);

  // FIXME:
  // listener.reset( new nom::NullListener() );
  listener.reset( new nom::Listener() );

  EXPECT_EQ(100.0f, listener->volume() );
  EXPECT_EQ( Point3f(0,0,0), listener->position() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), listener->velocity() );
  EXPECT_EQ( Point3f(0.0f, 0.0f, -1.0f), listener->direction() );

  EXPECT_EQ( 0.0f, nom::Listener::min_volume() );
  EXPECT_EQ( 100.0f, nom::Listener::max_volume() );

  nom::shutdown_audio(dev);
}
#endif

TEST_F(ALAudioTest, NullSoundBuffer)
{
  SoundBuffer* buffer = nullptr;

  ASSERT_TRUE(dev == nullptr);

  buffer = nom::create_audio_buffer();
  EXPECT_TRUE(buffer != nullptr);

  EXPECT_EQ(0, nom::audio_id(buffer, AudioID::AUDIO_BUFFER_ID) );
  EXPECT_EQ(0, nom::audio_id(buffer, AudioID::SOUND_ID) );
  EXPECT_EQ(0, nom::audio_duration(buffer) );
  EXPECT_NE(AUDIO_STATE_LOOPING, nom::audio_state(buffer, dev) );

  nom::free_buffer(buffer);
  EXPECT_TRUE(buffer != nullptr);
  nom::shutdown_audio(dev);
}

TEST_F(ALAudioTest, NullSound)
{
  ASSERT_TRUE(dev == nullptr);

  auto buffer = nom::create_audio_buffer();
  EXPECT_TRUE(buffer != nullptr);

  EXPECT_EQ(AUDIO_STATE_STOPPED, nom::audio_state(buffer, dev) );
  EXPECT_FLOAT_EQ(0.0f, nom::audio_volume(buffer, dev) );
  EXPECT_FLOAT_EQ(0.0f, nom::audio_min_volume(buffer, dev) );
  EXPECT_FLOAT_EQ(0.0f, nom::audio_max_volume(buffer, dev) );
  EXPECT_FLOAT_EQ(0.0f, nom::audio_pitch(buffer, dev) );
  EXPECT_EQ(Point3f::zero, nom::audio_position(buffer, dev) );
  EXPECT_EQ(Point3f::zero, nom::audio_velocity(buffer, dev) );

  EXPECT_EQ(0.0f, nom::audio_playback_position(buffer, dev) );
  EXPECT_EQ(0.0f, nom::audio_playback_samples_position(buffer, dev) );

  nom::free_buffer(buffer);

  // FIXME:
  // EXPECT_TRUE(buffer != nullptr);

  // TODO: Check for proper shutdown?
  nom::shutdown_audio(dev);
}

#if 0
TEST_F(ALAudioTest, NullMusic)
{
  // SoundBuffer* buffer = nullptr;
  NullMusic sound;

  dev = test::create_null_audio_handle();

  // buffer = nom::create_audio_buffer();
  // ASSERT_TRUE(buffer != nullptr);

  // IMPORTANT: The buffer id result is internally dependent on the platform
  // architecture, i.e.: x86, x64 -- because of the variable size of ALuint.
  EXPECT_GT(0, nom::audio_id(buffer, AudioID::AUDIO_BUFFER_ID) );
  EXPECT_GT(0, nom::audio_id(buffer, AudioID::SOUND_ID) );
  // EXPECT_EQ(0, nom::audio_duration(buffer) );

  EXPECT_TRUE( sound.load_file(RESOURCE_AUDIO_SOUND) );
  EXPECT_EQ(SOUND_STOPPED, sound.status() );
  EXPECT_EQ(100.0f, sound.volume() );
  EXPECT_EQ(0.0f, sound.min_volume() );
  EXPECT_EQ(100.0f, sound.max_volume() );
  EXPECT_EQ( 0.0f, sound.pitch() );
  EXPECT_EQ( false, sound.getLooping() );
  EXPECT_EQ( Point3f::zero, sound.position() );
  EXPECT_EQ( Point3f::zero, sound.velocity() );
  EXPECT_EQ( false, sound.getPositionRelativeToListener() );
  EXPECT_EQ( 0.0f, sound.getMinDistance() );
  EXPECT_EQ( 0.0f, sound.getAttenuation() );
  EXPECT_EQ( -1, sound.getBufferID() );
  EXPECT_EQ( 0.0f, sound.getPlayPosition() );

  // nom::free_buffer(buffer);
  // EXPECT_TRUE(buffer != nullptr);
}

TEST_F( ALAudioTest, NullAudioDeviceLocatorAPI )
{
  nom::AudioDeviceLocator::set_provider(nullptr);

  EXPECT_EQ( "NullAudioDevice", nom::AudioDeviceLocator::audio_device().getDeviceName() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, nom::AudioDeviceLocator::audio_device().getDeviceName() );
}
#endif

// EOF NullAudio tests

#if defined(NOM_USE_OPENAL)

TEST_F(ALAudioTest, AudioDevice)
{
  dev = nom::create_audio_device(nullptr);
  ASSERT_TRUE(dev != nullptr);

  EXPECT_NE("", dev->device_name() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, dev->device_name() );

  nom::shutdown_audio(dev);
}

TEST_F(ALAudioTest, AudioListener)
{
  dev = nom::create_audio_device(nullptr);
  ASSERT_TRUE(dev != nullptr);

  listener = new Listener();

  EXPECT_EQ( 100.0f, listener->volume() );
  EXPECT_EQ( Point3f(0,0,0), listener->position() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), listener->velocity() );
  EXPECT_EQ( Point3f(0.0f, 0.0f, -1.0f), listener->direction() );
  EXPECT_EQ( 0.0f, nom::Listener::min_volume() );
  EXPECT_EQ( 100.0f, nom::Listener::max_volume() );

  nom::shutdown_audio(dev);
}

TEST_F(ALAudioTest, SoundBuffer)
{
  dev = nom::create_audio_device(nullptr);

  SoundBuffer* buffer = nullptr;
  buffer = nom::create_audio_buffer(RESOURCE_AUDIO_SOUND, dev);
  ASSERT_TRUE(buffer != nullptr);
  EXPECT_EQ(455, nom::audio_duration(buffer) );

  nom::free_buffer(buffer);
  EXPECT_TRUE(buffer != nullptr);

  nom::shutdown_audio(dev);
}

TEST_F(ALAudioTest, Sound)
{
  dev = nom::create_audio_device(nullptr);
  EXPECT_TRUE(dev != nullptr);

  auto buffer = nom::create_audio_buffer(RESOURCE_AUDIO_SOUND, dev);
  ASSERT_TRUE(buffer != nullptr);
  EXPECT_FLOAT_EQ(455, nom::audio_duration(buffer) );

  EXPECT_EQ(AUDIO_STATE_STOPPED, nom::audio_state(buffer, dev) );
  EXPECT_FLOAT_EQ(100.0f, nom::audio_volume(buffer, dev) );
  EXPECT_FLOAT_EQ(0.0f, nom::audio_min_volume(buffer, dev) );
  EXPECT_FLOAT_EQ(100.0f, nom::audio_max_volume(buffer, dev) );
  EXPECT_FLOAT_EQ(1.0f, nom::audio_pitch(buffer, dev) );
  EXPECT_EQ(Point3f::zero, nom::audio_position(buffer, dev) );
  EXPECT_EQ(Point3f::zero, nom::audio_velocity(buffer, dev) );

  // IMPORTANT: The buffer id result is internally dependent on the platform
  // architecture, i.e.: x86, x64 -- because of the variable size of ALuint.
  EXPECT_NE(0, nom::audio_id(buffer, AudioID::AUDIO_BUFFER_ID) );
  EXPECT_NE(0, nom::audio_id(buffer, AudioID::SOUND_ID) );

  EXPECT_EQ( 0.0f, nom::audio_playback_position(buffer, dev) );
  EXPECT_EQ( 0.0f, nom::audio_playback_samples_position(buffer, dev) );

  nom::free_buffer(buffer);
  nom::shutdown_audio(dev);
  EXPECT_TRUE(dev != nullptr);
}
#if 0
TEST_F(ALAudioTest, Music)
{
  Music sound;
  dev = test::create_audio_handle();

  // auto buffer = nom::create_audio_buffer(RESOURCE_AUDIO_SOUND);
  // ASSERT_TRUE(buffer != nullptr);
  // EXPECT_FLOAT_EQ(455, nom::audio_duration(buffer) );

  sound.load_file(RESOURCE_AUDIO_SOUND);
  EXPECT_EQ(SOUND_STOPPED, sound.status() );

  // nom::free_buffer(buffer);
  // EXPECT_TRUE(buffer != nullptr);
}
#endif
#if 0
TEST_F(ALAudioTest, AudioDeviceLocatorAPI)
{
  dev = test::create_audio_handle();
  nom::AudioDeviceLocator::set_provider(dev);

  EXPECT_NE( "", nom::AudioDeviceLocator::audio_device().getDeviceName() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, nom::AudioDeviceLocator::audio_device().getDeviceName() );

  EXPECT_TRUE( dev == &nom::AudioDeviceLocator::audio_device() );
}
#endif

// TODO: Finish implementation of test!
TEST_F(ALAudioTest, OpenALExtensions)
{
  bool ret = false;
  // uint32 freq = 0;

  dev = nom::create_audio_device(nullptr);
  ASSERT_TRUE(dev != nullptr);

  // i.e., check for available audio outputs (by device name)
  ret = nom::extension_availableT("ALC_enumeration_all_EXT", dev);

  // freq = nom::sound_frequency(dev);
  // NOM_DUMP(freq);

  ASSERT_TRUE(ret)
  << "The OpenAL extension for device enumeration is not supported";

  // ret = nom::extension_available("ALC_DEFAULT_DEVICE_SPECIFIER", dev);

  // FIXME
  // ASSERT_TRUE(ret)
  // << "FIXME: enumeration of output devices";

  nom::shutdown_audio(dev);
}

#endif // defined NOM_USE_OPENAL

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  NOM_ASSERT( nom::init ( argc, argv ) == true );
  atexit( nom::quit );

  return RUN_ALL_TESTS();
}
