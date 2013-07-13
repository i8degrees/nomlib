/******************************************************************************

    SDL-based Timer Implementation

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Timer.hpp"

nom::Timer::Timer ( void )
{
//NOMLIB_LOG_INFO;

  if ( SDL_InitSubSystem ( SDL_INIT_TIMER ) == -1 )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
  }

  this->started = false;
  this->paused = false;
  this->elapsed_ticks = 0;
  this->paused_ticks = 0;
}

nom::Timer::~Timer ( void )
{
//NOMLIB_LOG_INFO;

  SDL_QuitSubSystem ( SDL_INIT_TIMER );
}

void nom::Timer::Start ( void )
{
  this->elapsed_ticks = SDL_GetTicks ();
  this->started = true;
  this->paused = false;
}

void nom::Timer::Stop ( void )
{
  this->started = false;
  this->paused = false;
}

void nom::Timer::Pause ( void )
{
  if ( ( this->started == true ) && ( this->paused == false ) )
  {
    this->paused = true;
    this->paused_ticks = SDL_GetTicks() - this->elapsed_ticks;
  }
}

void nom::Timer::Unpause ( void )
{
  if ( this->paused == true )
  {
    this->paused = false;
    this->elapsed_ticks = SDL_GetTicks() - this->paused_ticks;
    this->paused_ticks = 0;
  }
}

unsigned int nom::Timer::getTicks ( void )
{
  if ( this->started == true )
  {
    if ( this->paused == true )
    {
      return this->paused_ticks;
    }
    else
    {
      return SDL_GetTicks() - this->elapsed_ticks;
    }
  }
  return SDL_GetTicks();
}

bool nom::Timer::isStarted ( void )
{
  return this->started;
}

bool nom::Timer::isPaused ( void )
{
  return this->paused;
}

uint32_t nom::Timer::seconds ( float seconds ) const
{
  return static_cast<uint32_t> ( seconds * 1000.f );
}
