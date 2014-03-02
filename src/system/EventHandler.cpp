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
#include "nomlib/system/EventHandler.hpp"

namespace nom {

EventHandler::EventHandler( void ) :
  joystick_ { EventHandler::JoystickUniquePtr( nullptr, priv::Free_Joystick ) },
  joystick_id_ ( 0 )
{
  NOM_LOG_TRACE( NOM );

  if ( SDL_WasInit( SDL_INIT_JOYSTICK ) == false )
  {
    if ( SDL_InitSubSystem( SDL_INIT_JOYSTICK ) < 0 )
    {
      NOM_LOG_ERR( NOM, SDL_GetError() );
      return;
    }
  }

  NOM_LOG_INFO( NOM, std::to_string( SDL_NumJoysticks() ) + " joysticks were found" );

  if ( SDL_NumJoysticks() > 0 )
  {
    this->joystick_.reset( SDL_JoystickOpen( 0 ) );

    if ( this->joystick_.get() )
    {
      for( int idx = 0; idx < SDL_NumJoysticks(); idx++ )
      {
        NOM_LOG_INFO( NOM, SDL_JoystickNameForIndex( idx ) );
      }

      SDL_JoystickEventState( SDL_ENABLE );

      this->joystick_id_ = SDL_JoystickInstanceID( this->joystick_.get() );
    }
  }
}

EventHandler::~EventHandler( void )
{
  NOM_LOG_TRACE( NOM );
}

bool EventHandler::on_input( SDL_Event* event )
{
  Event ev;

  // Nothing to do; no pending events!
  // if( this->poll_event( &event ) == false ) return false;

  ev.type = event->type;

  // Create our event structure from the existing SDL_Event information.
  switch( event->type )
  {
    default:
    {
      // Ignore unknown events
      break;
    }

    case SDL_QUIT:
    {
      ev.quit.type = event->quit.type;
      ev.quit.timestamp = event->quit.timestamp;

      this->on_app_quit( ev );
      break;
    }

    case SDL_WINDOWEVENT:
    {
      switch( event->window.event )
      {
        default:
        {
          // Ignore unknown events
          break;
        }

        case SDL_WINDOWEVENT_NONE:
        {
          // Not implemented as per SDL2 wiki documentation
          break;
        }

        case SDL_WINDOWEVENT_SHOWN:
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_shown( ev );
          break;
        }

        case SDL_WINDOWEVENT_HIDDEN:
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_hidden( ev );
          break;
        }

        case SDL_WINDOWEVENT_EXPOSED:
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_exposed( ev );
          break;
        }

        case SDL_WINDOWEVENT_MOVED:
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_moved( ev );
          break;
        }

