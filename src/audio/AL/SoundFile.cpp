/******************************************************************************

    OpenAL SoundFile

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SoundFile.hpp"

namespace nom {
  namespace OpenAL {

SoundFile::SoundFile ( void )
{
//NOMLIB_LOG_INFO;

  this->fp.reset();

  // Initialize here
}

SoundFile::~SoundFile ( void )
{
//NOMLIB_LOG_INFO;

  // Clean up instance variables
}

size_t SoundFile::getSampleCount ( void ) const
{
  return this->sample_count;
}

uint32 SoundFile::getChannelCount ( void ) const
{
  return this->channel_count;
}

uint32 SoundFile::getSampleRate ( void ) const
{
  return this->sample_rate;
}

uint32 SoundFile::getChannelFormat ( void ) const
{
  return this->channel_format;
}

bool SoundFile::open ( const std::string& filename )
{
  SF_INFO info;

  this->fp = std::shared_ptr<SNDFILE> ( sf_open ( filename.c_str(), SFM_READ, &info ), sf_close );

  this->channel_count = info.channels;
  // sample_count should be the same size as samples
  this->sample_count = info.frames * info.channels;
  this->sample_rate = info.samplerate;

  switch ( info.channels )
  {
    default: break;

    case 1: this->channel_format = Channels::Mono; break;
    case 2: this->channel_format = Channels::Stereo; break;
  }

  return true;
}

bool SoundFile::read ( std::vector<int16>& data )
{
  size_t read_size = 0;
  std::vector<int16> read_buffer;
  read_buffer.resize ( 4096 );

  while ( ( read_size = sf_read_short ( this->fp.get(), read_buffer.data(), read_buffer.size() ) ) != 0 )
  {
    data.insert ( data.end(), read_buffer.begin(), read_buffer.begin() + read_size);
  }

  return true;
}


  } // namespace OpenAL
} // namespace nom
