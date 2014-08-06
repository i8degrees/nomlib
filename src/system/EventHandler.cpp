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

EventHandler::EventHandler( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_EVENT, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

  // TODO: Err handling?
  this->joystick.initialize();
}

EventHandler::~EventHandler( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_EVENT, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );
}

void EventHandler::process_event( const Event& ev )
{
  // Process events gathered from the high-level events queue subsystem; wraps
  // the SDL events system.
  switch( ev.type )
  {
    default: break;

    case SDL_QUIT:
    {
      this->on_app_quit( ev );
      break;
    }

    case SDL_WINDOWEVENT:
    {
      switch( ev.window.event )
      {
        default: break;

        case SDL_WINDOWEVENT_NONE: break;

        case SDL_WINDOWEVENT_SHOWN:
        {
          this->on_window_shown( ev );
          break;
        }

        case SDL_WINDOWEVENT_HIDDEN:
        {
          this->on_window_hidden( ev );
          break;
        }

        case SDL_WINDOWEVENT_EXPOSED:
        {
          this->on_window_exposed( ev );
          break;
        }

        case SDL_WINDOWEVENT_MOVED:
        {
          this->on_window_moved( ev );
          break;
        }

        case SDL_WINDOWEVENT_RESIZED:
        {
          // FIXME: TTcards ends up in an infinite loop with the virtual method
          // call below.
          // this->on_window_resized( ev );
          break;
        }

        case SDL_WINDOWEVENT_SIZE_CHANGED:
        {
          this->on_window_size_changed( ev );
          break;
        }

        case SDL_WINDOWEVENT_MINIMIZED:
        {
          this->on_window_minimized( ev );
          break;
        }

        case SDL_WINDOWEVENT_MAXIMIZED:
        {
          this->on_window_maximized( ev );
          break;
        }

        case SDL_WINDOWEVENT_RESTORED:
        {
          this->on_window_restored( ev );
          break;
        }

        case SDL_WINDOWEVENT_ENTER:
        {
          this->on_window_mouse_focus( ev );
          break;
        }

        case SDL_WINDOWEVENT_LEAVE:
        {
          this->on_window_mouse_focus_lost( ev );
          break;
        }

        case SDL_WINDOWEVENT_FOCUS_GAINED:
        {
          this->on_window_keyboard_focus( ev );
          break;
        }

        case SDL_WINDOWEVENT_FOCUS_LOST:
        {
          this->on_window_keyboard_focus_lost( ev );
          break;
        }

        case SDL_WINDOWEVENT_CLOSE:
        {
          this->on_window_close( ev );
          break;
        }
      } // end switch ev.window.event
    } // end case SDL_WINDOWEVENT

    case SDL_USEREVENT:
    {
      this->on_user_event( ev );
      break;
    }

    case SDL_SYSWMEVENT: break;

    case SDL_KEYDOWN:
    {
      this->on_key_down( ev );
      break;
    }

    case SDL_KEYUP:
    {
      this->on_key_up( ev );
      break;
    }

    case SDL_MOUSEMOTION:
    {
      this->on_mouse_motion( ev );
      break;
    }

    case SDL_MOUSEBUTTONDOWN:
    {
      switch ( ev.mouse.button )
      {
        default: break;

        case SDL_BUTTON_LEFT:
        {
          this->on_mouse_left_button_down( ev );
          break;
        }

        case SDL_BUTTON_MIDDLE:
        {
          this->on_mouse_middle_button_down( ev );
          break;
        }

        case SDL_BUTTON_RIGHT:
        {
          this->on_mouse_right_button_down( ev );
          break;
        }

        case SDL_BUTTON_X1:
        {
          this->on_mouse_button_four_down( ev );
          break;
        }

        case SDL_BUTTON_X2:
        {
          this->on_mouse_button_five_down( ev );
          break;
        }
      } // end switch ev.button.button
    } // end switch SDL_MOUSEBUTTONDOWN

    case SDL_MOUSEBUTTONUP:
    {
      switch ( ev.mouse.button )
      {
        default: break;

        case SDL_BUTTON_LEFT:
        {
          this->on_mouse_left_button_up( ev );
          break;
        }

        case SDL_BUTTON_MIDDLE:
        {
          this->on_mouse_middle_button_up( ev );
          break;
        }

        case SDL_BUTTON_RIGHT:
        {
          this->on_mouse_right_button_up( ev );
          break;
        }

        case SDL_BUTTON_X1:
        {
          this->on_mouse_button_four_up( ev );
          break;
        }

        case SDL_BUTTON_X2:
        {
          this->on_mouse_button_five_up( ev );
          break;
        }
      } // end switch ev.button.button
    } // end switch SDL_MOUSEBUTTONUP

    case SDL_MOUSEWHEEL:
    {
      this->on_mouse_wheel( ev );
      break;
    }

    case SDL_JOYBUTTONDOWN:
    {
      this->on_joy_button_down( ev );
      break;
    }

    case SDL_JOYBUTTONUP:
    {
      this->on_joy_button_up( ev );
      break;
    }

    case SDL_JOYAXISMOTION:
    {
      this->on_joy_axis( ev );
      break;
    }

    case SDL_JOYDEVICEADDED:
    {
      this->on_joystick_connected( ev );
      break;
    }

    case SDL_JOYDEVICEREMOVED:
    {
      this->on_joystick_disconnected( ev );
      break;
    }

    case SDL_FINGERMOTION:
    {
      this->on_touch_motion( ev );
      break;
    }

    case SDL_FINGERDOWN:
    {
      this->on_touch_down( ev );
      break;
    }

    case SDL_FINGERUP:
    {
      this->on_touch_up( ev );
      break;
    }

    case SDL_MULTIGESTURE:
    {
      this->on_gesture( ev );
      break;
    }

    case SDL_DROPFILE:
    {
      this->on_drag_drop( ev );
      break;
    }

    case SDL_TEXTINPUT:
    {
      this->on_text_input( ev );
      break;
    }

    case SDL_TEXTEDITING:
    {
      this->on_text_edit( ev );
      break;
    }
  } // end switch ev.type
}

