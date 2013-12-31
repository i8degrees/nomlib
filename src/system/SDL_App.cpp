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
NOM_LOG_TRACE ( NOM );

  if ( IMG_Init ( IMG_INIT_PNG ) != IMG_INIT_PNG )
  {
    NOM_LOG_ERR ( NOM, IMG_GetError() );
  }

  atexit ( IMG_Quit );
  this->app_state = true;

  this->set_show_fps ( true );

  this->appTime.start();
}

SDL_App::~SDL_App ( void )
{
NOM_LOG_TRACE ( NOM );

  this->appTime.stop();

  this->app_state = false;

  if ( SDL_WasInit ( SDL_INIT_JOYSTICK ) )
  {
    SDL_QuitSubSystem ( SDL_INIT_JOYSTICK );
  }
}

bool SDL_App::running ( void )
{
  if ( this->app_state == true ) return true;

  return false;
}

bool SDL_App::onInit ( void )
{
  return true;
}

void SDL_App::quit ( void )
{
  this->app_state = false;
}

void SDL_App::onQuit ( void )
{
  this->quit();
}

void SDL_App::onEvent ( EventType* event )
{
  Input::HandleInput ( event );
}

uint32 SDL_App::ticks ( void )
{
  return this->appTime.ticks();
}

bool SDL_App::show_fps ( void ) const
{
  return this->show_fps_;
}

void SDL_App::set_show_fps ( bool toggle )
{
  this->show_fps_ = toggle;
}

bool SDL_App::toggle_fps ( void )
{
  if ( this->show_fps() )
  {
    this->set_show_fps ( false );
  }
  else
  {
    this->set_show_fps ( true );
  }

  return this->show_fps_;
}

bool SDL_App::PollEvents ( EventType* event )
{
  if ( SDL_PollEvent ( event ) )
  {
    return true;
  }

  return false;
}

} // namespace nom
