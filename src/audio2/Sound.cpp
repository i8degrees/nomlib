
#include "nomlib/audio2/Sound.hpp"
//#include "ISoundFileReader.hpp"
#include "nomlib/audio/libsndfile/SoundFileReader.hpp"

// Forward declarations
// #include <sndfile.h>
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/audio/SoundFile.hpp"

using namespace nom;
using namespace nom::audio;

namespace nom {
namespace audio {

Sound::Sound()
{
  NOM_LOG_TRACE(NOM);
#if defined(DEBUG)
  this->filename_ = "";
#endif
  this->fp_ = new SoundFileReader();
  this->bitrate_ = 0;
  this->frequency_ = 0;
  this->duration_ = 0;
  this->samples_ = nullptr;
  this->buffer_id_ = 0;
  this->total_bytes_ = 0;
  // Part 2
  this->streaming_= false;
}

Sound::~Sound()
{
  NOM_LOG_TRACE(NOM);
  delete this->samples_;
#if defined(DEBUG)
  std::cout << "Sound::~Sound(" << this->filename_ << ", buffer=" <<
    this->buffer() << ")" << std::endl;
#endif
  AL_CLEAR_ERR();
  AL_CHECK_ERR(alDeleteBuffers(1, &this->buffer()));

  if(this->fp_ != nullptr) {
    this->fp_->close();
    NOM_DELETE_PTR(this->fp_);
  }
}

bool Sound::open(const std::string& filename)
{
  std::cout << "Sound::open(" << filename << ")" << std::endl;
#if defined(DEBUG)
  this->filename_ = filename;
#endif
  this->bitrate_ = 0;
  this->frequency_ = 0;
  this->duration_ = 0;
  this->samples_ = nullptr;
  this->buffer_id_ = 0;
  this->total_bytes_ = 0;

  this->fp_ = new SoundFileReader();

  // Part 2
  this->streaming_ = false;
  this->init(filename);

  return true;
}

Sound::Sound(const std::string& filename)
{
  this->open(filename);
}

void Sound::init(const std::string& filename)
{
  nom::audio::SoundInfo metadata;
  // This is done explicitly so that class instances derived from Sound, such
  // as OggSound, can still call Sound::init and not try to open another file
  // handle that will ultimately cause corruption at best, worse a segfault.

  if(this->fp_ == nullptr) {
    return;
  }

  if(this->fp_->open(filename, metadata) == false) {
    std::cerr << "Failed to load file at " << filename << std::endl;
    return;
  }

  this->sample_count_ = metadata.sample_count;
  this->total_bytes_ = metadata.total_bytes;
  this->duration_ = metadata.duration;
  //this->duration_ = (real32)this->duration_ * 1000.0f;
  this->num_channels_ = metadata.channel_count;
  this->frequency_ = (real32)metadata.sample_rate;
  this->format_ = metadata.channel_format; // AUDIO_FORMAT_XXX
  this->bitrate_ = metadata.bitrate;
#if defined(DEBUG)
  std::cout << "Sound(" << this->filename_ << ", " <<
    this->format() << ", " << this->channels() << ")" << std::endl;
#endif
  this->init(this->samples_, this->format_, this->channels());
}

void Sound::init(int16* samples, uint32 format, int num_channels) {
  std::cout << "Sound::init(samples)" << std::endl;
  // Heap allocation for holding the audio samples -- this is the base
  // implentation (non-streaming instance).
  // int16* pointer
  samples = this->allocate_samples(this->total_bytes(), num_channels);
  this->samples_ = samples;

  this->fp_->seek(0, SEEK_SET);
  if(this->streaming() == false) {
    auto sample_count = this->fp_->read(samples, format,
      this->sample_count());
    if(sample_count != this->sample_count()) {
  #if defined(DEBUG)
      std::cerr << "Sound::init(" << this->filename_ << ")" <<
        "Failed to load audio samples" << std::endl;
  #endif
      this->fp_->close();
    } else {
  #if defined(DEBUG)
      std::cout << "Sound::init(" << this->filename_ << ")" <<
        "sample_count is " << sample_count << std::endl;
  #endif
    }
  }

  if(this->streaming() == false) {
    this->fp_->close();
  }
}

//ALenum
int Sound::format() const
{
  ALenum format = AL_FORMAT_MONO16;

  switch(this->format_) {
    default:
    case AUDIO_FORMAT_UNKNOWN: {
      format = this->channels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    } break;

    case AUDIO_FORMAT_U8:
    case AUDIO_FORMAT_S8: {
      format = this->channels() == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
    } break;
    case AUDIO_FORMAT_S16: {
      format = this->channels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    } break;

    case AUDIO_FORMAT_S24: {
      //format = AL_FORMAT_MONO24;
    } break;
    case AUDIO_FORMAT_S32: {
      //format = ;
    } break;
    case AUDIO_FORMAT_R32: {
      //format = ;
    } break;
    case AUDIO_FORMAT_R64: {
      //format = ;
    } break;
  }

  return format;
}

real64 Sound::duration() const
{
  return this->duration_;
}

int64 Sound::sample_count() const
{
  return this->sample_count_;
}

int16* Sound::data()
{
  return this->samples_;
}

uint64 Sound::bitrate() const
{
  return this->bitrate_;
}

real32 Sound::frequency() const
{
  return this->frequency_;
}

int Sound::channels() const
{
  return this->num_channels_;
}

int Sound::total_bytes() const
{
  return this->total_bytes_;
}

nom::uint& Sound::buffer()
{
  return this->buffer_id_;
}

bool Sound::streaming() const
{
  return this->streaming_;
}

bool Sound::stream__(uint32 buffer, real64 pos)
{
  return false;
}

real64 Sound::stream(uint32 buffer, real64 pos)
{
  std::cout << "Sound::stream(uint32, real64)" << std::endl;
  return 0.0f;
}

// Private scope

int16* Sound::allocate_samples(size_type bytes, int channels)
{
  int16* samples = new int16[bytes * channels * sizeof(int16)];
  return samples;
}

// audio::AudioFormat fmt
void*
Sound::allocate_samples(size_type bytes, int num_channels, uint32 fmt)
{
  void* samples = nullptr;

  switch(fmt) {
    default:
    case AUDIO_FORMAT_UNKNOWN: {
      // ...Err state...
    } break;

    case AUDIO_FORMAT_S16: {
      samples = new int16[bytes * num_channels * sizeof(int16)];
    } break;

    case AUDIO_FORMAT_S8: {
      samples = new int16[bytes * num_channels * sizeof(int16)];
    } break;

    case AUDIO_FORMAT_U8: {
      samples = new int16[bytes * num_channels * sizeof(int16)];
    } break;

    case AUDIO_FORMAT_S24:
    case AUDIO_FORMAT_S32: {
      samples = new int32[bytes * num_channels * sizeof(int32)];
    } break;

    case AUDIO_FORMAT_R32: {
      samples = new real32[bytes * num_channels * sizeof(real32)];
    } break;

    case AUDIO_FORMAT_R64: {
      samples = new real64[bytes * num_channels * sizeof(real64)];
    } break;
  }

  return samples;
}

} // namespace audio
} // namespace nom