bool EventHandler::poll_event( Event& ev )
{
  // We have pending events in the queue!
  if( this->pop_event( ev ) )
  {
    return true;
  }

  // No pending events in queue
  return false;
}

// bool EventHandler::poll_event( SDL_Event* ev )
// {
//   if ( SDL_PollEvent( ev ) == 1 )
//   {
//     return true;
//   }

//   return false;
// }

void EventHandler::on_app_quit( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_QUIT_EVENT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
  #endif
}

void EventHandler::on_window_shown( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_hidden( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_exposed( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_moved( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_resized( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_size_changed( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_minimized( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_maximized( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_restored( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_mouse_focus( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_FOCUS_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_mouse_focus_lost( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_FOCUS_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_keyboard_focus( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_FOCUS_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_keyboard_focus_lost( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_FOCUS_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_window_close( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_WINDOW_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.window.dump();
  #endif
}

void EventHandler::on_key_down( const Event& ev )
{
  #if defined( NOM_DEBUG_SDL2_KEYBOARD_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.key.dump();
  #endif
}

void EventHandler::on_key_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_KEYBOARD_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.key.dump();
  #endif
}

void EventHandler::on_mouse_motion( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_MOTION_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.motion.dump();
  #endif
}

void EventHandler::on_mouse_wheel( const Event& ev )
{
  // User implemented

  if( ev.type != SDL_MOUSEWHEEL ) return;

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.wheel.dump();
  #endif
}

void EventHandler::on_mouse_left_button_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.mouse.dump();
  #endif
}

void EventHandler::on_mouse_middle_button_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.mouse.dump();
  #endif
}

void EventHandler::on_mouse_right_button_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.mouse.dump();
  #endif
}

void EventHandler::on_mouse_button_four_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.mouse.dump();
  #endif
}

void EventHandler::on_mouse_button_five_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.mouse.dump();
  #endif
}

void EventHandler::on_mouse_left_button_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.mouse.dump();
  #endif
}

void EventHandler::on_mouse_middle_button_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.mouse.dump();
  #endif
}

void EventHandler::on_mouse_right_button_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.mouse.dump();
  #endif
}

void EventHandler::on_mouse_button_four_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.mouse.dump();
  #endif
}

void EventHandler::on_mouse_button_five_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_MOUSE_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.mouse.dump();
  #endif
}

void EventHandler::on_joy_axis( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_JOYSTICK_AXIS_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.jaxis.dump();
  #endif
}

void EventHandler::on_joy_button_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_JOYSTICK_BUTTON_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.jbutton.dump();
  #endif
}

void EventHandler::on_joy_button_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_JOYSTICK_BUTTON_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.jbutton.dump();
  #endif
}

