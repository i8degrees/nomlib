/******************************************************************************
    audio.cpp

    SDL-based Audio Effects API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "audio.h"

Audio::Audio ( unsigned int rate, Uint16 format, unsigned int channels, unsigned int buffers )
{
  #ifdef DEBUG_AUDIO
    std::cout << "Hello, world! <From Audio::Audio>" << "\n" << std::endl;
  #endif
  // Memory allocation buffers for audio
  this->sound = NULL;
  this->music = NULL;

  if ( Mix_OpenAudio ( rate, format, channels, buffers ) == -1 )
  {
    std::cout << "ERR: " << Mix_GetError() << std::endl;
  }
}

Audio::~Audio ( void )
{
  #ifdef DEBUG_AUDIO
    std::cout << "Goodbye cruel world! <From Audio::~Audio>" << "\n" << std::endl;
  #endif
  //this->sound = NULL;

  Mix_HaltMusic ();
  Mix_FreeMusic ( this->music );
  this->music = NULL;

  Mix_CloseAudio ();
}

/*
bool Audio::LoadSoundTrack ( std::string filename )
{
  this->sound = Mix_LoadWAV ( filename.c_str() );

  if ( ! this->sound )
  {
    std::cout << "ERR: " << Mix_GetError() << std::endl;
    return false;
  }

  return true;
}
*/

/*
  FIXME: -1 is *NOT* a good choice of a mix_channel for this method at the moment.
*/
bool Audio::PlaySoundTrack ( std::string filename, signed int mix_channel, signed int loops )
{
  this->sound = Mix_LoadWAV ( filename.c_str() );

  if ( ! this->sound )
  {
    std::cout << "ERR: " << Mix_GetError() << std::endl;
    return false;
  }

  Mix_PlayChannel ( mix_channel, this->sound, loops );

  Mix_ChannelFinished ( soundFinished );

  return true;
}

bool Audio::LoadMusicTrack ( std::string filename )
{
  this->music = Mix_LoadMUS ( filename.c_str() );

  if ( ! this->music )
  {
    std::cout << "ERR: " << Mix_GetError() << std::endl;
  }

  return true;
}

bool Audio::PlayMusicTrack ( signed int loops )
{
  Mix_PlayMusic ( this->music, loops );

  return true;
}

bool Audio::PauseMusic ( void )
{
  Mix_PauseMusic ();

  return true;
}

bool Audio::togglePlayingMusic ( void )
{
  if ( Mix_PausedMusic () == 1 )
  {
    // Mix_FadeInMusic ( this->music, -1, 2000 );
    Mix_ResumeMusic ();
  }
  else
  {
    //Mix_FadeOutMusic ( this->milliseconds );
    //Mix_FadeOutMusic ( this->music, this->music_loops, this->milliseconds );
    Mix_PauseMusic ();
  }

  return true;
}

// Fielding's Deriving Audio Methods

void Audio::setMusicVolume ( int volAsPercent )
{
  Mix_VolumeMusic ( ceil (MIX_MAX_VOLUME * ( volAsPercent / 100.f )) );  // set the music volume based on the percent passed to the method
}

int Audio::getMusicVolume ( void )
{
  return floor ( ( Mix_VolumeMusic (-1) / 128.f ) * 100 );    // return the current music volume as a percent
}

void Audio::setChannelVolume ( signed int mix_channel, int volAsPercent )
{
  Mix_Volume(mix_channel, ceil( MIX_MAX_VOLUME * ( volAsPercent / 100.f )) );  // set the music volume based on the percent passed to the method
}

int Audio::getChannelVolume ( signed int mix_channel )
{
  return floor ( ( Mix_Volume (mix_channel, -1) / 128.f ) * 100 );    // return the current music volume as a percent
}

void Audio::soundFinished ( signed int mix_channel )
{
  Mix_FreeChunk ( Mix_GetChunk ( mix_channel ) );
}