        case SDL_WINDOWEVENT_RESIZED:
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          // FIXME: TTcards ends up in an infinite loop with the virtual method
          // call below.
          // this->on_window_resized( ev );
          break;
        }

        case SDL_WINDOWEVENT_SIZE_CHANGED:
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_size_changed( ev );
          break;
        }

        case SDL_WINDOWEVENT_MINIMIZED:
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_minimized( ev );
          break;
        }

        case SDL_WINDOWEVENT_MAXIMIZED:
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_maximized( ev );
          break;
        }

        case SDL_WINDOWEVENT_RESTORED:
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_restored( ev );
          break;
        }

        case SDL_WINDOWEVENT_ENTER: // Mouse has entered window
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_mouse_focus( ev );
          break;
        }

        case SDL_WINDOWEVENT_LEAVE: // Mouse has left window
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_mouse_focus_lost( ev );
          break;
        }

        case SDL_WINDOWEVENT_FOCUS_GAINED: // Keyboard has focus of window
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_keyboard_focus( ev );
          break;
        }

        case SDL_WINDOWEVENT_FOCUS_LOST: // Keyboard has lost window focus
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_keyboard_focus_lost( ev );
          break;
        }

        case SDL_WINDOWEVENT_CLOSE:
        {
          ev.window.type = event->window.type;
          ev.window.event = event->window.event;
          ev.window.data1 = event->window.data1;
          ev.window.data2 = event->window.data2;
          ev.window.window_id = event->window.windowID;
          ev.window.timestamp = event->window.timestamp;

          this->on_window_close( ev );
          break;
        }
      } // end event->window.event switch
    } // end SDL_WINDOWEVENT

    case SDL_USEREVENT:
    {
      UserEvent user_event;

      user_event.type = event->type;
      user_event.type = event->user.type;
      user_event.code = event->user.code;
      user_event.data1 = event->user.data1;
      user_event.data2 = event->user.data2;
      user_event.window_id = event->user.windowID;
      user_event.timestamp = event->user.timestamp;

      this->on_user_event( user_event );
      break;
    }

    case SDL_SYSWMEVENT:
    {
      // Ignored event
      break;
    }

    case SDL_KEYDOWN:
    {
      ev.key.type = event->key.type;
      ev.key.sym = event->key.keysym.sym;
      ev.key.mod = event->key.keysym.mod;
      ev.key.state = event->key.state;
      ev.key.repeat = event->key.repeat;
      ev.key.window_id = event->key.windowID;
      ev.key.timestamp = event->key.timestamp;

      this->on_key_down( ev );
      break;
    }

    case SDL_KEYUP:
    {
      ev.key.type = event->key.type;
      ev.key.sym = event->key.keysym.sym;
      ev.key.mod = event->key.keysym.mod;
      ev.key.state = event->key.state;
      ev.key.repeat = event->key.repeat;
      ev.key.window_id = event->key.windowID;
      ev.key.timestamp = event->key.timestamp;

      this->on_key_up( ev );
      break;
    }

    case SDL_MOUSEMOTION:
    {
      ev.m_motion.type = event->motion.type;
      ev.m_motion.x = event->motion.x;
      ev.m_motion.y = event->motion.y;
      ev.m_motion.x_rel = event->motion.xrel;
      ev.m_motion.y_rel = event->motion.yrel;
      ev.m_motion.state = event->motion.state;
      ev.m_motion.window_id = event->button.windowID;
      ev.m_motion.timestamp = event->button.timestamp;

      this->on_mouse_motion( ev );
      break;
    }

    case SDL_MOUSEBUTTONDOWN:
    {
      switch ( event->button.button )
      {
        case SDL_BUTTON_LEFT:
        {
          ev.mouse.type = event->button.type;
          ev.mouse.id = event->button.which;
          ev.mouse.x = event->button.x;
          ev.mouse.y = event->button.y;
          ev.mouse.button = event->button.button;
          ev.mouse.state = event->button.state;
          // ev.mouse.clicks = event->button.clicks;
          ev.mouse.window_id = event->button.windowID;
          ev.mouse.timestamp = event->button.timestamp;

          this->on_mouse_left_button_down( ev );
          break;
        }

        case SDL_BUTTON_MIDDLE:
        {
          ev.mouse.type = event->button.type;
          ev.mouse.id = event->button.which;
          ev.mouse.x = event->button.x;
          ev.mouse.y = event->button.y;
          ev.mouse.button = event->button.button;
          ev.mouse.state = event->button.state;
          // ev.mouse.clicks = event->button.clicks;
          ev.mouse.window_id = event->button.windowID;
          ev.mouse.timestamp = event->button.timestamp;

          this->on_mouse_middle_button_down( ev );
          break;
        }

        case SDL_BUTTON_RIGHT:
        {
          ev.mouse.type = event->button.type;
          ev.mouse.id = event->button.which;
          ev.mouse.x = event->button.x;
          ev.mouse.y = event->button.y;
          ev.mouse.button = event->button.button;
          ev.mouse.state = event->button.state;
          // ev.mouse.clicks = event->button.clicks;
          ev.mouse.window_id = event->button.windowID;
          ev.mouse.timestamp = event->button.timestamp;

          this->on_mouse_right_button_down( ev );
          break;
        }

        case SDL_BUTTON_X1:
        {
          ev.mouse.type = event->button.type;
          ev.mouse.id = event->button.which;
          ev.mouse.x = event->button.x;
          ev.mouse.y = event->button.y;
          ev.mouse.button = event->button.button;
          ev.mouse.state = event->button.state;
          // ev.mouse.clicks = event->button.clicks;
          ev.mouse.window_id = event->button.windowID;
          ev.mouse.timestamp = event->button.timestamp;

          this->on_mouse_button_four_down( ev );
          break;
        }

        case SDL_BUTTON_X2:
        {
          ev.mouse.type = event->button.type;
          ev.mouse.id = event->button.which;
          ev.mouse.x = event->button.x;
          ev.mouse.y = event->button.y;
          ev.mouse.button = event->button.button;
          ev.mouse.state = event->button.state;
          // ev.mouse.clicks = event->button.clicks;
          ev.mouse.window_id = event->button.windowID;
          ev.mouse.timestamp = event->button.timestamp;

          this->on_mouse_button_five_down( ev );
          break;
        }
      }
      break;
    } // end SDL_MOUSEBUTTONDOWN event

    case SDL_MOUSEBUTTONUP:
      switch ( event->button.button )
      {
        case SDL_BUTTON_LEFT:
        {
          ev.mouse.type = event->button.type;
          ev.mouse.id = event->button.which;
          ev.mouse.x = event->button.x;
          ev.mouse.y = event->button.y;
          ev.mouse.button = event->button.button;
          ev.mouse.state = event->button.state;
          // ev.mouse.clicks = event->button.clicks;
          ev.mouse.window_id = event->button.windowID;
          ev.mouse.timestamp = event->button.timestamp;

          this->on_mouse_left_button_up( ev );
          break;
        }

        case SDL_BUTTON_MIDDLE:
        {
          ev.mouse.type = event->button.type;
          ev.mouse.id = event->button.which;
          ev.mouse.x = event->button.x;
          ev.mouse.y = event->button.y;
          ev.mouse.button = event->button.button;
          ev.mouse.state = event->button.state;
          // ev.mouse.clicks = event->button.clicks;
          ev.mouse.window_id = event->button.windowID;
          ev.mouse.timestamp = event->button.timestamp;

          this->on_mouse_middle_button_up( ev );
          break;
        }

        case SDL_BUTTON_RIGHT:
        {
          ev.mouse.type = event->button.type;
          ev.mouse.id = event->button.which;
          ev.mouse.x = event->button.x;
          ev.mouse.y = event->button.y;
          ev.mouse.button = event->button.button;
          ev.mouse.state = event->button.state;
          // ev.mouse.clicks = event->button.clicks;
          ev.mouse.window_id = event->button.windowID;
          ev.mouse.timestamp = event->button.timestamp;

          this->on_mouse_right_button_up( ev );
          break;
        }

        case SDL_BUTTON_X1:
        {
          ev.mouse.type = event->button.type;
          ev.mouse.id = event->button.which;
          ev.mouse.x = event->button.x;
          ev.mouse.y = event->button.y;
          ev.mouse.button = event->button.button;
          ev.mouse.state = event->button.state;
          // ev.mouse.clicks = event->button.clicks;
          ev.mouse.window_id = event->button.windowID;
          ev.mouse.timestamp = event->button.timestamp;

          this->on_mouse_button_four_up( ev );
          break;
        }

        case SDL_BUTTON_X2:
        {
          ev.mouse.type = event->button.type;
          ev.mouse.id = event->button.which;
          ev.mouse.x = event->button.x;
          ev.mouse.y = event->button.y;
          ev.mouse.button = event->button.button;
          ev.mouse.state = event->button.state;
          // ev.mouse.clicks = event->button.clicks;
          ev.mouse.window_id = event->button.windowID;
          ev.mouse.timestamp = event->button.timestamp;

          this->on_mouse_button_five_up( ev );
          break;
        }
      }
    break;

    case SDL_MOUSEWHEEL:
    {
      ev.wheel.type = event->wheel.type;
      ev.wheel.id = event->wheel.which;
      ev.wheel.x = event->wheel.x;
      ev.wheel.y = event->wheel.y;
      ev.wheel.window_id = event->wheel.windowID;
      ev.wheel.timestamp = event->wheel.timestamp;

      this->on_mouse_wheel( ev );
      break;
    }

    case SDL_JOYBUTTONDOWN:
    {
      ev.jbutton.type = event->jbutton.type;
      ev.jbutton.id = this->joystick_id_;
      ev.jbutton.button = event->jbutton.button;
      ev.jbutton.state = event->jbutton.state;
      // ev.jbutton.window_id = event->jbutton.windowID;
      ev.jbutton.timestamp = event->jbutton.timestamp;

      this->on_joy_button_down( ev );
      break;
    }

    case SDL_JOYBUTTONUP:
    {
      ev.jbutton.type = event->jbutton.type;
      ev.jbutton.id = this->joystick_id_;
      ev.jbutton.button = event->jbutton.button;
      ev.jbutton.state = event->jbutton.state;
      // ev.jbutton.window_id = event->jbutton.windowID;
      ev.jbutton.timestamp = event->jbutton.timestamp;

      this->on_joy_button_up( ev );
      break;
    }

    case SDL_JOYAXISMOTION:
    {
      ev.jaxis.type = event->jaxis.type;
      ev.jaxis.id = this->joystick_id_;
      ev.jaxis.axis = event->jaxis.axis;
      ev.jaxis.value = event->jaxis.value;
      // ev.jaxis.window_id = event->jaxis.windowID;
      ev.jaxis.timestamp = event->jaxis.timestamp;

      this->on_joy_axis( ev );
      break;
    }

    case SDL_JOYDEVICEADDED:
    {
      this->on_joystick_add();
      break;
    }

    case SDL_JOYDEVICEREMOVED:
    {
      this->on_joystick_remove();
      break;
    }

    case SDL_FINGERMOTION:
    {
      ev.touch.type = event->tfinger.type;
      ev.touch.id = event->tfinger.touchId;
      ev.touch.finger.id = event->tfinger.fingerId;
      ev.touch.x = event->tfinger.x;
      ev.touch.y = event->tfinger.y;
      ev.touch.dx = event->tfinger.dx;
      ev.touch.dy = event->tfinger.dy;
      ev.touch.pressure = event->tfinger.pressure;

      this->on_touch_motion( ev );
      break;
    }

    case SDL_FINGERDOWN:
    {
      ev.touch.type = event->tfinger.type;
      ev.touch.id = event->tfinger.touchId;
      ev.touch.finger.id = event->tfinger.fingerId;
      ev.touch.x = event->tfinger.x;
      ev.touch.y = event->tfinger.y;
      ev.touch.dx = event->tfinger.dx;
      ev.touch.dy = event->tfinger.dy;
      ev.touch.pressure = event->tfinger.pressure;

      this->on_touch_down( ev );
      break;
    }

    case SDL_FINGERUP:
    {
      ev.touch.type = event->tfinger.type;
      ev.touch.id = event->tfinger.touchId;
      ev.touch.finger.id = event->tfinger.fingerId;
      ev.touch.x = event->tfinger.x;
      ev.touch.y = event->tfinger.y;
      ev.touch.dx = event->tfinger.dx;
      ev.touch.dy = event->tfinger.dy;
      ev.touch.pressure = event->tfinger.pressure;

      this->on_touch_up( ev );
      break;
    }

    case SDL_MULTIGESTURE:
    {
      ev.gesture.type = event->mgesture.type;
      ev.gesture.id = event->mgesture.touchId;
      ev.gesture.dTheta = event->mgesture.dTheta;
      ev.gesture.dDist = event->mgesture.dDist;
      ev.gesture.x = event->mgesture.x;
      ev.gesture.y = event->mgesture.y;
      ev.gesture.num_fingers = event->mgesture.numFingers;

      this->on_gesture( ev );
      break;
    }

    case SDL_DROPFILE:
    {
      ev.drop.file_path = event->drop.file;
      ev.drop.timestamp = event->drop.timestamp;

      this->on_drag_drop( ev );
      break;
    }

    case SDL_TEXTINPUT:
    {
      ev.text.type = event->text.type;
      strcpy( ev.text.text, event->text.text );
      ev.text.timestamp = event->text.timestamp;
      ev.text.window_id = event->text.windowID;

      this->on_text_input( ev );
      break;
    }

    case SDL_TEXTEDITING: // FIXME
    {
      ev.edit.type = event->edit.type;
      strcpy( ev.edit.text, event->edit.text );
      ev.edit.timestamp = event->edit.timestamp;
      ev.edit.window_id = event->edit.windowID;

      this->on_text_edit( ev );
      break;
    }
  } // end switch event->type

  // Pending events
  return true;
}