void EventHandler::on_joystick_connected( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_JOYSTICK_EVENT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.jconnected.dump();
  #endif
}
void EventHandler::on_joystick_disconnected( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_JOYSTICK_EVENT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.jdisconnected.dump();
  #endif
}

void EventHandler::on_touch_motion( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_TOUCH_MOTION_EVENT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.touch.dump();
  #endif
}

void EventHandler::on_touch_down( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_TOUCH_EVENT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.touch.dump();
  #endif
}

void EventHandler::on_touch_up( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_TOUCH_EVENT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.touch.dump();
  #endif
}

void EventHandler::on_gesture( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_GESTURE_EVENT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.gesture.dump();
  #endif
}

void EventHandler::on_drag_drop( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_DRAG_DROP_INPUT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.drop.dump();
  #endif
}

void EventHandler::on_text_input( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_TEXT_INPUT_EVENT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.text.dump();
  #endif
}

void EventHandler::on_text_edit( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_TEXT_EDIT_EVENT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.edit.dump();
  #endif
}

void EventHandler::on_user_event( const Event& ev )
{
  // User implemented

  #if defined( NOM_DEBUG_SDL2_USER_EVENT )
    NOM_LOG_TRACE( NOM );
    ev.dump();
    ev.user.dump();
  #endif
}

bool EventHandler::pop_event( Event& ev )
{
  // Empty event queue -- it's time to start gathering events
  if( this->events_.empty() )
  {
    // Use the underlying events subsystem (SDL2 events) to poll for available
    // events. Once processed, our wrapped nom::Event queue should contain the
    // same SDL_Event struct(s) preicsely, less & except minus any event type we
    // may omit from pushing into our events queue.
    this->process_events();

    // TODO:
    // Implement blocking mode logic; we must process events until one is
    // triggered.
    // if( block )
    // {
    //   while( this->events_.empty() )
    //   {
    //     this->process_events();
    //   }
    // }
  }

  // Events queue is NOT empty; return the top of the queue stack to the
  // end-user as the current event and mark it processed (remove it from queue).
  if( ! this->events_.empty() )
  {
    ev = this->events_.front();
    this->events_.pop();

    return true;
  }

  return false;
}

void EventHandler::push_event( const Event& ev )
{
  this->events_.push( ev );
}

