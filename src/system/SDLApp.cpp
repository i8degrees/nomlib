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
#include "nomlib/system/SDLApp.hpp"

namespace nom {

SDLApp::SDLApp ( void ) :
  //state_factory { new GameStates() },
  // input_mapper { std::unique_ptr<InputMapping> ( nullptr ) },
  app_state_( true ),
  show_fps_( true )
{
  NOM_LOG_TRACE( NOM );

  this->app_timer_.start();

  // Mac OS X "bug" fix for keeping our window from getting minimized upon
  // losing focus to an application on another display.
  #if defined( NOM_PLATFORM_OSX )
    if( nom::set_hint( SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0" ) == false )
    {
      NOM_LOG_ERR( NOM, "Could not disable minimizing window on focus loss." );
    }
  #endif
}

SDLApp::~SDLApp ( void )
{
  NOM_LOG_TRACE( NOM );

  this->app_timer_.stop();

  this->quit();

  if ( SDL_WasInit ( SDL_INIT_JOYSTICK ) )
  {
    SDL_QuitSubSystem ( SDL_INIT_JOYSTICK );
  }
}

bool SDLApp::on_init( void )
{
  // User implemented

  return true;
}

void SDLApp::on_event( SDL_Event* ev )
{
  // Active (set) input mappings take priority over traditional events
  // handling -- virtual inheritance.
  // if( this->on_map( event ) == true ) return;

  // First, handle our events
  EventHandler::on_input( ev );

  // Next, handle the state's events
  this->states.process_events( ev );

  this->input_mapper.on_input( ev );
}

void SDLApp::on_update ( float delta )
{
  this->states.update( delta );
}

void SDLApp::on_draw ( IDrawable::RenderTarget& target )
{
  this->states.draw( target );
}

void SDLApp::on_window_close( const Event& ev )
{
  // A call is made here to the virtual method being re-implemented here in
  // order to catch debugging output with debug builds compiled in; see
  // EventHandler.hpp.
  EventHandler::on_window_close( ev );

  this->on_app_quit( ev );
}

void SDLApp::on_app_quit( const Event& ev )
{
  // A call is made here to the virtual method being re-implemented here in
  // order to catch debugging output with debug builds compiled in; see
  // EventHandler.hpp.
  EventHandler::on_app_quit( ev );

  this->quit();
}

bool SDLApp::running( void )
{
  if ( this->app_state_ == true ) return true;

  return false;
}

uint32 SDLApp::ticks ( void )
{
  return this->app_timer_.ticks();
}

void SDLApp::quit( void )
{
  this->app_state_ = false;
}

bool SDLApp::show_fps ( void ) const
{
  return this->show_fps_;
}

void SDLApp::set_show_fps ( bool toggle )
{
  this->show_fps_ = toggle;
}

bool SDLApp::toggle_fps ( void )
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

uint32 SDLApp::previous_state ( void ) const
{
  return this->states.previous_state();
}

void SDLApp::set_state ( uint32 id, void_ptr data )
{
/*
  IState::UniquePtr state = this->state_factory->state( id );

  this->states.set_state( std::move(state), data );
  if ( state != nullptr )
  {
    this->states.set_state( std::move(state), data );
  }
*/
}

void SDLApp::set_state ( IState::UniquePtr state, void_ptr data )
{
  this->states.set_state( std::move(state), data );
}

void SDLApp::push_state ( IState::UniquePtr state, void_ptr data )
{
  this->states.push_state( std::move(state), data );
}

void SDLApp::pop_state ( IState::UniquePtr state, void_ptr data )
{
  this->states.pop_state( std::move(state), data );
}

void SDLApp::pop_state ( void_ptr data )
{
  this->states.pop_state( data );
}

} // namespace nom
