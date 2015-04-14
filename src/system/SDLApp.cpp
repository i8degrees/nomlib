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

// Private headers
#include "nomlib/core/err.hpp"
#include "nomlib/system/SDL_helpers.hpp"
#include "nomlib/system/init.hpp"
#include "nomlib/system/ColorDatabase.hpp"

// Forward declarations
#include "nomlib/system/Event.hpp"
#include "nomlib/system/EventHandler.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/system/StateMachine.hpp"

namespace nom {

SDLApp::SDLApp( void )
  // state_factory { new GameStates() }
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_SYSTEM );

  uint32 flags =  OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS |
                  OSX_DISABLE_FULLSCREEN_SPACES;

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
  // NOM_ASSERT( this->state_ != nullptr );

  return this->state_.get();
}

void SDLApp::set_state_machine( StateMachine* mech )
{
  NOM_ASSERT( mech != nullptr );

  this->state_.reset( mech );
}

void SDLApp::set_event_handler(EventHandler& evt_handler)
{
  this->event_handler_ = &evt_handler;

  auto event_watch = nom::event_filter( [=](const Event& evt, void* data) {
    this->on_app_event(evt);
  });

  this->event_handler_->append_event_watch(event_watch, nullptr);
}

// Protected scope

void SDLApp::on_app_quit(const Event& ev)
{
  // Default implementation

  this->quit();
}

void SDLApp::on_window_shown(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_hidden(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_exposed(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_moved(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_resized(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_size_changed(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_minimized(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_maximized(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_restored(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_mouse_focus(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_mouse_focus_lost(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_keyboard_focus(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_keyboard_focus_lost(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_window_close(const Event& ev)
{
  // Default implementation

  this->on_app_quit(ev);
}

void SDLApp::on_input_event(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_drag_drop(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_user_event(const Event& ev)
{
  // Default implementation
}

void SDLApp::on_render_targets_reset(const Event& ev)
{
  // Default implementation
}

// NOTE: Not available until the release of SDL 2.0.4
#if 0
void SDLApp::on_render_device_reset(const Event& ev)
{
  // Default implementation
}
#endif

// Private scope

void SDLApp::on_app_event(const Event& ev)
{
  // Handle our events
  this->process_event(ev);

  // Handle the state's events
  if( this->state_ != nullptr ) {
    this->state()->on_event(ev);
  }
}

void SDLApp::process_event(const Event& ev)
{
  switch(ev.type)
  {
    case Event::QUIT_EVENT:
    {
      this->on_app_quit(ev);
    } break;

    case Event::WINDOW_EVENT:
    {
      switch(ev.window.event)
      {
        default: break;

        case WindowEvent::NONE: break;

        case WindowEvent::SHOWN:
        {
          this->on_window_shown(ev);
        } break;

        case WindowEvent::HIDDEN:
        {
          this->on_window_hidden(ev);
        } break;

        case WindowEvent::EXPOSED:
        {
          this->on_window_exposed(ev);
        } break;

        case WindowEvent::MOVED:
        {
          this->on_window_moved(ev);
        } break;

        case WindowEvent::RESIZED:
        {
          this->on_window_resized(ev);
        } break;

        case WindowEvent::SIZE_CHANGED:
        {
          this->on_window_size_changed(ev);
        } break;

        case WindowEvent::MINIMIZED:
        {
          this->on_window_minimized(ev);
        } break;

        case WindowEvent::MAXIMIZED:
        {
          this->on_window_maximized(ev);
        } break;

        case WindowEvent::RESTORED:
        {
          this->on_window_restored(ev);
        } break;

        case WindowEvent::MOUSE_FOCUS_GAINED:
        {
          this->on_window_mouse_focus(ev);
        } break;

        case WindowEvent::MOUSE_FOCUS_LOST:
        {
          this->on_window_mouse_focus_lost(ev);
        } break;

        case WindowEvent::KEYBOARD_FOCUS_GAINED:
        {
          this->on_window_keyboard_focus(ev);
        } break;

        case WindowEvent::KEYBOARD_FOCUS_LOST:
        {
          this->on_window_keyboard_focus_lost(ev);
        } break;

        case WindowEvent::CLOSE:
        {
          this->on_window_close(ev);
        } break;
      } // end switch ev.window.event

    } break; // end case WINDOW_EVENT

    case Event::SYSWMEVENT: break;

    case Event::DROP_FILE:
    {
      this->on_drag_drop(ev);
    } break;

    case Event::RENDER_TARGETS_RESET:
    {
      this->on_render_targets_reset(ev);
    } break;

// NOTE: Not available until the release of SDL 2.0.4
#if 0
    case Event::RENDER_DEVICE_RESET:
    {
      this->on_render_device_reset(ev);
    } break;
#endif

    case Event::USER_EVENT:
    {
      this->on_user_event(ev);
    } break;

    default:
    {
      this->on_input_event(ev);
    } break;
  } // end switch ev.type
}

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

  // The color database is allocated and immediately populated
  SystemColors::initialize();

  return true;
}

} // namespace nom