void EventHandler::on_user_event( const UserEvent& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_USER_EVENT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.code );
    NOM_DUMP( ev.window_id );
    NOM_DUMP( ev.timestamp );
  #endif
}

void EventHandler::on_app_quit( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_QUIT_EVENT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.quit.type );
    NOM_DUMP( ev.quit.timestamp );
  #endif
}

void EventHandler::on_window_shown( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_hidden( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_exposed( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_moved( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_resized( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_size_changed( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_minimized( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_maximized( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_restored( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_mouse_focus( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_FOCUS_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_mouse_focus_lost( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_FOCUS_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_keyboard_focus( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_FOCUS_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_keyboard_focus_lost( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_FOCUS_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_window_close( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.window.type );
    NOM_DUMP( NOM_SCAST( int, ev.window.event ) );
    NOM_DUMP( ev.window.data1 );
    NOM_DUMP( ev.window.data2 );
    NOM_DUMP( ev.window.window_id );
    NOM_DUMP( ev.window.timestamp );
  #endif
}

void EventHandler::on_key_down( const Event& ev )
{
  #if defined( NOM_DEBUG_SDL2_KEYBOARD_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.key.type );
    NOM_DUMP( ev.key.sym );
    NOM_DUMP( ev.key.mod );
    NOM_DUMP( NOM_SCAST( int, ev.key.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.key.repeat ) );
    NOM_DUMP( ev.key.window_id );
    NOM_DUMP( ev.key.timestamp );
  #endif

  // Mac OS X: Handle the standard Command + W action -- close window.
  #if defined( NOM_PLATFORM_OSX )
    if( ev.key.sym == SDLK_w && ev.key.mod == KMOD_LGUI )
    {
      this->on_window_close( ev );
    }
  #endif
}

void EventHandler::on_key_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_KEYBOARD_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.key.type );
    NOM_DUMP( ev.key.sym );
    NOM_DUMP( ev.key.mod );
    NOM_DUMP( NOM_SCAST( int, ev.key.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.key.repeat ) );
    NOM_DUMP( ev.key.window_id );
    NOM_DUMP( ev.key.timestamp );
  #endif
}

void EventHandler::on_mouse_motion( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_MOTION_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.m_motion.type );
    NOM_DUMP( ev.m_motion.id );
    NOM_DUMP( ev.m_motion.x );
    NOM_DUMP( ev.m_motion.y );
    NOM_DUMP( ev.m_motion.x_rel );
    NOM_DUMP( ev.m_motion.y_rel );
    // FIXME: Handle the print out of the possible bit-mask values; see also
    // remarks in EventHandler.hpp.
    NOM_DUMP( NOM_SCAST( int, ev.m_motion.state ) );
    NOM_DUMP( ev.m_motion.window_id );
    NOM_DUMP( ev.m_motion.timestamp );
  #endif
}

void EventHandler::on_mouse_wheel( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.wheel.type );
    NOM_DUMP( ev.wheel.id );
    NOM_DUMP( ev.wheel.x );
    NOM_DUMP( ev.wheel.y );
    NOM_DUMP( ev.wheel.window_id );
    NOM_DUMP( ev.wheel.timestamp );
  #endif
}

void EventHandler::on_mouse_left_button_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.mouse.type );
    NOM_DUMP( ev.mouse.id );
    NOM_DUMP( ev.mouse.x );
    NOM_DUMP( ev.mouse.y );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.clicks ) );
    NOM_DUMP( ev.mouse.window_id );
    NOM_DUMP( ev.mouse.timestamp );
  #endif
}

void EventHandler::on_mouse_middle_button_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.mouse.type );
    NOM_DUMP( ev.mouse.id );
    NOM_DUMP( ev.mouse.x );
    NOM_DUMP( ev.mouse.y );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.clicks ) );
    NOM_DUMP( ev.mouse.window_id );
    NOM_DUMP( ev.mouse.timestamp );
  #endif
}

