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

#include "nomlib/config.hpp"
#include "nomlib/system/init.hpp"
#include "nomlib/audio.hpp"

using namespace nom;

class ALAudioTest: public ::testing::Test
{
  public:
    /// \remarks This method is called at the start of each unit test.
    ALAudioTest( void ) :
      dev{ nullptr },
      listener{ nullptr },
      buffer{ nullptr },
      sound{ nullptr }
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~ALAudioTest( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown( void )
    {
      // NOM_LOG_TRACE( NOM );

      NOM_DELETE_PTR( dev );
      NOM_DELETE_PTR( listener );
      NOM_DELETE_PTR( buffer );
      NOM_DELETE_PTR( sound );
    }

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

  protected:
    const std::string APP_RESOURCES_DIR = "Resources";
    const nom::Path p;
    const std::string RESOURCE_AUDIO_SOUND = APP_RESOURCES_DIR + p.native() + "cursor_wrong.wav";

    nom::IAudioDevice* dev;
    nom::IListener* listener;
    nom::ISoundBuffer* buffer;
    nom::ISoundSource* sound;
};

TEST_F( ALAudioTest, NullAudioDevice )
{
  this->dev = new nom::NullAudioDevice();
  EXPECT_EQ( "NullAudioDevice", this->dev->getDeviceName() );

  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, this->dev->getDeviceName() );
}

TEST_F( ALAudioTest, NullAudioListener )
{
  // Dependency
  this->dev = new nom::NullAudioDevice();

  this->listener = new nom::NullListener();

  EXPECT_EQ( 0.0f, this->listener->getVolume() );
  EXPECT_EQ( Point3f(0,0,0), this->listener->getPosition() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), this->listener->getVelocity() );
  EXPECT_EQ( Point3f(0,0,-1), this->listener->getDirection() );
}

TEST_F( ALAudioTest, NullSoundBuffer )
{
  this->buffer = new nom::NullSoundBuffer();
  EXPECT_EQ( 0, this->buffer->get() );
  EXPECT_EQ( 0, this->buffer->getDuration() );
  EXPECT_FALSE( this->buffer->load( RESOURCE_AUDIO_SOUND ) );
}

TEST_F( ALAudioTest, NullSound )
{
  // Dependencies
  this->dev = new nom::NullAudioDevice();
  this->listener = new nom::NullListener();
  this->buffer = new nom::NullSoundBuffer();
  this->sound = new nom::NullSound();

  EXPECT_EQ( 0, this->buffer->get() );
  EXPECT_EQ( 0, this->buffer->getDuration() );
  EXPECT_FALSE( this->buffer->load( RESOURCE_AUDIO_SOUND ) );

  this->sound->setBuffer( *buffer );
  EXPECT_EQ( nom::SoundStatus::Stopped, this->sound->getStatus() );
  EXPECT_EQ( 0.0f, this->sound->getVolume() );
  EXPECT_EQ( 0.0f, this->sound->getMinVolume() );
  EXPECT_EQ( 0.0f, this->sound->getMaxVolume() );
  EXPECT_EQ( 0.0f, this->sound->getPitch() );
  EXPECT_EQ( false, this->sound->getLooping() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), this->sound->getPosition() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), this->sound->getVelocity() );
  EXPECT_EQ( false, this->sound->getPositionRelativeToListener() );
  EXPECT_EQ( 0.0f, this->sound->getMinDistance() );
  EXPECT_EQ( 0.0f, this->sound->getAttenuation() );
  EXPECT_EQ( -1, this->sound->getBufferID() );
  EXPECT_EQ( 0.0f, this->sound->getPlayPosition() );
}

TEST_F( ALAudioTest, NullMusic )
{
  // Dependencies
  this->dev = new nom::NullAudioDevice();
  this->listener = new nom::NullListener();
  this->buffer = new nom::NullSoundBuffer();
  this->sound = new nom::NullMusic();

  EXPECT_EQ( 0, this->buffer->get() );
  EXPECT_EQ( 0, this->buffer->getDuration() );
  EXPECT_FALSE( this->buffer->load( RESOURCE_AUDIO_SOUND ) );
  this->sound->setBuffer( *buffer );
  EXPECT_EQ( nom::SoundStatus::Stopped, this->sound->getStatus() );
}

TEST_F( ALAudioTest, NullAudioDeviceLocatorAPI )
{
  nom::AudioDeviceLocator::set_provider( nullptr );

  EXPECT_EQ( "NullAudioDevice", nom::AudioDeviceLocator::audio_device().getDeviceName() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, nom::AudioDeviceLocator::audio_device().getDeviceName() );
}

