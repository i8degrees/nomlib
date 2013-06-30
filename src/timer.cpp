/******************************************************************************
    timer.cpp

    SDL-based Timer Implementation Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "timer.h"

Timer::Timer ( void )
{
  #ifdef DEBUG_TIMER_OBJ
    std::cout << "Timer::Timer (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  if ( SDL_InitSubSystem ( SDL_INIT_TIMER ) == -1 )
  {
    std::cout << "ERR in Timer::Timer() at: " << SDL_GetError() << std::endl;
    return;
  }

  this->started = false;
  this->paused = false;
  this->elapsed_ticks = 0;
  this->paused_ticks = 0;
}

Timer::~Timer ( void )
{
  #ifdef DEBUG_TIMER_OBJ
    std::cout << "Timer::~Timer (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  SDL_QuitSubSystem ( SDL_INIT_TIMER );
}

void Timer::Start ( void )
{
  this->elapsed_ticks = SDL_GetTicks ();
  this->started = true;
  this->paused = false;
}

void Timer::Stop ( void )
{
  this->started = false;
  this->paused = false;
}

void Timer::Pause ( void )
{
  if ( ( this->started == true ) && ( this->paused == false ) )
  {
    this->paused = true;
    this->paused_ticks = SDL_GetTicks() - this->elapsed_ticks;
  }
}

void Timer::Unpause ( void )
{
  if ( this->paused == true )
  {
    this->paused = false;
    this->elapsed_ticks = SDL_GetTicks() - this->paused_ticks;
    this->paused_ticks = 0;
  }
}

unsigned int Timer::getTicks ( void )
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
  return 0;
}

bool Timer::isStarted ( void )
{
  return this->started;
}

bool Timer::isPaused ( void )
{
  return this->paused;
}

void Timer::sleep ( float milliseconds )
{
  SDL_Delay ( milliseconds );
}

uint32_t Timer::seconds ( float seconds ) const
{
  return static_cast<uint32_t> ( seconds * 1000.f );
}
