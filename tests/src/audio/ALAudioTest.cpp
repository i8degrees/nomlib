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
#include <iostream>
#include <string>
#include <map>

#include <nomlib/tests/UnitTest.hpp>

#include <nomlib/config.hpp>
#include <nomlib/system/init.hpp>
#include <nomlib/system/Path.hpp>
#include <nomlib/audio.hpp>
#include <nomlib/system/Timer.hpp>
#include <nomlib/serializers.hpp>

// SCRATCH CODE
// #include <nomlib/core/unique_ptr.hpp>
#include <nomlib/core/strings.hpp>

using namespace nom;

#define EXPECT_FLOAT_NEAR(val1, val2, abs_error) \
  EXPECT_NEAR(val1, val2, abs_error)

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

      // NOTE(jeff): Find resource files for the test
      const std::string RES_FILENAME = "ALAudioTest.json";

      if(res.load_file(RES_FILENAME, "resources") == false) {
        FAIL()
        << "Could not resolve the 'resources' path from input file: "
        << RES_FILENAME;
      }

      auto p = file_ref("chunk", res.path() + "sinewave_1s-chunk.wav");
      AUDIO_RESOURCES.insert(p);

      p = file_ref("sine1s-900", res.path() + "sinewave_1s-900.wav");
      AUDIO_RESOURCES.insert(p);

      p = file_ref("sine2s-440", res.path() + "sinewave_2s-440.wav");
      AUDIO_RESOURCES.insert(p);

      p = file_ref("sine1s-real32", res.path() + "sinewave-real32_1s-900.wav");
      AUDIO_RESOURCES.insert(p);

      this->null_request.engine = "null"; // default
      this->openal_request.engine = "openal";
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
    const bool TEST_CHUNK_PLAYBACK = true;

    // Test resources (file paths)
    SearchPath res;

    typedef std::pair<const char*, std::string> file_ref;
    std::map<const char*, std::string> AUDIO_RESOURCES;

    audio::AudioSpec null_request = {};
    audio::AudioSpec openal_request = {};
};

namespace test {

static void play_audio(void* samples, const audio::SoundInfo& metadata,
                       audio::IOAudioEngine* target)
{
  nom::Timer elapsed;

  audio::SoundBuffer* buffer =
    audio::create_buffer(samples, metadata, target);
  ASSERT_TRUE(buffer != nullptr);

  EXPECT_NE(0, audio::buffer_id(buffer));
  EXPECT_NE(0, audio::source_id(buffer));

  // TODO(jeff): Write test for testing the playback of audio::play multiple
  // times; ensure that the buffering only occurs when necessary!
  // audio::queue(buffer, target);
  audio::play(buffer, target);

  elapsed.start();
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "start ticks:", elapsed.ticks() );
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "Total samples played:",
               audio::playback_samples(buffer, target));

  bool playback_samples_shown = false;
  uint32 playback_state = audio::AUDIO_STATE_PLAYING;
  while(playback_state != audio::AUDIO_STATE_STOPPED &&
        elapsed.to_seconds() < metadata.duration)
  {

    playback_state = audio::state(buffer, target);

    if(elapsed.ticks() > 500 && playback_samples_shown == false) {
      NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "Total samples played:",
                   audio::playback_samples(buffer, target));
      playback_samples_shown = true;
    }
  }

// IMPORTANT(jeff): OpenAL-Soft resets the samples playback cursor upon
// completion of the audio buffer, whereas Apple's distributed OpenAL SDK does
// not.
#if NOM_USE_APPLE_OPENAL
  EXPECT_EQ(metadata.sample_rate * metadata.duration,
            audio::playback_samples(buffer, target));
#endif

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "total samples played:",
               audio::playback_samples(buffer, target));

  EXPECT_GE(elapsed.ticks(), buffer->duration)
  << "The number of elapsed ticks should always be greater than the audio "
  << "playback duration!";

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "elapsed ticks:", elapsed.ticks() );

  audio::free_buffer(buffer, target);
}

} // namespace test

