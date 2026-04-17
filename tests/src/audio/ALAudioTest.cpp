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
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

using namespace nom;

class ALAudioTest: public ::testing::Test
{
  public:
    /// \remarks This method is called at the start of each unit test.
    ALAudioTest() :
      dev(nullptr),
      listener(nullptr)
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

    std::unique_ptr<IAudioDevice> dev;
    std::unique_ptr<IListener> listener;
};

namespace test {

std::unique_ptr<AudioDevice> create_audio_handle()
{
  std::unique_ptr<AudioDevice> dev;
  dev.reset( new AudioDevice() );

  return std::move(dev);
}

std::unique_ptr<NullAudioDevice> create_null_audio_handle()
{
  std::unique_ptr<NullAudioDevice> dev;
  dev.reset( new NullAudioDevice() );

  return std::move(dev);
}

std::unique_ptr<Listener> create_audio_listener_handle()
{
  std::unique_ptr<Listener> dev;
  dev.reset( new Listener() );

  return std::move(dev);
}

std::unique_ptr<NullListener> create_null_audio_listener_handle()
{
  std::unique_ptr<NullListener> dev;
  dev.reset( new NullListener() );

  return std::move(dev);
}

} // namespace test

TEST_F(ALAudioTest, NullAudioDevice)
{
  dev = test::create_null_audio_handle();

  EXPECT_EQ( "NullAudioDevice", dev->getDeviceName() );

  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, dev->getDeviceName() );
}

TEST_F( ALAudioTest, NullAudioListener )
{
  dev = test::create_null_audio_handle();
  listener = test::create_null_audio_listener_handle();

  EXPECT_EQ(100.0f, listener->volume() );
  EXPECT_EQ( Point3f(0,0,0), listener->position() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), listener->velocity() );
  EXPECT_EQ( Point3f(0.0f, 0.0f, -1.0f), listener->direction() );

  EXPECT_EQ( 0.0f, nom::Listener::min_volume() );
  EXPECT_EQ( 100.0f, nom::Listener::max_volume() );
}

TEST_F( ALAudioTest, NullSoundBuffer )
{
  dev = test::create_null_audio_handle();

  SoundBuffer buffer;
  EXPECT_TRUE( buffer.load(RESOURCE_AUDIO_SOUND) );
  EXPECT_EQ( 2400, buffer.get() );
  EXPECT_EQ( 455, buffer.getDuration() );
}

TEST_F( ALAudioTest, NullSound )
{
  NullSoundBuffer buffer;
  NullSound sound;

  dev = test::create_null_audio_handle();

  EXPECT_TRUE( buffer.load(RESOURCE_AUDIO_SOUND) );
  EXPECT_EQ( 0, buffer.get() );
  EXPECT_EQ( 0, buffer.getDuration() );

  sound.setBuffer(buffer);
  EXPECT_EQ( nom::SoundStatus::Stopped, sound.getStatus() );
  EXPECT_EQ(100.0f, sound.volume() );
  EXPECT_EQ(0.0f, sound.min_volume() );
  EXPECT_EQ(100.0f, sound.max_volume() );
  EXPECT_EQ( 0.0f, sound.getPitch() );
  EXPECT_EQ( false, sound.getLooping() );
  EXPECT_EQ( Point3f::zero, sound.position() );
  EXPECT_EQ( Point3f::zero, sound.velocity() );
  EXPECT_EQ( false, sound.getPositionRelativeToListener() );
  EXPECT_EQ( 0.0f, sound.getMinDistance() );
  EXPECT_EQ( 0.0f, sound.getAttenuation() );
  EXPECT_EQ( -1, sound.getBufferID() );
  EXPECT_EQ( 0.0f, sound.getPlayPosition() );
}

TEST_F( ALAudioTest, NullMusic )
{
  NullSoundBuffer buffer;
  NullMusic sound;

  dev = test::create_null_audio_handle();

  EXPECT_TRUE( buffer.load(RESOURCE_AUDIO_SOUND) );
  EXPECT_EQ( 0, buffer.get() );
  EXPECT_EQ( 0, buffer.getDuration() );

  sound.setBuffer(buffer);
  EXPECT_EQ( nom::SoundStatus::Stopped, sound.getStatus() );
  EXPECT_EQ(100.0f, sound.volume() );
  EXPECT_EQ(0.0f, sound.min_volume() );
  EXPECT_EQ(100.0f, sound.max_volume() );
  EXPECT_EQ( 0.0f, sound.getPitch() );
  EXPECT_EQ( false, sound.getLooping() );
  EXPECT_EQ( Point3f::zero, sound.position() );
  EXPECT_EQ( Point3f::zero, sound.velocity() );
  EXPECT_EQ( false, sound.getPositionRelativeToListener() );
  EXPECT_EQ( 0.0f, sound.getMinDistance() );
  EXPECT_EQ( 0.0f, sound.getAttenuation() );
  EXPECT_EQ( -1, sound.getBufferID() );
  EXPECT_EQ( 0.0f, sound.getPlayPosition() );
}

