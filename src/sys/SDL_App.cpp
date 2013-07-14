/******************************************************************************

    SDL app interface class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_App.hpp"

namespace nom {

SDL_App::SDL_App ( void )
{
NOMLIB_LOG_INFO;

  this->app_state = false;

  this->showFPS ( true );

  this->setFullScreen ( false );

  this->appTime.Start();
}

SDL_App::~SDL_App ( void )
{
NOMLIB_LOG_INFO;

  this->setFullScreen ( false );

  this->appTime.Stop();

  this->app_state = false;
}

bool SDL_App::isRunning ( void )
{
  if ( this->app_state )
    return true;
  else
    return false;
}

void SDL_App::Running ( void )
{
  this->app_state = true;
}

void SDL_App::Quit ( void )
{
  this->app_state = false;
}

void SDL_App::onQuit ( void )
{
  this->Quit();
}

void SDL_App::onEvent ( SDL_Event* event )
{
  SDL_Input::HandleInput ( event );
}

uint32_t SDL_App::getTicks ( void )
{
  return this->appTime.getTicks();
}

bool SDL_App::getShowFPS ( void )
{
  return this->show_fps;
}

void SDL_App::showFPS ( bool toggle )
{
  this->show_fps = toggle;
}

// Helper method; toggles showing FPS counter (or not)
void SDL_App::toggleFPS ( void )
{
  if ( this->getShowFPS() )
    this->showFPS ( false );
  else
    this->showFPS ( true );
}

bool SDL_App::isFullScreen ( void )
{
  if ( this->fullscreen )
    return true;
  else
    return false;
}

void SDL_App::setFullScreen ( bool toggle )
{
  this->fullscreen = toggle;
}

bool SDL_App::PollEvents ( SDL_Event* event )
{
  if ( SDL_PollEvent ( event ) )
  {
    return true;
  }

  return false;
}

void SDL_App::enableKeyRepeat ( int32 delay, int32 interval )
{
  SDL_EnableKeyRepeat ( delay, interval );
}


} // namespace nom