// EOF NullAudio tests

#if defined( NOM_USE_OPENAL )

TEST_F( ALAudioTest, AudioDevice )
{
  this->dev = new nom::AudioDevice();
  EXPECT_NE( "", this->dev->getDeviceName() );

  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, this->dev->getDeviceName() );
}

TEST_F( ALAudioTest, AudioListener )
{
  // Dependency
  this->dev = new nom::AudioDevice();

  this->listener = new nom::Listener();

  EXPECT_EQ( 100, this->listener->getVolume() );
  EXPECT_EQ( Point3f(0,0,0), this->listener->getPosition() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), this->listener->getVelocity() );
  EXPECT_EQ( Point3f(-1,-1,-1), this->listener->getDirection() );
}

TEST_F( ALAudioTest, SoundBuffer )
{
  this->buffer = new nom::SoundBuffer();
  EXPECT_TRUE( this->buffer->load( RESOURCE_AUDIO_SOUND ) );

  // EXPECT_EQ( 2400, this->buffer->get() );
  EXPECT_EQ( 455, this->buffer->getDuration() );
}

TEST_F( ALAudioTest, Sound )
{
  // Dependencies
  this->dev = new nom::AudioDevice();
  this->listener = new nom::Listener();
  this->buffer = new nom::SoundBuffer();
  this->sound = new nom::Sound();

  EXPECT_TRUE( this->buffer->load( RESOURCE_AUDIO_SOUND ) );
  // EXPECT_EQ( 2401, this->buffer->get() );
  EXPECT_EQ( 455, this->buffer->getDuration() );

  this->sound->setBuffer( *buffer );
  EXPECT_EQ( nom::SoundStatus::Stopped, this->sound->getStatus() );

  EXPECT_EQ( 100, this->sound->getVolume() );
  EXPECT_EQ( 0, this->sound->getMinVolume() );
  EXPECT_EQ( 1.0f, this->sound->getMaxVolume() );
  EXPECT_EQ( 1, this->sound->getPitch() );
  EXPECT_EQ( false, this->sound->getLooping() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), this->sound->getPosition() );
  EXPECT_EQ( Point3f(0.0f,0.0f,0.0f), this->sound->getVelocity() );
  EXPECT_EQ( false, this->sound->getPositionRelativeToListener() );
  EXPECT_EQ( 1, this->sound->getMinDistance() );
  EXPECT_EQ( 1, this->sound->getAttenuation() );
  // EXPECT_EQ( 2401, this->sound->getBufferID() );
  EXPECT_EQ( 0.0f, this->sound->getPlayPosition() );
}

TEST_F( ALAudioTest, Music )
{
  // Dependencies
  this->dev = new nom::AudioDevice();
  this->listener = new nom::Listener();
  this->buffer = new nom::SoundBuffer();
  this->sound = new nom::Music();

  EXPECT_TRUE( this->buffer->load( RESOURCE_AUDIO_SOUND ) );
  // EXPECT_EQ( 2404, this->buffer->get() );
  EXPECT_EQ( 455, this->buffer->getDuration() );

  this->sound->setBuffer( *buffer );
  EXPECT_EQ( nom::SoundStatus::Stopped, this->sound->getStatus() );
}

TEST_F( ALAudioTest, ThreeAudioDeviceInitalizations )
{
  this->dev = new nom::AudioDevice();
  EXPECT_NE( "", this->dev->getDeviceName() );

  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, this->dev->getDeviceName() );

  this->dev = new nom::AudioDevice();
  EXPECT_NE( "", this->dev->getDeviceName() );

  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, this->dev->getDeviceName() );

  this->dev = new nom::AudioDevice();
  EXPECT_NE( "", this->dev->getDeviceName() );

  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, this->dev->getDeviceName() );
}

TEST_F( ALAudioTest, AudioDeviceLocatorAPI )
{
  this->dev = new nom::AudioDevice();
  nom::AudioDeviceLocator::set_provider( this->dev );

  EXPECT_NE( "", nom::AudioDeviceLocator::audio_device().getDeviceName() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, nom::AudioDeviceLocator::audio_device().getDeviceName() );

  EXPECT_TRUE( this->dev == &nom::AudioDeviceLocator::audio_device() );
}

#endif // defined NOM_USE_OPENAL

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  NOM_ASSERT( nom::init ( argc, argv ) == true );
  atexit( nom::quit );

  return RUN_ALL_TESTS();
}