// TODO(jeff): Dedicate to SoundFileReaderTest::AudioRead test executable
TEST_F(ALAudioTest, SoundFileReader_AudioRead)
{
  const auto AUDIO_FILENAME = AUDIO_RESOURCES["sine1s-900"];
  void* buffer = nullptr;
  audio::SoundInfo metadata = {};

  audio::ISoundFileReader* fp = new audio::SoundFileReader();
  ASSERT_TRUE(fp != nullptr);

  EXPECT_EQ(true, fp->open(AUDIO_FILENAME, metadata))
  << "Could not load audio from input file " << AUDIO_FILENAME;
  EXPECT_EQ(true, fp->valid());

  const nom::size_type CHUNK_SIZE = metadata.total_bytes;
  buffer = new int16[CHUNK_SIZE * metadata.channel_count];

  EXPECT_NE(0, fp->read(buffer, metadata.channel_format, CHUNK_SIZE))
  << "Could not read audio samples from input file " << AUDIO_FILENAME;

  fp->close();
#if 0
  NOM_DELETE_PTR(fp);
#endif

  EXPECT_EQ(-15, NOM_SCAST(int16*, buffer)[0]);
  EXPECT_EQ(-5, NOM_SCAST(int16*, buffer)[1]);

  delete NOM_SCAST(int16*, buffer);
  buffer = nullptr;
}

// TODO(jeff): Dedicate to SoundFileReaderTest::AudioRead test executable
TEST_F(ALAudioTest, SoundFileReader_AudioReadChunk)
{
  const auto AUDIO_FILENAME = AUDIO_RESOURCES["chunk"];

  audio::SoundInfo metadata = {};
  nom::Timer elapsed;

  audio::IOAudioEngine* dev = nullptr;
  audio::AudioSpec spec = {};
  if(TEST_CHUNK_PLAYBACK == true) {
    dev = audio::init_audio(&this->openal_request, &spec);
    ASSERT_TRUE(dev != nullptr);
  }

  audio::ISoundFileReader* fp = new audio::SoundFileReader();
  ASSERT_TRUE(fp != nullptr);

  EXPECT_EQ(true, fp->open(AUDIO_FILENAME, metadata))
  << "Could not load audio from input file " << AUDIO_FILENAME;
  EXPECT_EQ(true, fp->valid());

  const nom::size_type CHUNK_SIZE = metadata.total_bytes / 2; // 44100
  EXPECT_TRUE(CHUNK_SIZE == metadata.sample_rate)
  << "The data read chunk size should match the input audio sample rate";

  EXPECT_TRUE(CHUNK_SIZE*2 == metadata.total_bytes)
  << "The data read chunk size should match the total size of the input audio";

  // Create a buffer that is large enough to read in both data chunks
  const nom::size_type READ_SIZE = CHUNK_SIZE/2;
  nom::size_type offset = 0;

  void* samples = new int16[metadata.total_bytes * metadata.channel_count];
  ASSERT_TRUE(samples != nullptr);

  auto first_half =
    fp->read((int16*)samples, metadata.channel_format, READ_SIZE);
  auto first_half_sample = NOM_SCAST(int16*, samples)[offset];
  EXPECT_EQ(-1, first_half_sample)
  << "read chunk offset: " << offset;

  offset += READ_SIZE;

  auto second_half =
    fp->read((int16*)samples + offset, metadata.channel_format, READ_SIZE);
  auto second_half_sample = NOM_SCAST(int16*, samples)[offset]; // offset = 22050
  EXPECT_EQ(3, second_half_sample)
  << "read chunk offset: " << offset;

  EXPECT_EQ(READ_SIZE, first_half);
  EXPECT_EQ(READ_SIZE, second_half);

  if(TEST_CHUNK_PLAYBACK == true) {
    // audio::set_volume(4.0f, dev);
    // EXPECT_EQ(4.0f, audio::volume(dev));
    test::play_audio(samples, metadata, dev);
  } else {
    audio::free_samples(metadata.channel_format, samples);
  }

  fp->close();
  // TODO(jeff): cppcheck reports `fp` as a memory leak; my own visual
  // examination agrees with the report. Verify this to be true!
#if 0
  NOM_DELETE_PTR(fp);
#endif

  if(TEST_CHUNK_PLAYBACK == true) {
    audio::shutdown_audio(dev);
  }
}