void EventHandler::on_mouse_right_button_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.mouse.type );
    NOM_DUMP( ev.mouse.id );
    NOM_DUMP( ev.mouse.x );
    NOM_DUMP( ev.mouse.y );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.clicks ) );
    NOM_DUMP( ev.mouse.window_id );
    NOM_DUMP( ev.mouse.timestamp );
  #endif
}

void EventHandler::on_mouse_button_four_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.mouse.type );
    NOM_DUMP( ev.mouse.id );
    NOM_DUMP( ev.mouse.x );
    NOM_DUMP( ev.mouse.y );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.clicks ) );
    NOM_DUMP( ev.mouse.window_id );
    NOM_DUMP( ev.mouse.timestamp );
  #endif
}

void EventHandler::on_mouse_button_five_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.mouse.type );
    NOM_DUMP( ev.mouse.id );
    NOM_DUMP( ev.mouse.x );
    NOM_DUMP( ev.mouse.y );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.clicks ) );
    NOM_DUMP( ev.mouse.window_id );
    NOM_DUMP( ev.mouse.timestamp );
  #endif
}

void EventHandler::on_mouse_left_button_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.mouse.type );
    NOM_DUMP( ev.mouse.id );
    NOM_DUMP( ev.mouse.x );
    NOM_DUMP( ev.mouse.y );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.clicks ) );
    NOM_DUMP( ev.mouse.window_id );
    NOM_DUMP( ev.mouse.timestamp );
  #endif
}

