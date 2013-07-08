/******************************************************************************

    OpenAL SoundFile

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "OpenAL_SoundFile.hpp"

namespace nom {

OpenAL_SoundFile::OpenAL_SoundFile ( void )
{
NOMLIB_LOG_INFO;

  this->fp.reset();

  // Initialize here
}

OpenAL_SoundFile::~OpenAL_SoundFile ( void )
{
NOMLIB_LOG_INFO;

  // Clean up instance variables
}

std::size_t OpenAL_SoundFile::getSampleCount ( void ) const
{
  return this->sample_count;
}

uint32 OpenAL_SoundFile::getChannelCount ( void ) const
{
  return this->channel_count;
}

uint32 OpenAL_SoundFile::getSampleRate ( void ) const
{
  return this->sample_rate;
}

bool OpenAL_SoundFile::loadFromFile ( const std::string& filename )
{
  SF_INFO info;
  size_t read_size = 0;

  this->fp = std::shared_ptr<SNDFILE> ( sf_open ( filename.c_str(), SFM_READ, &info ), sf_close );

  this->channel_count = info.channels;
  this->sample_rate = info.samplerate;
  this->sample_count = static_cast<std::size_t> ( info.frames ) * info.channels;

  this->read_buffer.resize ( 4096 ); // default buffer size of 4096 bytes

  while ( ( read_size = sf_read_short ( this->fp.get(), this->read_buffer.data(), this->read_buffer.size() ) ) != 0 )
  {
    this->snd.insert ( this->snd.end(), this->read_buffer.begin(), this->read_buffer.begin() + read_size);
  }

  return true;
}

} // namespace nom