// TODO(jeff): Dedicate to SoundFileReaderTest::Seek test executable
TEST_F(ALAudioTest, SoundFileReader_Seek)
{
  const auto AUDIO_FILENAME = AUDIO_RESOURCES["chunk"];
  nom::size_type offset = 0;

  audio::SoundInfo metadata = {};
  nom::Timer elapsed;

  audio::IOAudioEngine* dev = nullptr;
  audio::AudioSpec spec = {};
  if(TEST_CHUNK_PLAYBACK == true) {
    dev = audio::init_audio(&this->openal_request, &spec);
    ASSERT_TRUE(dev != nullptr);
  }

  audio::ISoundFileReader* fp = new audio::SoundFileReader();
  ASSERT_TRUE(fp != nullptr);

  EXPECT_EQ(true, fp->open(AUDIO_FILENAME, metadata))
  << "Could not load audio from input file " << AUDIO_FILENAME;
  EXPECT_EQ(true, fp->valid());

  offset = fp->seek(0, audio::SOUND_SEEK_CUR);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(metadata.frame_count, fp->seek(0, audio::SOUND_SEEK_END));

  const nom::size_type CHUNK_SIZE = metadata.total_bytes / 2; // 44100
  EXPECT_TRUE(CHUNK_SIZE == metadata.sample_rate)
  << "The data read chunk size should match the input audio sample rate";

  EXPECT_TRUE(CHUNK_SIZE*2 == metadata.total_bytes)
  << "The data read chunk size should match the total size of the input audio";

  // Create a buffer that is large enough to read in both data chunks
  const nom::size_type READ_SIZE = (CHUNK_SIZE / 2); // 22050
  // void* samples = new int16[metadata.total_bytes/2];
  void* samples = new int16[ (READ_SIZE * 2) * metadata.channel_count];
  ASSERT_TRUE(samples != nullptr);

  // Seek to halfway through the audio sample
  offset = fp->seek(READ_SIZE, audio::SOUND_SEEK_SET);
  EXPECT_EQ(READ_SIZE, offset);

  auto first_half =
    fp->read( NOM_SCAST(int16*, samples), metadata.channel_format, READ_SIZE);
  EXPECT_EQ(READ_SIZE, first_half);

  if(TEST_CHUNK_PLAYBACK == true) {
    // audio::set_volume(4.0f, dev);
    // EXPECT_EQ(4.0f, audio::volume(dev));
    test::play_audio(samples, metadata, dev);
  } else {
    audio::free_samples(metadata.channel_format, samples);
  }

  fp->close();
  // TODO(jeff): cppcheck reports `fp` as a memory leak; my own visual
  // examination agrees with the report. Verify this to be true!
#if 0
  NOM_DELETE_PTR(fp);
#endif
  if(TEST_CHUNK_PLAYBACK == true) {
    audio::shutdown_audio(dev);
  }
}