void EventHandler::on_mouse_middle_button_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.mouse.type );
    NOM_DUMP( ev.mouse.id );
    NOM_DUMP( ev.mouse.x );
    NOM_DUMP( ev.mouse.y );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.clicks ) );
    NOM_DUMP( ev.mouse.window_id );
    NOM_DUMP( ev.mouse.timestamp );
  #endif
}

void EventHandler::on_mouse_right_button_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.mouse.type );
    NOM_DUMP( ev.mouse.id );
    NOM_DUMP( ev.mouse.x );
    NOM_DUMP( ev.mouse.y );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.clicks ) );
    NOM_DUMP( ev.mouse.window_id );
    NOM_DUMP( ev.mouse.timestamp );
  #endif
}

void EventHandler::on_mouse_button_four_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.mouse.type );
    NOM_DUMP( ev.mouse.id );
    NOM_DUMP( ev.mouse.x );
    NOM_DUMP( ev.mouse.y );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.clicks ) );
    NOM_DUMP( ev.mouse.window_id );
    NOM_DUMP( ev.mouse.timestamp );
  #endif
}

void EventHandler::on_mouse_button_five_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.mouse.type );
    NOM_DUMP( ev.mouse.id );
    NOM_DUMP( ev.mouse.x );
    NOM_DUMP( ev.mouse.y );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.state ) );
    NOM_DUMP( NOM_SCAST( int, ev.mouse.clicks ) );
    NOM_DUMP( ev.mouse.window_id );
    NOM_DUMP( ev.mouse.timestamp );
  #endif
}