void EventHandler::process_event( const SDL_Event* ev )
{
  // Create our event structure from the existing SDL_Event information.
  switch( ev->type )
  {
    default: break;

    case SDL_QUIT:
    {
      Event event;
      event.type = ev->quit.type;
      event.timestamp = ev->quit.timestamp;
      this->push_event( event );
      break;
    }

    case SDL_WINDOWEVENT:
    {
      switch( ev->window.event )
      {
        default: break;

        // Not implemented as per SDL2 wiki documentation
        case SDL_WINDOWEVENT_NONE: break;

        case SDL_WINDOWEVENT_SHOWN:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_HIDDEN:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_EXPOSED:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_MOVED:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_RESIZED:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_SIZE_CHANGED:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_MINIMIZED:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_MAXIMIZED:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_RESTORED:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_ENTER:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_LEAVE:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_FOCUS_GAINED:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_FOCUS_LOST:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }

        case SDL_WINDOWEVENT_CLOSE:
        {
          Event event;
          event.type = ev->window.type;
          event.timestamp = ev->window.timestamp;
          event.window.event = ev->window.event;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event( event );
          break;
        }
      } // end switch ev->window.event
    } // end case SDL_WINDOWEVENT

    case SDL_SYSWMEVENT: break;

    case SDL_KEYDOWN:
    {
      Event event;
      event.type = ev->key.type;
      event.timestamp = ev->key.timestamp;
      event.key.scan_code = ev->key.keysym.scancode;
      event.key.sym = ev->key.keysym.sym;
      event.key.mod = ev->key.keysym.mod;
      event.key.state = ev->key.state;
      event.key.repeat = ev->key.repeat;
      event.key.window_id = ev->key.windowID;
      this->push_event( event );
      break;
    }

    case SDL_KEYUP:
    {
      Event event;
      event.type = ev->key.type;
      event.timestamp = ev->key.timestamp;
      event.key.scan_code = ev->key.keysym.scancode;
      event.key.sym = ev->key.keysym.sym;
      event.key.mod = ev->key.keysym.mod;
      event.key.state = ev->key.state;
      event.key.repeat = ev->key.repeat;
      event.key.window_id = ev->key.windowID;
      this->push_event( event );
      break;
    }

    case SDL_MOUSEMOTION:
    {
      Event event;
      event.type = ev->motion.type;
      event.timestamp = ev->motion.timestamp;
      event.motion.id = ev->motion.which;
      event.motion.x = ev->motion.x;
      event.motion.y = ev->motion.y;
      event.motion.x_rel = ev->motion.xrel;
      event.motion.y_rel = ev->motion.yrel;
      event.motion.state = ev->motion.state;
      event.motion.window_id = ev->motion.windowID;
      this->push_event( event );
      break;
    }

    case SDL_MOUSEBUTTONDOWN:
    {
      switch ( ev->button.button )
      {
        default: break;

        case SDL_BUTTON_LEFT:
        {
          Event event;
          event.type = ev->button.type;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = ev->button.button;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event( event );
          break;
        }

        case SDL_BUTTON_MIDDLE:
        {
          Event event;
          event.type = ev->button.type;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = ev->button.button;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event( event );
          break;
        }

        case SDL_BUTTON_RIGHT:
        {
          Event event;
          event.type = ev->button.type;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = ev->button.button;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event( event );
          break;
        }

        case SDL_BUTTON_X1:
        {
          Event event;
          event.type = ev->button.type;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = ev->button.button;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event( event );
          break;
        }

        case SDL_BUTTON_X2:
        {
          Event event;
          event.type = ev->button.type;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = ev->button.button;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event( event );
          break;
        }
      } // end switch ev->button.button
    } // end switch SDL_MOUSEBUTTONDOWN

    case SDL_MOUSEBUTTONUP:
    {
      switch ( ev->button.button )
      {
        default: break;

        case SDL_BUTTON_LEFT:
        {
          Event event;
          event.type = ev->button.type;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = ev->button.button;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event( event );
          break;
        }

        case SDL_BUTTON_MIDDLE:
        {
          Event event;
          event.type = ev->button.type;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = ev->button.button;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event( event );
          break;
        }

        case SDL_BUTTON_RIGHT:
        {
          Event event;
          event.type = ev->button.type;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = ev->button.button;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event( event );
          break;
        }

        case SDL_BUTTON_X1:
        {
          Event event;
          event.type = ev->button.type;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = ev->button.button;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event( event );
          break;
        }

        case SDL_BUTTON_X2:
        {
          Event event;
          event.type = ev->button.type;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = ev->button.button;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event( event );
          break;
        }

      } // end switch ev->button.button
    } // end switch SDL_MOUSEBUTTONUP

    case SDL_MOUSEWHEEL:
    {
      Event event;
      event.type = ev->wheel.type;
      event.timestamp = ev->wheel.timestamp;
      event.wheel.id = ev->wheel.which;
      event.wheel.x = ev->wheel.x;
      event.wheel.y = ev->wheel.y;
      event.wheel.window_id = ev->wheel.windowID;
      this->push_event( event );
      break;
    }

    case SDL_JOYBUTTONDOWN:
    {
      Event event;
      event.type = ev->jbutton.type;
      event.timestamp = ev->jbutton.timestamp;
      event.jbutton.id = this->joystick.first_joystick();
      event.jbutton.button = ev->jbutton.button;
      event.jbutton.state = ev->jbutton.state;
      // event.jbutton.window_id = ev->jbutton.windowID;
      this->push_event( event );
      break;
    }

    case SDL_JOYBUTTONUP:
    {
      Event event;
      event.type = ev->jbutton.type;
      event.timestamp = ev->jbutton.timestamp;
      event.jbutton.id = this->joystick.first_joystick();
      event.jbutton.button = ev->jbutton.button;
      event.jbutton.state = ev->jbutton.state;
      // event.jbutton.window_id = ev->jbutton.windowID;
      this->push_event( event );
      break;
    }

    case SDL_JOYAXISMOTION:
    {
      Event event;
      event.type = ev->jaxis.type;
      event.timestamp = ev->jaxis.timestamp;
      event.jaxis.id = this->joystick.first_joystick();
      event.jaxis.axis = ev->jaxis.axis;
      event.jaxis.value = ev->jaxis.value;
      // event.jbutton.window_id = ev->jbutton.windowID;
      this->push_event( event );
      break;
    }

    case SDL_JOYDEVICEADDED:
    {
      // Poll available joystick devices in order to react to joysticks that are
      // connected after initialization of the engine.
      this->joystick.enumerate_devices();

      Event event;
      event.type = ev->jdevice.type;
      event.timestamp = ev->jdevice.timestamp;
      event.jconnected.id = this->joystick.first_joystick();
      this->push_event( event );
      break;
    }

    case SDL_JOYDEVICEREMOVED:
    {
      // Poll available joystick devices in order to react to joysticks that are
      // connected after initialization of the engine.
      this->joystick.enumerate_devices();

      Event event;
      event.type = ev->jdevice.type;
      event.timestamp = ev->jdevice.timestamp;
      event.jdisconnected.id = this->joystick.first_joystick();
      this->push_event( event );
      break;
    }

    case SDL_FINGERMOTION:
    {
      Event event;
      event.type = ev->tfinger.type;
      event.timestamp = ev->tfinger.timestamp;
      event.touch.id = ev->tfinger.touchId;
      event.touch.finger.id = ev->tfinger.fingerId;
      event.touch.x = ev->tfinger.x;
      event.touch.y = ev->tfinger.y;
      event.touch.dx = ev->tfinger.dx;
      event.touch.dy = ev->tfinger.dy;
      event.touch.pressure = ev->tfinger.pressure;
      this->push_event( event );
      break;
    }

    case SDL_FINGERDOWN:
    {
      Event event;
      event.type = ev->tfinger.type;
      event.timestamp = ev->tfinger.timestamp;
      event.touch.id = ev->tfinger.touchId;
      event.touch.finger.id = ev->tfinger.fingerId;
      event.touch.x = ev->tfinger.x;
      event.touch.y = ev->tfinger.y;
      event.touch.dx = ev->tfinger.dx;
      event.touch.dy = ev->tfinger.dy;
      event.touch.pressure = ev->tfinger.pressure;
      this->push_event( event );
      break;
    }

    case SDL_FINGERUP:
    {
      Event event;
      event.type = ev->tfinger.type;
      event.timestamp = ev->tfinger.timestamp;
      event.touch.id = ev->tfinger.touchId;
      event.touch.finger.id = ev->tfinger.fingerId;
      event.touch.x = ev->tfinger.x;
      event.touch.y = ev->tfinger.y;
      event.touch.dx = ev->tfinger.dx;
      event.touch.dy = ev->tfinger.dy;
      event.touch.pressure = ev->tfinger.pressure;
      this->push_event( event );
      break;
    }

    case SDL_MULTIGESTURE:
    {
      Event event;
      event.type = ev->mgesture.type;
      event.timestamp = ev->mgesture.timestamp;
      event.gesture.id = ev->mgesture.touchId;
      event.gesture.dTheta = ev->mgesture.dTheta;
      event.gesture.dDist = ev->mgesture.dDist;
      event.gesture.x = ev->mgesture.x;
      event.gesture.y = ev->mgesture.y;
      event.gesture.num_fingers = ev->mgesture.numFingers;
      this->push_event( event );
      break;
    }

    case SDL_DROPFILE:
    {
      Event event;
      event.type = ev->drop.type;
      event.timestamp = ticks();
      event.drop.file_path = ev->drop.file;
      this->push_event( event );
      break;
    }

    case SDL_TEXTINPUT:
    {
      Event event;
      event.type = ev->text.type;
      event.timestamp = ev->text.timestamp;
      strcpy( event.text.text, ev->text.text );
      event.text.window_id = ev->text.windowID;
      this->push_event( event );
      break;
    }

    case SDL_TEXTEDITING: // FIXME
    {
      Event event;
      event.type = ev->edit.type;
      event.timestamp = ev->edit.timestamp;
      strcpy( event.edit.text, ev->edit.text );
      event.edit.window_id = ev->edit.windowID;
      this->push_event( event );
      break;
    }

    case SDL_USEREVENT:
    {
      Event event;
      event.type = ev->user.type;
      event.timestamp = ev->user.timestamp;
      event.user.code = ev->user.code;
      event.user.data1 = ev->user.data1;
      event.user.data2 = ev->user.data2;
      event.user.window_id = ev->user.windowID;
      this->push_event( event );
      break;
    }
  } // end switch event->type
}

void EventHandler::process_events( void )
{
  SDL_Event ev;

  // Enumerate events from all available input devices
  SDL_PumpEvents();

  while( SDL_PeepEvents( &ev, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT ) )
  {
    this->process_event( &ev );
  }
}

} // namespace nom