TEST_F(ALAudioTest, SoundFileReader_Metadata)
{
  nom::Timer elapsed;

  for(auto itr = AUDIO_RESOURCES.begin(); itr != AUDIO_RESOURCES.end();
      ++itr)
  {
    audio::SoundInfo metadata = {};
    audio::SoundFileReader* fp = new audio::SoundFileReader();
    ASSERT_TRUE(fp != nullptr);

    EXPECT_EQ(true, fp->open(itr->second, metadata))
    << "Could not load audio from input file " << itr->second;

    EXPECT_EQ(true, fp->valid());
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST,
                  "    Audio resource:", itr->second);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "title:",
                  metadata.tags.title);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "copyright:",
                  metadata.tags.copyright);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "artist:",
                  metadata.tags.artist);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "comment:",
                  metadata.tags.comment);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "date:",
                  metadata.tags.date);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "album:",
                  metadata.tags.album);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "license:",
                  metadata.tags.license);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "track_number:",
                  metadata.tags.track_number);
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "genre:",
                  metadata.tags.genre);

    fp->close();
    // TODO(jeff): cppcheck reports `fp` as a memory leak; my own visual
    // examination agrees with the report. Verify this to be true!
  #if 0
    NOM_DELETE_PTR(fp);
  #endif
  }
}

// TODO(jeff): Dedicate to SoundFileWriterTest::RawWrite test executable
#if 0
TEST_F(ALAudioTest, DISABLED_SoundFileWriterTest_RawWrite)
{
  const auto AUDIO_FILENAME = AUDIO_RESOURCES["sine1s-900"];
  int16* samples = nullptr;
  SoundInfo metadata = {};

  // TODO:
  // ISoundFileReader* fpr = new SoundFileReader();
  // fpr->open(AUDIO_FILENAME, metadata);
  // ...
  // fpr->read(samples, ...);
  // fpw->open(AUDIO_FILENAME + ".raw");
  // fpw->write(samples, ...);
  // ...
  // fpw->close();
  // fpr->close();
  ISoundFileWriter* fpw = new SoundFileWriter();
  ASSERT_TRUE(fpw != nullptr);

  EXPECT_EQ(true, fpw->open(AUDIO_FILENAME, metadata))
  << "Could not load audio from input file " << AUDIO_FILENAME;

  EXPECT_EQ(true, fpw->valid());

  // const nom::size_type CHUNK_SIZE = metadata.total_bytes;
  // buffer = new int16[CHUNK_SIZE * metadata.channel_count];

  // EXPECT_NE(0, fp->read(buffer, CHUNK_SIZE))
  // << "Could not read audio samples from input file " << AUDIO_FILENAME;

  fpw->close();

  // EXPECT_EQ(-15, NOM_SCAST(int16*, buffer)[0]);
  // EXPECT_EQ(-5, NOM_SCAST(int16*, buffer)[1]);

  nom::free_samples(metadata.channel_format, samples);
}
#endif

TEST_F(ALAudioTest, NullAudioDevice)
{
  audio::AudioSpec spec = {};
  audio::IOAudioEngine* dev =
    audio::init_audio(nullptr, &spec);
  ASSERT_TRUE(dev == nullptr);
  // EXPECT_EQ(false, dev->valid());

  // FIXME
  // EXPECT_EQ("NullAudioDevice", dev->device_name() );
  EXPECT_EQ(0.0f, audio::volume(dev));
  EXPECT_EQ(Point3f(0,0,0), audio::position(dev));

  audio::shutdown_audio(dev);
}

TEST_F(ALAudioTest, NullAudioVolume)
{
  audio::AudioSpec spec = {};
  audio::IOAudioEngine* dev =
    audio::init_audio(nullptr, &spec);
  ASSERT_TRUE(dev == nullptr);
  // EXPECT_EQ(false, dev->valid());

  // FIXME
  // EXPECT_EQ("NullAudioDevice", dev->device_name() );
  EXPECT_EQ(0.0f, audio::volume(dev));
  EXPECT_EQ(Point3f(0,0,0), audio::position(dev));
  EXPECT_EQ(0.0f, audio::volume(dev));
  EXPECT_EQ(Point3f(0,0,0), audio::position(dev));

  audio::shutdown_audio(dev);
}

