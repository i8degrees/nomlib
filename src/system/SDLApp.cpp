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

// Private headers (third-party libs)
// #include "SDL.h"

// Private headers
#include "nomlib/system/SDL_helpers.hpp"
#include "nomlib/system/init.hpp"
#include "nomlib/system/ColorDatabase.hpp"
#include "nomlib/system/PlatformSettings.hpp" // System fonts initialization

// Forward declarations
#include "nomlib/system/Event.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/system/StateMachine.hpp"

namespace nom {

SDLApp::SDLApp( void )
  // state_factory { new GameStates() }
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_SYSTEM );

  uint32 flags =  OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS |
                  OSX_DISABLE_FULLSCREEN_SPACES |
                  INIT_ENGINE_FONTS;

  this->initialize( flags );
}

SDLApp::SDLApp( uint32 flags )
  // state_factory { new GameStates() }
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_SYSTEM );

  this->initialize( flags );
}

SDLApp::~SDLApp( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_SYSTEM );

  this->app_timer_.stop();

  this->quit();
}

bool SDLApp::app_state( void ) const
{
  return this->app_state_;
}

bool SDLApp::on_init( void )
{
  return true;
}

sint SDLApp::Run( void )
{
  return NOM_EXIT_SUCCESS;
}

void SDLApp::on_event( const Event& ev )
{
  // First, handle our own events
  EventHandler::process_event( ev );

  // Next, handle the state's manager's list of events
  //
  // StateMachine::on_event
  if( this->state_ != nullptr )
  {
    this->state()->on_event( ev );
  }
}

void SDLApp::on_update( float delta )
{
  if( this->state_ != nullptr )
  {
    this->state()->update( delta );
  }
}

void SDLApp::on_draw( RenderWindow& target )
{
  if( this->state() != nullptr )
  {
    this->state()->draw( target );
  }
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
  if ( this->app_state() == true ) return true;

  return false;
}

uint32 SDLApp::ticks ( void )
{
  return this->app_timer_.ticks();
}

void SDLApp::quit( void )
{
  this->set_app_state( false );
}

void SDLApp::set_app_state( bool state )
{
  this->app_state_ = state;
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

StateMachine* SDLApp::state( void ) const
{
  NOM_ASSERT( this->state_ != nullptr );

  return this->state_.get();
}

void SDLApp::set_state_machine( StateMachine* mech )
{
  this->state_.reset( mech );
}

// Private scope

bool SDLApp::initialize( uint32 flags )
{
  this->app_timer_.start();
  this->set_app_state( true );
  this->set_show_fps( true );

  if( flags & OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS )
  {
    // Mac OS X "bug" fix for keeping our window from getting minimized upon
    // losing focus to an application on another display.
    if( nom::set_hint( SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0" ) == false )
    {
      NOM_LOG_ERR( NOM, "Could not disable minimizing window on focus loss." );
      return false;
    }
  }

  if( flags & OSX_DISABLE_FULLSCREEN_SPACES )
  {
    if( nom::set_hint( SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0" ) == false )
    {
      NOM_LOG_ERR( NOM, "Could not disable Spaces support." );
      return false;
    }
  }

  if( flags & INIT_ENGINE_FONTS )
  {
    // Create a resource cache for letting either SDLApp or the end-user add
    // fonts for ease of access.
    SystemFonts::initialize();

    // Platform specific initialization of fonts (system, user, engine) that are
    // available to the engine for access.
    PlatformSettings::initialize();
  }

  // The color database is allocated and immediately populated
  SystemColors::initialize();

  return true;
}

} // namespace nom
