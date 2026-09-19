
#include "nomlib/audio2/OggSound.hpp"
#include "nomlib/types.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"
#include <vector>

using namespace nom;
using namespace nom::audio;

namespace nom {
namespace audio {

OggSound::OggSound()
{
  std::cout << "OggSound::OggSound()" << std::endl;

  this->streaming_ = true;

  // NOTE(JEFF): This pointer to ISoundFileReader is not used in this
  // implementation and thus must not be initialized.
  this->fp_ = nullptr;
}

OggSound::~OggSound()
{
#if defined(DEBUG)
  std::cout << "OggSound::~OggSound(" << this->filename_ << ")" <<
    std::endl;
#endif
  this->close();
}

void OggSound::close() {
  if(ov_clear(&this->sfp_) != 0) {
#if defined(DEBUG)
    std::cerr << "DEBUG: " << "Failed to close opened OGG stream at " <<
      this->filename_ << std::endl;
#endif
  }
}

bool OggSound::open(const std::string& filename)
{
  std::cout << "OggSound::open(" << filename << ")" << std::endl;
#if defined(DEBUG)
  this->filename_ = filename;
#endif
  this->streaming_ = true;

  int error = ov_fopen(filename.c_str(), &this->sfp_);
  if(error != 0) {
    std::cerr << "Failed to open OGG stream from " << filename << std::endl;
    this->check_error(filename, error);
    // Do not need to call ::close if this call fails as per the API docs
    return false;
  }

  this->init(filename);

  return true;
}

OggSound::OggSound(const std::string& filename)
{
  this->open(filename);
}

// not impl
OggSound::OggSound(int16* samples, uint32 format, int channels)
{
  NOM_ASSERT(0 == 1 && "This method is not implemented");
}

real64 OggSound::stream(uint32 buffer, real64 pos)
{
#if defined(DEBUG)
  real32 seek_pos = ((this->duration()) - pos);
  std::cout << "OggSound::stream(" << buffer << ", " << pos << ")" <<
    "seek_pos: " << seek_pos << std::endl;
#endif

  // output buffer
  char pcmData[BUFFERLENGTH];
  int read = 0; // bytes
  int read_result = 0; // cursor
  // input data size
  // one of the following -- 1 for 8-bit samples; 2 for 16-bit samples;
  // typical value is 2
  int word = 2;
  // signed or unsigned input data type
  // one of the following -- 0 for unsigned; 1 for signed; typical value is 1
  int sign = 1; // AUDIO_FORMAT_S16
  int endian = 0; // big endian (1) || little endain (0)
  int section; // current logical bitstream

  while(read < BUFFERLENGTH) {
    read_result = ov_read(&this->sfp_, pcmData + read,
      static_cast<int>(BUFFERLENGTH - read), endian, word, sign, &section);

    if(read_result > 0) {
      read += read_result;
    } else if(read_result == 0) {
      // EOF
      break;
    } else {
      this->check_error(this->filename_, read_result);
      return 0.0f;
    }
  }

  if(read > 0) {
    AL_CLEAR_ERR();
    AL_CHECK_ERR(alBufferData(buffer, this->format(), pcmData,
      static_cast<ALsizei>(read), this->frequency()));
  }

#if defined(DEBUG)
  std::cout << "read pos: " << (real32)read << std::endl;
  std::cout << "stream pos: " << seek_pos << std::endl;
  //std::cout << "stream pos: " << (real32)read /
    //(this->channels() * this->frequency() * (this->bitrate() / 8.0)) <<
  //std::endl;
#endif
  // return the current position of the stream in seconds
  //return (real32)read / (this->channels() * this->frequency() * (this->bitrate() / 8.0)) * 1000.0f;
  return((real32)read / (this->channels() * this->frequency() * (this->bitrate() / 8.0)));
}

// Private scope - member methods

void OggSound::init(const std::string& filename)
{
  // IMPORTANT(JEFF): This must be done in order to not have a conflicting
  // file open by the underlying Sound class that OggSound inherits from.
  this->fp_ = nullptr;

  super::init(filename);

  // The desired logical bitstream
  int logical_bs = -1; // obtain totals from entire physical bitstream

  if(ov_seekable(&this->sfp_) == 0) {
    std::cerr << "DEBUG: " << "OGG stream at " << filename <<
      " is not seekable!" << std::endl;
    this->close();
    return;
  }

  vorbis_info* vorbisInfo = ov_info(&this->sfp_, logical_bs);
  if(vorbisInfo) {
    this->total_bytes_ = ov_raw_total(&this->sfp_, logical_bs);
    this->sample_count_ = ov_pcm_total(&this->sfp_, logical_bs);
    this->num_channels_ = vorbisInfo->channels;
    this->frequency_ = (real32)vorbisInfo->rate;
    this->format_ = this->format();

    // TODO(JEFF): Test the following, instead of fuding the bitrate as we have been!
    // this->bitrate_ = ov_bitrate(&this->sfp_, logical_bs);
    this->bitrate_ = vorbisInfo->bitrate_nominal;
    std::cout << "BITRATE is " << this->bitrate_ << std::endl;
    // FIXME(JEFF): Why is this variable fudged?
    this->bitrate_ = 16;

    this->duration_ = (real32) ov_time_total(&this->sfp_, logical_bs);
  } else {
    // TODO(JEFF): Handle out of memory allocation?
    std::cerr << "DEBUG: " << "Failed memory allocation for ov_info call " <<
      "with filename " << filename << std::endl;
    this->close();
    return;
  }

  auto seek_bytes = 0; // Beginning of audio samples
  ov_time_seek(&this->sfp_, seek_bytes);
}

void OggSound::init(int16* samples, uint32 format, int channels)
{
  super::init(samples, format, channels);
}

bool OggSound::stream__(uint32 buffer, real64 pos) 
{
}

// Private scope - member methods

void OggSound::check_error(const std::string& filename, int error_code)
{
  int error = error_code;

  if(error == OV_EREAD) {
    std::cerr << "OV_EREAD: A read from media returned an error." <<
      std::endl;
  } else if(error == OV_ENOTVORBIS) {
    std::cerr << "OV_ENOTVORBIS: Bitstream does not contain any Vorbis data." <<
      std::endl;
  } else if(error == OV_EVERSION) {
    std::cerr << "OV_EVERSION: Vorbis version mismatch." << std::endl;
  } else if(error == OV_EBADHEADER) {
    std::cerr << "OV_EBADHEADER: Invalid Vorbis bitstream header." <<
      std::endl;
  } else if(error == OV_EFAULT) {
    std::cerr << "OV_EFAULT: Internal logic fault; indicates a bug or " <<
      "heap/stack corruption." << std::endl;
  } else if(error == OV_FALSE) {
    std::cerr << "OV_FALSE: Not true, or no data available." << std::endl;
  } else if(error == OV_HOLE) {
    std::cerr << "OV_HOLE: Vorbisfile encountered missing or corrupt data "
      "in the bitstream. Recovery is normally automatic and this return "
    << "code is for informational purposes only." << std::endl;
  } else if(error == OV_EIMPL) {
    std::cerr << "OV_EIMPL: Not implemented. " << std::endl;
  } else if(error == OV_EINVAL) {
    std::cerr << "OV_EINVAL: Either an invalid argument, or uninitialized "
      << "argument passed to call." << std::endl;
  } else if(error == OV_EBADLINK) {
    std::cerr << "OV_EBADLINK: The given link exists in the Vorbis data " <<
      "stream, but is not decipherable due to garbage or corruption." <<
    std::endl;
  } else if(error == OV_ENOSEEK) {
    std::cerr << "OV_ENOSEEK: The given file is not seekable." << std::endl;
  }
}

} // namespace audio
} // namespace nom