TEST_F(ALAudioTest, NullSoundBuffer)
{
  audio::AudioSpec spec = {};
  audio::IOAudioEngine* dev =
    audio::init_audio(nullptr, &spec);
  ASSERT_TRUE(dev == nullptr);
  // EXPECT_EQ(false, dev->valid());

  audio::SoundBuffer* buffer = nullptr;

  buffer = audio::create_buffer_memory();
  EXPECT_TRUE(buffer != nullptr);

  EXPECT_EQ(0, audio::buffer_id(buffer));
  EXPECT_EQ(0, audio::source_id(buffer));
  EXPECT_EQ(0, buffer->duration);
  EXPECT_NE(audio::AUDIO_STATE_LOOPING, audio::state(buffer, dev) );

  audio::free_buffer(buffer, dev);
  EXPECT_TRUE(buffer != nullptr);

  audio::shutdown_audio(dev);
}

TEST_F(ALAudioTest, NullSound)
{
  audio::AudioSpec spec = {};
  audio::IOAudioEngine* dev =
    audio::init_audio(nullptr, &spec);
  ASSERT_TRUE(dev == nullptr);
  // EXPECT_EQ(false, dev->valid());

  auto buffer = audio::create_buffer_memory();
  EXPECT_TRUE(buffer != nullptr);

  EXPECT_EQ(audio::AUDIO_STATE_STOPPED, audio::pitch(buffer, dev));
  EXPECT_FLOAT_EQ(0.0f, audio::volume(buffer, dev));
  EXPECT_FLOAT_EQ(0.0f, audio::min_volume(buffer, dev));
  EXPECT_FLOAT_EQ(0.0f, audio::max_volume(buffer, dev));
  EXPECT_FLOAT_EQ(0.0f, audio::pitch(buffer, dev));
  EXPECT_EQ(Point3f::zero, audio::position(buffer, dev));
  EXPECT_EQ(Point3f::zero, audio::velocity(buffer, dev));

  EXPECT_EQ(0.0f, audio::playback_position(buffer, dev));
  EXPECT_EQ(0.0f, audio::playback_samples(buffer, dev));

  EXPECT_EQ(0, audio::buffer_id(buffer));
  EXPECT_EQ(0, audio::source_id(buffer));

  audio::free_buffer(buffer, dev);
  EXPECT_TRUE(buffer != nullptr);

  audio::shutdown_audio(dev);
}

// TODO(jeff): Finish implementation
#if 1
TEST_F(ALAudioTest, NullMusic)
{
  audio::AudioSpec spec = {};
  audio::IOAudioEngine* dev = nullptr;
  audio::SoundBuffer* buffer = audio::create_buffer_memory();
  // audio::SoundBuffer* buffer = nullptr;
  ASSERT_TRUE(buffer != nullptr);

  dev = audio::init_audio(&this->null_request, &spec);
  ASSERT_TRUE(dev == nullptr);

  EXPECT_EQ(0, audio::buffer_id(buffer));
  EXPECT_EQ(0, audio::source_id(buffer));
  EXPECT_TRUE(buffer->samples == nullptr);

  EXPECT_EQ(audio::AUDIO_STATE_STOPPED, audio::state(buffer, dev));
  EXPECT_EQ(0.0f, audio::pitch(buffer, dev));
  EXPECT_EQ(0.0f, audio::volume(buffer, dev));
  EXPECT_EQ(0.0f, audio::min_volume(buffer, dev));
  EXPECT_EQ(0.0f, audio::max_volume(buffer, dev));
  EXPECT_EQ( Point3f::zero, audio::position(buffer, dev));
  EXPECT_EQ( Point3f::zero, audio::velocity(buffer, dev));
  // EXPECT_EQ( false, sound.getPositionRelativeToListener() );
  // EXPECT_EQ( 0.0f, sound.getMinDistance() );
  // EXPECT_EQ( 0.0f, sound.getAttenuation() );
  EXPECT_EQ(0.0f, audio::playback_position(buffer, dev));

  audio::free_buffer(buffer, dev);
  // EXPECT_TRUE(buffer != nullptr);

  audio::shutdown_audio(dev);
}
#endif

