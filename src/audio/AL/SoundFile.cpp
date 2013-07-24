/******************************************************************************

    OpenAL SoundFile

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/audio/AL/SoundFile.hpp"

namespace nom {
  namespace OpenAL {

SoundFile::SoundFile ( void )
{
NOM_LOG_CLASSINFO;

  this->fp.reset();

  // Initialize here
}

SoundFile::~SoundFile ( void )
{
NOM_LOG_CLASSINFO;
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

size_t SoundFile::getDataByteSize ( void ) const
{
  return this->getSampleCount() * sizeof ( int16 );
}

bool SoundFile::open ( const std::string& filename )
{
  SF_INFO info;

  this->fp = std::shared_ptr<SNDFILE> ( sf_open ( filename.c_str(), SFM_READ, &info ), sf_close );

  if ( this->fp.get() == nullptr )
  {
NOM_LOG_ERR ( "Could not not audio file: " + filename );
    return false;
  }

  this->channel_count = info.channels;
  // sample_count should be the same size as samples
  this->sample_count = info.frames * info.channels;
  this->sample_rate = info.samplerate;

  switch ( info.channels )
  {
    default: this->channel_format = 0; break;
    case 1: this->channel_format = alGetEnumValue ( "AL_FORMAT_MONO16" ); break;
    case 2: this->channel_format = alGetEnumValue ( "AL_FORMAT_STEREO16" ); break;
    case 4: this->channel_format = alGetEnumValue ( "AL_FORMAT_QUAD16" ); break;
    case 6: this->channel_format = alGetEnumValue ( "AL_FORMAT_51CHN16" ); break;
    case 7: this->channel_format = alGetEnumValue ( "AL_FORMAT_61CHN16" ); break;
    case 8: this->channel_format = alGetEnumValue ( "AL_FORMAT_71CHN16" ); break;
  }

  return true;
}

bool SoundFile::read ( std::vector<int16>& data )
{
  size_t read_size = 0;
  std::vector<int16> read_buffer;
  read_buffer.resize ( BUFFER_SIZE );

  while ( ( read_size = sf_read_short ( this->fp.get(), read_buffer.data(), read_buffer.size() ) ) != 0 )
  {
    data.insert ( data.end(), read_buffer.begin(), read_buffer.begin() + read_size);
  }

  return true;
}


  } // namespace OpenAL
} // namespace nom