TEST_F( ALAudioTest, NullAudioDeviceLocatorAPI )
{
  nom::AudioDeviceLocator::set_provider(nullptr);

  EXPECT_EQ( "NullAudioDevice", nom::AudioDeviceLocator::audio_device().getDeviceName() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, nom::AudioDeviceLocator::audio_device().getDeviceName() );
}

// EOF NullAudio tests

#if defined( NOM_USE_OPENAL )

TEST_F( ALAudioTest, AudioDevice )
{
  dev = test::create_audio_handle();
  EXPECT_NE( "", dev->getDeviceName() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, dev->getDeviceName() );
}

TEST_F(ALAudioTest, AudioListener)
{
  dev = test::create_audio_handle();
  listener = test::create_audio_listener_handle();

  EXPECT_EQ( 100.0f, listener->volume() );
  EXPECT_EQ( Point3f(0,0,0), listener->position() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), listener->velocity() );
  EXPECT_EQ( Point3f(0.0f, 0.0f, -1.0f), listener->direction() );
  EXPECT_EQ( 0.0f, nom::Listener::min_volume() );
  EXPECT_EQ( 100.0f, nom::Listener::max_volume() );
}

TEST_F(ALAudioTest, SoundBuffer)
{
  dev = test::create_audio_handle();

  SoundBuffer buffer;
  EXPECT_TRUE( buffer.load(RESOURCE_AUDIO_SOUND) );

  // EXPECT_EQ( 2400, buffer.get() );
  EXPECT_EQ( 455, buffer.getDuration() );
}

TEST_F(ALAudioTest, Sound)
{
  SoundBuffer buffer;
  Sound sound;
  dev = test::create_audio_handle();

  EXPECT_TRUE( buffer.load(RESOURCE_AUDIO_SOUND) );
  // EXPECT_EQ( 2401, buffer.get() );
  EXPECT_EQ(455, buffer.getDuration() );

  sound.setBuffer(buffer);
  EXPECT_EQ(nom::SoundStatus::Stopped, sound.getStatus() );

  EXPECT_FLOAT_EQ( 100.0f, sound.volume() );
  EXPECT_EQ( 0.0f, sound.min_volume() );
  EXPECT_EQ( 100.0f, sound.max_volume() );
  EXPECT_EQ( 1, sound.getPitch() );
  EXPECT_EQ( false, sound.getLooping() );
  EXPECT_EQ( Point3f::zero, sound.position() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), sound.velocity() );
  EXPECT_EQ( false, sound.getPositionRelativeToListener() );
  EXPECT_FLOAT_EQ( 1, sound.getMinDistance() );
  EXPECT_FLOAT_EQ( 1, sound.getAttenuation() );
  // EXPECT_EQ( 2401, sound.getBufferID() );
  EXPECT_FLOAT_EQ( 0.0f, sound.getPlayPosition() );
}

// FIXME: This interface is broken; the sound buffer memory is not properly
// deallocated.
TEST_F(ALAudioTest, Music)
{
  SoundBuffer buffer;
  Music sound;
  dev = test::create_audio_handle();

  EXPECT_TRUE( buffer.load(RESOURCE_AUDIO_SOUND) );
  // EXPECT_EQ( 2404, buffer.get() );
  EXPECT_EQ( 455, buffer.getDuration() );

  sound.setBuffer(buffer);
  EXPECT_EQ( nom::SoundStatus::Stopped, sound.getStatus() );
}

TEST_F(ALAudioTest, AudioDeviceLocatorAPI)
{
  dev = test::create_audio_handle();
  nom::AudioDeviceLocator::set_provider( dev.get() );

  EXPECT_NE( "", nom::AudioDeviceLocator::audio_device().getDeviceName() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, nom::AudioDeviceLocator::audio_device().getDeviceName() );

  EXPECT_TRUE( this->dev.get() == &nom::AudioDeviceLocator::audio_device() );
}

#endif // defined NOM_USE_OPENAL

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  NOM_ASSERT( nom::init ( argc, argv ) == true );
  atexit( nom::quit );

  return RUN_ALL_TESTS();
}