#if 0
TEST_F( ALAudioTest, NullAudioDeviceLocatorAPI)
{
  nom::AudioDeviceLocator::set_provider(nullptr);

  EXPECT_EQ( "NullAudioDevice", nom::AudioDeviceLocator::audio_device().getDeviceName() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, nom::AudioDeviceLocator::audio_device().getDeviceName() );
}
#endif

// EOF NullAudio tests

TEST_F(ALAudioTest, AudioOutputDeviceOpen)
{
  audio::AudioSpec spec = {};
  audio::IOAudioEngine* dev =
    audio::init_audio(&this->openal_request, &spec);
  ASSERT_TRUE(dev != nullptr);

  EXPECT_NE("", spec.name);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, spec.name);

  audio::shutdown_audio(dev);
}

TEST_F(ALAudioTest, GlobalAudioOutputVolume)
{
  audio::AudioSpec spec = {};
  audio::IOAudioEngine* dev =
    audio::init_audio(&this->openal_request, &spec);
  ASSERT_TRUE(dev != nullptr);

  EXPECT_EQ(100.0f, audio::volume(dev));
  EXPECT_EQ(Point3f(0.0f, 0.0f, 0.0f), audio::position(dev));

  audio::shutdown_audio(dev);
}

TEST_F(ALAudioTest, AudioOutputBufferVolume)
{
  const auto AUDIO_FILENAME = AUDIO_RESOURCES["sine1s-900"];

  audio::AudioSpec spec = {};
  audio::IOAudioEngine* dev =
    audio::init_audio(&this->openal_request, &spec);
  ASSERT_TRUE(dev != nullptr);

  EXPECT_EQ(100.0f, audio::volume(dev));

  audio::SoundBuffer* buffer =
    audio::create_buffer(AUDIO_FILENAME, dev);
  EXPECT_EQ(100.0f, audio::volume(dev));
  EXPECT_EQ(100.0f, audio::volume(buffer, dev));
  EXPECT_EQ(0.0f, audio::min_volume(buffer, dev));
  EXPECT_EQ(100.0f, audio::max_volume(buffer, dev));
  EXPECT_EQ(Point3f(0.0f, 0.0f, 0.0f), audio::position(buffer, dev));
  EXPECT_EQ(Point3f(0.0f, 0.0f, 0.0f), audio::velocity(buffer, dev));

  audio::free_buffer(buffer, dev);
  audio::shutdown_audio(dev);
}

TEST_F(ALAudioTest, AudioSpec)
{
  audio::IOAudioEngine* dev = nullptr;
  audio::AudioSpec request = {}; // input spec
  audio::AudioSpec spec = {}; // output spec

  request.name = "";
  request.sample_rate = 44100;
  request.engine = "openal";

  dev = audio::init_audio(&request, &spec);
  ASSERT_TRUE(dev != nullptr);
  EXPECT_EQ(true, dev->valid());
  EXPECT_NE("", spec.name);
  EXPECT_EQ(request.sample_rate, spec.sample_rate);

  audio::shutdown_audio(dev);

  request.sample_rate = 48000;
  dev = audio::init_audio(&request, &spec);
  ASSERT_TRUE(dev != nullptr);
  EXPECT_EQ(true, dev->valid());
  EXPECT_STREQ("openal", spec.engine);
  EXPECT_NE("", spec.name);
  EXPECT_EQ(48000, spec.sample_rate);

  auto driver = audio::current_device();
  // spec = audio::create_openal_attributes(driver);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "refresh rate (Hz):",
               spec.refresh_rate);

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "synchronous context (boolean):",
               spec.sync_context);

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "max sources:",
               audio::max_sources(driver));

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "max mono sources:",
               spec.num_mono_sources);

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "max stereo sources:",
               spec.num_stereo_sources);

  audio::shutdown_audio(dev);

  // TODO(jeff): Test these attributes out
