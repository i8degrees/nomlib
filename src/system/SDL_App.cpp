/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/system/SDL_App.hpp"

namespace nom {

SDL_App::SDL_App ( void )
{
NOM_LOG_CLASSINFO;

  this->app_state = false;

  this->showFPS ( true );

  this->setFullScreen ( false );

  this->appTime.Start();
}

SDL_App::~SDL_App ( void )
{
NOM_LOG_CLASSINFO;

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
  Input::HandleInput ( event );
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
