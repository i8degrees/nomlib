#include "nomlib/audio/StreamSound.hpp"
#include "nomlib/audio/libsndfile/SoundFileReader.hpp"

// Private dependency
#include "nomlib/audio/AL/OpenAL.hpp"

using namespace nom;
using namespace nom::audio;

namespace nom {
namespace audio {

StreamSound::StreamSound()
{
  NOM_LOG_TRACE(NOM);

  this->streaming_ = true;
  this->fp_ = new SoundFileReader();
}

StreamSound::~StreamSound()
{
#if defined(DEBUG)
  std::cout << "StreamSound::~StreamSound(" << this->filename_ << ")" <<
    std::endl;
#endif

  this->close();
  NOM_DELETE_PTR(this->fp_);
}

void StreamSound::close()
{
  NOM_ASSERT(this->fp_ != nullptr);

  this->fp_->close();
}

StreamSound::StreamSound(const std::string& filename)
{
  std::cout << "StreamSound::StreamSound(" << filename << ")" << std::endl;
  this->open(filename);
}

bool StreamSound::open(const std::string& filename)
{
  std::cout << "StreamSound::open(" << filename << ")" << std::endl;
  this->fp_ = new SoundFileReader();

  NOM_ASSERT(this->fp_ != nullptr);
  if(this->fp_ == nullptr) {
    // TODO(JEFF): Handle out of memory allocation error
    return false;
  }

#if defined(DEBUG)
  this->filename_ = filename;
#endif
  this->streaming_ = true;
  this->init(filename);
  return true;
}

// not impl
StreamSound::StreamSound(int16* samples, uint32 format, int channels)
{
  this->fp_ = new SoundFileReader();
  this->streaming_ = true;
  this->init(samples, format, channels);
}

real64 StreamSound::stream(uint32 buffer, real64 pos)
{
  NOM_ASSERT(this->fp_ != nullptr);
  if(this->fp_ == nullptr) {
    return 0.0;
  }

  // 'pos' is seconds remaining until EOF, flowing from duration() down to
  // zero. We rely on the file cursor advancing sequentially across calls;
  // explicit seeking is not needed for the engine's linear streaming model.
#if defined(DEBUG)
  std::cout << "StreamSound::stream(" << buffer << ", " << pos << ")" << std::endl;
#endif

  int16* pcmData = this->data();
  size_type frames_buffered = 0;
  size_type bytes_buffered = 0;

  // Fill one BUFFERLENGTH byte chunk (matching the old OggSound chunk size).
  const size_type bytes_per_frame = this->channels() * sizeof(int16);
  while(bytes_buffered < BUFFERLENGTH) {
    size_type bytes_remaining = BUFFERLENGTH - bytes_buffered;
    size_type frames_remaining = bytes_remaining / bytes_per_frame;
    if(frames_remaining == 0) {
      break;
    }

    int64 read_result = this->fp_->read(
      pcmData + (frames_buffered * this->channels()), AUDIO_FORMAT_S16,
      static_cast<int64>(frames_remaining));

    if(read_result > 0) {
      frames_buffered += static_cast<size_type>(read_result);
      bytes_buffered = frames_buffered * bytes_per_frame;
    } else if(read_result == 0) {
      std::cout << "read result was zero" << std::endl;
      break; // EOF
    } else {
      std::cerr << "ERROR: stream decoding failure" << std::endl;
      return 0.0;
    }
  }

  if(frames_buffered == 0) {
    return 0.0;
  }

  // alBufferData expects the size in bytes.
  alBufferData(buffer, this->format(), pcmData,
    static_cast<ALsizei>(bytes_buffered), this->frequency());

#if defined(DEBUG)
  std::cout << "read frames: " << (real32)frames_buffered << std::endl;
#endif

  // bitrate() is the *compressed* rate for Vorbis/MP3, so derive the chunk
  // duration from the decoded PCM frame count and sample rate.
  if(this->frequency() <= 0.0f) {
    return 0.0;
  }

  // Return the duration of the buffered audio (seconds).
  return static_cast<real64>(frames_buffered) / this->frequency();
}

// Private scope - member methods

void StreamSound::init(const std::string& filename)
{
  super::init(filename);

  this->samples_ = new int16[BUFFERLENGTH * this->channels() * sizeof(int16)];
  // this->fp_->read(this->samples_, AUDIO_FORMAT_S16, (BUFFERLENGTH*2));
}

void StreamSound::init(int16* samples, uint32 format, int channels)
{
  super::init(samples, format, channels);
}

} // namespace audio
} // namespace nom