#if 0
  request.refresh_rate = 30;
  request.sync_context = true;
  request.mono_sources = 100;
  request.stereo_sources = 100;
#endif
  // FIXME(jeff): Implement NULL audio hardware abstraction
  // request.name = "null device";
  // dev = audio::init_audio(&request, &spec);
  // ASSERT_TRUE(dev == nullptr);
  // EXPECT_STREQ("openal", spec.engine);
  // EXPECT_NE("", spec.name);
  // EXPECT_EQ(request.frequency, spec.frequency);

  // audio::shutdown_audio(dev);
}

TEST_F(ALAudioTest, SoundBuffer)
{
  const auto AUDIO_FILENAME = AUDIO_RESOURCES["sine1s-900"];

  audio::AudioSpec spec = {};
  audio::IOAudioEngine* dev =
    audio::init_audio(&this->openal_request, &spec);
  ASSERT_TRUE(dev != nullptr);

  audio::SoundBuffer* buffer = nullptr;
  buffer = audio::create_buffer(AUDIO_FILENAME, dev);
  ASSERT_TRUE(buffer != nullptr);
  EXPECT_FLOAT_NEAR(1.0f, buffer->duration, 0.01f);

  EXPECT_EQ(1, audio::buffer_id(buffer));
  EXPECT_EQ(2, audio::source_id(buffer));

  audio::free_buffer(buffer, dev);
  EXPECT_TRUE(buffer != nullptr);

  audio::shutdown_audio(dev);
}

TEST_F(ALAudioTest, Sound)
{
  const auto AUDIO_FILENAME = AUDIO_RESOURCES["sine1s-900"];

  audio::AudioSpec spec = {};
  audio::IOAudioEngine* dev =
    audio::init_audio(&this->openal_request, &spec);
  ASSERT_TRUE(dev != nullptr);

  auto buffer = audio::create_buffer(AUDIO_FILENAME, dev);
  ASSERT_TRUE(buffer != nullptr);
  EXPECT_FLOAT_NEAR(1.0f, buffer->duration, 0.01f);

  EXPECT_EQ(audio::AUDIO_STATE_STOPPED, audio::state(buffer, dev));
  EXPECT_FLOAT_EQ(100.0f, audio::volume(buffer, dev));
  EXPECT_FLOAT_EQ(0.0f, audio::min_volume(buffer, dev));
  EXPECT_FLOAT_EQ(100.0f, audio::max_volume(buffer, dev));
  EXPECT_FLOAT_EQ(1.0f, audio::pitch(buffer, dev));
  EXPECT_EQ(Point3f::zero, audio::position(buffer, dev));
  EXPECT_EQ(Point3f::zero, audio::velocity(buffer, dev));

  EXPECT_NE(0, audio::buffer_id(buffer));
  EXPECT_NE(0, audio::source_id(buffer));

  EXPECT_EQ(0.0f, audio::playback_position(buffer, dev));
  EXPECT_EQ(0.0f, audio::playback_samples(buffer, dev));

  audio::free_buffer(buffer, dev);
  audio::shutdown_audio(dev);
}
#if 0
TEST_F(ALAudioTest, Music)
{
  Music sound;
  dev = test::create_audio_handle();

  // auto buffer = audio::create_buffer(RESOURCE_AUDIO_SOUND);
  // ASSERT_TRUE(buffer != nullptr);
  EXPECT_FLOAT_NEAR(1.0f, buffer->duration, 0.01f);

  sound.load_file(RESOURCE_AUDIO_SOUND);
  EXPECT_EQ(SOUND_STOPPED, sound.status() );

  // nom::free_buffer(buffer, dev);
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

// TODO(jeff): Test non-NULL dev pointer passing
TEST_F(ALAudioTest, DefaultOutputDeviceName)
{
  audio::AudioSpec spec = {}; // output
  const char* dev_name = nullptr;
  audio::IOAudioEngine* dev = nullptr;

  // dev = audio::init_audio(&this->openal_request, &spec);
  // ASSERT_TRUE(dev != nullptr);

  dev_name = audio::default_output_device_name(nullptr);
  ASSERT_TRUE(dev_name != nullptr);

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST,
               "Default output audio device:", dev_name);

  audio::shutdown_audio(dev);
}

