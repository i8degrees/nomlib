#include "nomlib/audio2/StreamSound.hpp"
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
#if defined(DEBUG)
  real32 seek_pos = ((this->duration()) - pos);
  std::cout << "StreamSound::stream(" << buffer << ", " << pos << ")" <<
    "seek_pos: " << seek_pos << std::endl;
#endif

  //std::vector<int16*> pcmData(BUFFERLENGTH);
  // int16* pcmData = new int16[BUFFERLENGTH * this->channels() * sizeof(int16)];
  int16* pcmData = this->data();
  // for(auto idx = 0; idx != 256; ++idx) {
  //   std::cout << NOM_SCAST(int16*, pcmData)[idx] << "\n";
  // }
  size_type bytes_read = 0;

  while(bytes_read < BUFFERLENGTH) {
    int64 read_result = this->fp_->read(pcmData + bytes_read, AUDIO_FORMAT_S16,
      static_cast<int>(BUFFERLENGTH - bytes_read));

    if(read_result > 0) {
      bytes_read += read_result;
    } else if(read_result == 0) {
      std::cout << "read result was zero" << std::endl;
      break; // EOF
    } else {
      std::cerr << "ERROR: stream decoding failure" << std::endl;
      return 0.0f;
      //return false;
    }

    if(bytes_read > 0) {
      alBufferData(buffer, this->format(), pcmData,
        static_cast<ALsizei>(bytes_read), this->frequency());
      //return true;
    }
  }
#if defined(DEBUG)
  std::cout << "read pos: " << (real32)bytes_read << std::endl;
  std::cout << "stream pos: " << seek_pos << std::endl;
#endif
  real32 byte_rate = (this->bitrate() / 8.0f);
  byte_rate = (16 / 8.0f);

  // FIXME(JEFF): Need to test this new equation!
  // The bits per second divided by 8 gives us the byte rate per second
  // times the total duration minus the time remaining
  //auto time_cursor = (this->bitrate() / 8) * (this->duration() - pos);

  // return the current position of the stream in seconds
  // return((real32)bytes_read / (this->channels() * this->frequency() * (this->bitrate() / 8.0)));
  return((real32)bytes_read / (this->channels() * this->frequency() * byte_rate));
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
