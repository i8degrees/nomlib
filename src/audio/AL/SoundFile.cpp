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
#include "nomlib/audio/AL/SoundFile.hpp"

namespace nom {

SoundFile::SoundFile ( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  this->fp.reset();

  // Initialize here
}

SoundFile::~SoundFile ( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );
  // Clean up instance variables
}

sf_count_t SoundFile::getSampleCount ( void ) const
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

sf_count_t SoundFile::getDataByteSize ( void ) const
{
  return this->getSampleCount() * sizeof ( int16 );
}

bool SoundFile::open ( const std::string& filename )
{
  SF_INFO info;

  this->fp = std::shared_ptr<SNDFILE> ( sf_open ( filename.c_str(), SFM_READ, &info ), sf_close );

  if ( this->fp.get() == nullptr )
  {
NOM_LOG_ERR ( NOM, "Could not not audio file: " + filename );
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
  sf_count_t read_size = 0;
  std::vector<int16> read_buffer;
  read_buffer.resize ( BUFFER_SIZE );

  while ( ( read_size = sf_read_short ( this->fp.get(), read_buffer.data(), read_buffer.size() ) ) != 0 )
  {
    data.insert ( data.end(), read_buffer.begin(), read_buffer.begin() + read_size);
  }

  return true;
}

} // namespace nom