void EventHandler::on_joy_axis( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_JOYSTICK_AXIS_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.jaxis.type );
    NOM_DUMP( ev.jaxis.id );
    NOM_DUMP( NOM_SCAST( int, ev.jaxis.axis ) );
    NOM_DUMP( ev.jaxis.value );
    NOM_DUMP( ev.jaxis.timestamp );
  #endif
}

void EventHandler::on_joy_button_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_JOYSTICK_BUTTON_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.jbutton.type );
    NOM_DUMP( ev.jbutton.id );
    NOM_DUMP( NOM_SCAST( int, ev.jbutton.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.jbutton.state ) );
    NOM_DUMP( ev.jbutton.timestamp );
  #endif
}

void EventHandler::on_joy_button_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_JOYSTICK_BUTTON_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.jbutton.type );
    NOM_DUMP( ev.jbutton.id );
    NOM_DUMP( NOM_SCAST( int, ev.jbutton.button ) );
    NOM_DUMP( NOM_SCAST( int, ev.jbutton.state ) );
    NOM_DUMP( ev.jbutton.timestamp );
  #endif
}

void EventHandler::on_joystick_add( void )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_JOYSTICK_EVENT )
    NOM_LOG_TRACE( NOM );
  #endif
}
void EventHandler::on_joystick_remove( void )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_JOYSTICK_EVENT )
    NOM_LOG_TRACE( NOM );
  #endif
}