// TODO(jeff): Test non-NULL dev pointer passing to output_device_names
TEST_F(ALAudioTest, ListOutputDeviceNames)
{
  audio::AudioSpec spec = {}; // output
  audio::device_name_list dev_names;
  audio::IOAudioEngine* dev = nullptr;

  // dev = audio::init_audio(&this->openal_request, &spec);
  // ASSERT_TRUE(dev != nullptr);

  dev_names = audio::output_device_names(nullptr);

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "Available output audio devices:");
  for(auto itr = dev_names.begin(); itr != dev_names.end(); ++itr) {
    NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, *itr);
  }

  audio::shutdown_audio(dev);
}

// TODO(jeff): Test non-NULL dev pointer passing to input_device_names
TEST_F(ALAudioTest, ListInputDeviceNames)
{
  audio::AudioSpec spec = {}; // output
  audio::device_name_list dev_names;
  audio::IOAudioEngine* dev = nullptr;

  // dev = audio::init_audio(&this->openal_request, &spec);
  // ASSERT_TRUE(dev != nullptr);

  dev_names = audio::input_device_names(nullptr);

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "Available input audio devices:");
  for(auto itr = dev_names.begin(); itr != dev_names.end(); ++itr) {
    NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, *itr);
  }

  audio::shutdown_audio(dev);
}

// TODO(jeff): Test non-NULL dev pointer passing
TEST_F(ALAudioTest, DefaultInputDeviceName)
{
  audio::AudioSpec spec = {}; // output
  const char* dev_name = nullptr;
  audio::IOAudioEngine* dev = nullptr;

  // dev = audio::init_audio(&this->openal_request, &spec);
  // ASSERT_TRUE(dev != nullptr);

  dev_name = audio::default_input_device_name(nullptr);
  ASSERT_TRUE(dev_name != nullptr);

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST,
               "Default input audio device:", dev_name);

  audio::shutdown_audio(dev);
}

// TODO(jeff): Implement the following tests:
// ALAudioTest, InputDeviceNames

#if defined(NOM_USE_APPLE_OPENAL)
#include "nomlib/audio/AL/osx/apple_extensions.hpp"
TEST_F(ALAudioTest, MacOSX_OpenALExtensions)
{
  audio::AudioSpec spec = {}; // output
  const char* dev_name = nullptr;
  audio::IOAudioEngine* dev = nullptr;

  this->openal_request.num_mono_sources = 32;
  this->openal_request.num_stereo_sources = 16;
  dev = audio::init_audio(&this->openal_request, &spec);
  ASSERT_TRUE(dev != nullptr);

  int max_sources = audio::osx_max_sources();
  EXPECT_EQ(64, max_sources);
  NOM_DUMP(max_sources);

  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST,
               "max sources:", max_sources);

  audio::shutdown_audio(dev);
}
#endif

//
// Scratch tests
//

typedef void signature_t(void* data, nom::size_type byte_size);
signature_t* func = nullptr;

void null_func_signature(void* data, nom::size_type bytes)
{
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "NULL");
}

void func_signature(void* data, nom::size_type bytes)
{
  if(data) {
    NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "data:", (const char*)data);
  }
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "number of bytes:", bytes);
}

TEST_F(ALAudioTest, Scratch)
{
  void* data = NOM_CCAST(char*, nom::create_string("boobies"));

#if 1
  func = func_signature;
#else
  func = nullptr;
#endif

  if(func == nullptr) {
    func = null_func_signature;
  }

  func(data, 4);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  NOM_ASSERT(nom::init(argc, argv) == true);
  atexit(nom::quit);

  return RUN_ALL_TESTS();
}