void EventHandler::on_touch_motion( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_TOUCH_MOTION_EVENT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.touch.type );
    NOM_DUMP( ev.touch.id );
    NOM_DUMP( ev.touch.finger.id );
    NOM_DUMP( ev.touch.x );
    NOM_DUMP( ev.touch.y );
    NOM_DUMP( ev.touch.dx );
    NOM_DUMP( ev.touch.dy );
    NOM_DUMP( ev.touch.pressure );
    NOM_DUMP( ev.touch.timestamp );
  #endif
}

void EventHandler::on_touch_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_TOUCH_EVENT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.touch.type );
    NOM_DUMP( ev.touch.id );
    NOM_DUMP( ev.touch.finger.id );
    NOM_DUMP( ev.touch.x );
    NOM_DUMP( ev.touch.y );
    NOM_DUMP( ev.touch.dx );
    NOM_DUMP( ev.touch.dy );
    NOM_DUMP( ev.touch.pressure );
    NOM_DUMP( ev.touch.timestamp );
  #endif
}

void EventHandler::on_touch_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_TOUCH_EVENT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.touch.type );
    NOM_DUMP( ev.touch.id );
    NOM_DUMP( ev.touch.finger.id );
    NOM_DUMP( ev.touch.x );
    NOM_DUMP( ev.touch.y );
    NOM_DUMP( ev.touch.dx );
    NOM_DUMP( ev.touch.dy );
    NOM_DUMP( ev.touch.pressure );
    NOM_DUMP( ev.touch.timestamp );
  #endif
}

void EventHandler::on_gesture( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_GESTURE_EVENT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.gesture.type );
    NOM_DUMP( ev.gesture.id );
    NOM_DUMP( ev.gesture.dTheta );
    NOM_DUMP( ev.gesture.dDist );
    NOM_DUMP( ev.gesture.x );
    NOM_DUMP( ev.gesture.y );
    NOM_DUMP( ev.gesture.num_fingers );
  #endif
}

void EventHandler::on_drag_drop( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_DRAG_DROP_INPUT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.drop.file_path );
    NOM_DUMP( ev.drop.timestamp );
  #endif
}

void EventHandler::on_text_input( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_TEXT_INPUT_EVENT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.text.type );
    NOM_DUMP( ev.text.text );
    NOM_DUMP( ev.text.window_id );
    NOM_DUMP( ev.text.timestamp );
  #endif
}

void EventHandler::on_text_edit( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_TEXT_EDIT_EVENT )
    NOM_LOG_TRACE( NOM );
    NOM_DUMP( ev.type );
    NOM_DUMP( ev.edit.type );
    NOM_DUMP( ev.edit.text );
    NOM_DUMP( ev.edit.start );
    NOM_DUMP( ev.edit.length );
    NOM_DUMP( ev.edit.window_id );
    NOM_DUMP( ev.edit.timestamp );
  #endif
}

bool EventHandler::poll_event( SDL_Event* ev )
{
  if ( SDL_PollEvent( ev ) == 1 )
  {
    return true;
  }

  return false;
}

} // namespace nom
