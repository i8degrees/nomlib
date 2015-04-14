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

// Private headers
#include "nomlib/core/err.hpp"
#include "nomlib/core/clock.hpp"
#include "nomlib/core/helpers.hpp"
#include "nomlib/system/SDL_helpers.hpp"
#include "nomlib/system/JoystickEventHandler.hpp"
#include "nomlib/system/GameControllerEventHandler.hpp"

#include <SDL.h>

static_assert(  nom::InputState::RELEASED ==
                SDL_RELEASED, "Event mismatch" );
static_assert(  nom::InputState::PRESSED ==
                SDL_PRESSED, "Event mismatch" );

static_assert(  nom::MouseButton::LEFT_MOUSE_BUTTON ==
                SDL_BUTTON_LEFT, "Event mismatch" );
static_assert(  nom::MouseButton::MIDDLE_MOUSE_BUTTON ==
                SDL_BUTTON_MIDDLE, "Event mismatch" );
static_assert(  nom::MouseButton::RIGHT_MOUSE_BUTTON ==
                SDL_BUTTON_RIGHT, "Event mismatch" );
static_assert(  nom::MouseButton::X1_MOUSE_BUTTON ==
                SDL_BUTTON_X1, "Event mismatch" );
static_assert(  nom::MouseButton::X2_MOUSE_BUTTON ==
                SDL_BUTTON_X2, "Event mismatch" );

namespace nom {

// Forward declarations
struct event_watcher
{
  event_filter callback = nullptr;
  void* data1 = nullptr;
};

EventHandler::EventHandler()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_EVENT,
                      NOM_LOG_PRIORITY_VERBOSE );
}

EventHandler::~EventHandler()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_EVENT,
                      NOM_LOG_PRIORITY_VERBOSE );

  auto enable_report = nom::hint("NOM_EVENT_QUEUE_STATISTICS");
  if( nom::string_to_integer(enable_report) != 0 ) {
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_APPLICATION,
                    "num_events:", this->num_events() );
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_APPLICATION,
                    "max_events_count:", max_events_count_ );
  }

  if( this->joystick_event_type() == SDL_JOYSTICK_EVENT_HANDLER ) {
    this->disable_joystick_polling();
  } else if( this->joystick_event_type() == GAME_CONTROLLER_EVENT_HANDLER ) {
    this->disable_game_controller_polling();
  }
}

nom::size_type EventHandler::num_events() const
{
  return this->events_.size();
}

nom::size_type EventHandler::num_event_watchers() const
{
  return this->event_watchers_.size();
}

JoystickEventHandler* EventHandler::joystick_event_handler() const
{
  auto result = (JoystickEventHandler*)this->joystick_event_handler_;

  return result;
}

GameControllerEventHandler* EventHandler::game_controller_event_handler() const
{
  auto result = (GameControllerEventHandler*)this->joystick_event_handler_;

  return result;
}

EventHandler::JoystickHandlerType
EventHandler::joystick_event_type() const
{
  return this->joystick_event_type_;
}

bool EventHandler::enable_joystick_polling()
{
  if( nom::init_joystick_subsystem() == false ) {
    return false;
  }

  this->joystick_event_handler_ = new JoystickEventHandler();
  if( this->joystick_event_handler_ == nullptr ) {
    nom::set_error(nom::OUT_OF_MEMORY_ERR);
    return false;
  }

  this->joystick_event_type_ = SDL_JOYSTICK_EVENT_HANDLER;

  return true;
}

bool EventHandler::enable_game_controller_polling()
{
  if( nom::init_game_controller_subsystem() == false ) {
    return false;
  }

  this->joystick_event_handler_ = new GameControllerEventHandler();
  if( this->joystick_event_handler_ == nullptr ) {
    nom::set_error(nom::OUT_OF_MEMORY_ERR);
    return false;
  }

  this->joystick_event_type_ = GAME_CONTROLLER_EVENT_HANDLER;

  return true;
}

void EventHandler::disable_joystick_polling()
{
  if( this->joystick_event_type() == SDL_JOYSTICK_EVENT_HANDLER ) {

    auto evt_handler = this->joystick_event_handler();
    NOM_DELETE_PTR(evt_handler);

    this->joystick_event_type_ = NO_EVENT_HANDLER;
    nom::shutdown_joystick_subsystem();
  } else if( this->joystick_event_type() == NO_EVENT_HANDLER ) {
    // Nothing to do
  } else {
    // Possible memory leak
    NOM_ASSERT_INVALID_PATH();
  }

  this->joystick_event_handler_ = nullptr;
}

void EventHandler::disable_game_controller_polling()
{
  if( this->joystick_event_type() == GAME_CONTROLLER_EVENT_HANDLER ) {

    auto evt_handler = this->game_controller_event_handler();
    NOM_DELETE_PTR(evt_handler);

    this->joystick_event_type_ = NO_EVENT_HANDLER;
    nom::shutdown_game_controller_subsystem();
  } else if( this->joystick_event_type() == NO_EVENT_HANDLER ) {
    // Nothing to do
  } else {
    // Possible memory leak
    NOM_ASSERT_INVALID_PATH();
  }

  this->joystick_event_handler_ = nullptr;
}

bool EventHandler::poll_event(Event& ev)
{
  if( this->pop_event(ev) == true ) {
    // Pending events
    return true;
  } else {
    // No pending events in queue
    return false;
  }
}

void EventHandler::append_event_watch(const event_filter& filter, void* data)
{
  if( filter == nullptr ) {
    return;
  }

  auto event_watch = nom::make_unique<event_watcher>();
  if( event_watch == nullptr ) {
    // Err -- out of memory..??
    NOM_ASSERT_INVALID_PATH();
    return;
  }

  event_watch->callback = filter;
  event_watch->data1 = data;
  this->event_watchers_.push_back( std::move(event_watch) );
}

void EventHandler::remove_event_watch(const event_filter& filter)
{
  if( filter == nullptr ) {
    return;
  }

  auto& evt_watch = this->event_watchers_;
  for( auto itr = evt_watch.begin(); itr != evt_watch.end(); ++itr ) {

    event_filter* callback = (*itr)->callback.target<event_filter>();
    const event_filter* arg = filter.target<event_filter>();

    if( callback == arg ) {
      evt_watch.erase(itr);
      return;
    }
  }
}

void EventHandler::remove_event_watchers()
{
  this->event_watchers_.clear();
}

void EventHandler::push_event(const Event& ev)
{
  nom::size_type num_events = 0;
  this->events_.emplace_back(ev);

  num_events = this->num_events();
  if( num_events > this->max_events_count_ ) {
    this->max_events_count_ = num_events;
  }

  auto& evt_watch = this->event_watchers_;
  for( auto itr = evt_watch.begin(); itr != evt_watch.end(); ++itr ) {
    if( (*itr)->callback != nullptr ) {
      (*itr)->callback.operator()(ev, (*itr)->data1);
    }
  }
}

bool EventHandler::pop_event(Event& ev)
{
  bool result = false;

  if( this->events_.empty() == true ) {

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

    result = false;
  }

  if( this->events_.empty() == false ) {

    // Leave a copy of the reference for end-user retrieval
    ev = this->events_.front();

    this->events_.pop_front();
    result = true;
  }

  return result;
}

void EventHandler::process_events()
{
  int result = 1;
  SDL_Event ev;

  // Enumerate events from all available input devices
  SDL_PumpEvents();

  while( result > 0 ) {
    result =
      SDL_PeepEvents(&ev, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);

    if( result < 0 ) {
      NOM_ASSERT_INVALID_PATH();
    } else if( result > 0 ) {

      // Enqueue retrieved events from underlying platform (SDL)

      if( this->joystick_event_handler_ != nullptr ) {

        auto type = this->joystick_event_type();
        if( type == SDL_JOYSTICK_EVENT_HANDLER ) {
          this->process_joystick_event(&ev);
        } else if( type == GAME_CONTROLLER_EVENT_HANDLER ) {
          this->process_game_controller_event(&ev);
        }
      }

      this->process_event(&ev);
    }
  } // end while
}

void EventHandler::process_event(const SDL_Event* ev)
{
  switch(ev->type)
  {
    default: break;

    case SDL_QUIT:
    {
      nom::Event event;
      event.type = Event::QUIT_EVENT;
      event.timestamp = ev->quit.timestamp;
      // NOTE: user-defined fields
      event.quit.data1 = nullptr;
      event.quit.data2 = nullptr;
      this->push_event(event);
    } break;

    case SDL_WINDOWEVENT:
    {
      switch(ev->window.event)
      {
        case SDL_WINDOWEVENT_NONE:
        default: break;

        case SDL_WINDOWEVENT_SHOWN:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::SHOWN;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_HIDDEN:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::HIDDEN;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_EXPOSED:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::EXPOSED;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_MOVED:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::MOVED;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_RESIZED:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::RESIZED;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_SIZE_CHANGED:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::SIZE_CHANGED;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_MINIMIZED:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::MINIMIZED;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_MAXIMIZED:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::MAXIMIZED;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_RESTORED:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::RESTORED;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_ENTER:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::MOUSE_FOCUS_GAINED;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_LEAVE:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::MOUSE_FOCUS_LOST;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_FOCUS_GAINED:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::KEYBOARD_FOCUS_GAINED;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_FOCUS_LOST:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::KEYBOARD_FOCUS_LOST;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }

        case SDL_WINDOWEVENT_CLOSE:
        {
          Event event;
          event.type = Event::WINDOW_EVENT;
          event.timestamp = ev->window.timestamp;
          event.window.event = WindowEvent::CLOSE;
          event.window.data1 = ev->window.data1;
          event.window.data2 = ev->window.data2;
          event.window.window_id = ev->window.windowID;
          this->push_event(event);
          break;
        }
      } // end switch ev->window.event

      break;
    } // end case SDL_WINDOWEVENT

    case SDL_SYSWMEVENT: break;

    case SDL_KEYDOWN:
    {
      Event event;
      event.type = Event::KEY_PRESS;
      event.timestamp = ev->key.timestamp;
      event.key.scan_code = ev->key.keysym.scancode;
      event.key.sym = ev->key.keysym.sym;
      event.key.mod = ev->key.keysym.mod;
      event.key.state = ev->key.state;
      event.key.repeat = ev->key.repeat;
      event.key.window_id = ev->key.windowID;
      this->push_event(event);
      break;
    }

    case SDL_KEYUP:
    {
      Event event;
      event.type = Event::KEY_RELEASE;
      event.timestamp = ev->key.timestamp;
      event.key.scan_code = ev->key.keysym.scancode;
      event.key.sym = ev->key.keysym.sym;
      event.key.mod = ev->key.keysym.mod;
      event.key.state = ev->key.state;
      event.key.repeat = ev->key.repeat;
      event.key.window_id = ev->key.windowID;
      this->push_event(event);
      break;
    }

    case SDL_MOUSEMOTION:
    {
      Event event;
      event.type = Event::MOUSE_MOTION;
      event.timestamp = ev->motion.timestamp;
      event.motion.id = ev->motion.which;
      event.motion.x = ev->motion.x;
      event.motion.y = ev->motion.y;
      event.motion.x_rel = ev->motion.xrel;
      event.motion.y_rel = ev->motion.yrel;
      event.motion.state = ev->motion.state;
      event.motion.window_id = ev->motion.windowID;
      this->push_event(event);
      break;
    }

    case SDL_MOUSEBUTTONDOWN:
    {
      switch (ev->button.button)
      {
        default: break;

        case SDL_BUTTON_LEFT:
        {
          Event event;
          event.type = Event::MOUSE_BUTTON_CLICK;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = MouseButton::LEFT_MOUSE_BUTTON;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event(event);
          break;
        }

        case SDL_BUTTON_MIDDLE:
        {
          Event event;
          event.type = Event::MOUSE_BUTTON_CLICK;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = MouseButton::MIDDLE_MOUSE_BUTTON;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event(event);
          break;
        }

        case SDL_BUTTON_RIGHT:
        {
          Event event;
          event.type = Event::MOUSE_BUTTON_CLICK;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = MouseButton::RIGHT_MOUSE_BUTTON;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event(event);
          break;
        }

        case SDL_BUTTON_X1:
        {
          Event event;
          event.type = Event::MOUSE_BUTTON_CLICK;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = MouseButton::X1_MOUSE_BUTTON;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event(event);
          break;
        }

        case SDL_BUTTON_X2:
        {
          Event event;
          event.type = Event::MOUSE_BUTTON_CLICK;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = MouseButton::X2_MOUSE_BUTTON;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event(event);
          break;
        }
      } // end switch ev->button.button

      break;
    } // end switch SDL_MOUSEBUTTONDOWN

    case SDL_MOUSEBUTTONUP:
    {
      switch ( ev->button.button )
      {
        default: break;

        case SDL_BUTTON_LEFT:
        {
          Event event;
          event.type = Event::MOUSE_BUTTON_RELEASE;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = MouseButton::LEFT_MOUSE_BUTTON;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event(event);
          break;
        }

        case SDL_BUTTON_MIDDLE:
        {
          Event event;
          event.type = Event::MOUSE_BUTTON_RELEASE;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = MouseButton::MIDDLE_MOUSE_BUTTON;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event(event);
          break;
        }

        case SDL_BUTTON_RIGHT:
        {
          Event event;
          event.type = Event::MOUSE_BUTTON_RELEASE;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = MouseButton::RIGHT_MOUSE_BUTTON;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event(event);
          break;
        }

        case SDL_BUTTON_X1:
        {
          Event event;
          event.type = Event::MOUSE_BUTTON_RELEASE;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = MouseButton::X1_MOUSE_BUTTON;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event(event);
          break;
        }

        case SDL_BUTTON_X2:
        {
          Event event;
          event.type = Event::MOUSE_BUTTON_RELEASE;
          event.timestamp = ev->button.timestamp;
          event.mouse.id = ev->button.which;
          event.mouse.x = ev->button.x;
          event.mouse.y = ev->button.y;
          event.mouse.button = MouseButton::X2_MOUSE_BUTTON;
          event.mouse.state = ev->button.state;
          event.mouse.clicks = ev->button.clicks;
          event.mouse.window_id = ev->button.windowID;
          this->push_event(event);
          break;
        }
      } // end switch ev->button.button

      break;
    } // end switch SDL_MOUSEBUTTONUP

    case SDL_MOUSEWHEEL:
    {
      Event event;
      event.type = Event::MOUSE_WHEEL;
      event.timestamp = ev->wheel.timestamp;
      event.wheel.id = ev->wheel.which;
      event.wheel.x = ev->wheel.x;
      event.wheel.y = ev->wheel.y;
      event.wheel.window_id = ev->wheel.windowID;
      this->push_event(event);
      break;
    }

    case SDL_FINGERMOTION:
    {
      Event event;
      event.type = Event::FINGER_MOTION;
      event.timestamp = ev->tfinger.timestamp;
      event.touch.id = ev->tfinger.touchId;
      event.touch.finger.id = ev->tfinger.fingerId;
      event.touch.x = ev->tfinger.x;
      event.touch.y = ev->tfinger.y;
      event.touch.dx = ev->tfinger.dx;
      event.touch.dy = ev->tfinger.dy;
      event.touch.pressure = ev->tfinger.pressure;
      this->push_event(event);
      break;
    }

    case SDL_FINGERDOWN:
    {
      Event event;
      event.type = Event::FINGER_PRESS;
      event.timestamp = ev->tfinger.timestamp;
      event.touch.id = ev->tfinger.touchId;
      event.touch.finger.id = ev->tfinger.fingerId;
      event.touch.x = ev->tfinger.x;
      event.touch.y = ev->tfinger.y;
      event.touch.dx = ev->tfinger.dx;
      event.touch.dy = ev->tfinger.dy;
      event.touch.pressure = ev->tfinger.pressure;
      this->push_event(event);
      break;
    }

    case SDL_FINGERUP:
    {
      Event event;
      event.type = Event::FINGER_RELEASE;
      event.timestamp = ev->tfinger.timestamp;
      event.touch.id = ev->tfinger.touchId;
      event.touch.finger.id = ev->tfinger.fingerId;
      event.touch.x = ev->tfinger.x;
      event.touch.y = ev->tfinger.y;
      event.touch.dx = ev->tfinger.dx;
      event.touch.dy = ev->tfinger.dy;
      event.touch.pressure = ev->tfinger.pressure;
      this->push_event(event);
      break;
    }

    case SDL_MULTIGESTURE:
    {
      Event event;
      event.type = Event::MULTI_FINGER_GESTURE;
      event.timestamp = ev->mgesture.timestamp;
      event.gesture.id = ev->mgesture.touchId;
      event.gesture.dTheta = ev->mgesture.dTheta;
      event.gesture.dDist = ev->mgesture.dDist;
      event.gesture.x = ev->mgesture.x;
      event.gesture.y = ev->mgesture.y;
      event.gesture.num_fingers = ev->mgesture.numFingers;
      this->push_event(event);
      break;
    }

    case SDL_DROPFILE:
    {
      Event event;
      event.type = Event::DROP_FILE;
      event.timestamp = nom::ticks();
      event.drop.file_path = ev->drop.file;
      this->push_event(event);
      break;
    }

    case SDL_TEXTINPUT:
    {
      Event event;
      event.type = Event::TEXT_INPUT;
      event.timestamp = ev->text.timestamp;
      nom::copy_string(ev->text.text, event.text.text);
      event.text.window_id = ev->text.windowID;
      this->push_event(event);
      break;
    }

    case SDL_TEXTEDITING:
    {
      Event event;
      event.type = Event::TEXT_EDITING;
      event.timestamp = ev->edit.timestamp;
      event.edit.start = ev->edit.start;
      event.edit.length = ev->edit.length;
      nom::copy_string(ev->edit.text, event.edit.text);
      event.edit.window_id = ev->edit.windowID;
      this->push_event(event);
      break;
    }

    case SDL_RENDER_TARGETS_RESET:
    {
      Event event;
      event.type = Event::RENDER_TARGETS_RESET;
      event.timestamp = nom::ticks();
    } break;

// NOTE: Not available until the release of SDL 2.0.4
#if 0
    case SDL_RENDER_DEVICE_RESET:
    {
      Event event;
      event.type = Event::RENDER_DEVICE_RESET;
      event.timestamp = nom::ticks();
    } break;
#endif
    case SDL_USEREVENT:
    {
      nom::Event event;
      event.type = Event::USER_EVENT;
      event.timestamp = ev->user.timestamp;
      event.user.code = ev->user.code;
      event.user.data1 = ev->user.data1;
      event.user.data2 = ev->user.data2;
      event.user.window_id = ev->user.windowID;
      this->push_event(event);
    } break;
  } // end switch event->type
}

void EventHandler::process_joystick_event(const SDL_Event* ev)
{
  NOM_ASSERT(this->joystick_event_type() == SDL_JOYSTICK_EVENT_HANDLER);

  auto evt_handler = this->joystick_event_handler();
  NOM_ASSERT(evt_handler != nullptr);

  switch(ev->type)
  {
    default: break;

    case SDL_JOYDEVICEADDED:
    {
      Event event;
      event.type = Event::JOYSTICK_ADDED;
      event.timestamp = ev->jdevice.timestamp;
      event.jdevice.id = ev->jdevice.which;
      this->push_event(event);

      auto dev_index = event.jdevice.id;
      auto joy_dev = evt_handler->add_joystick(dev_index);
      if( joy_dev != nullptr ) {
        auto dev_id = joy_dev->device_id();
        NOM_LOG_INFO( NOM_LOG_CATEGORY_EVENT,
                      "Registered joystick instance ID",
                      dev_id, "for", joy_dev->name() );
      } else {
        NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                      "Failed to register joystick:", nom::error() );
      }
    } break;

    // IMPORTANT: The joystick device state is invalid by the time we get this
    // message!
    case SDL_JOYDEVICEREMOVED:
    {
      Event event;
      event.type = Event::JOYSTICK_REMOVED;
      event.timestamp = ev->jdevice.timestamp;
      event.jdevice.id = ev->jdevice.which;
      this->push_event(event);

      auto dev_id = event.jdevice.id;
      if( evt_handler->remove_joystick(dev_id) == true ) {
        NOM_LOG_INFO( NOM_LOG_CATEGORY_EVENT,
                      "Removing registered instance ID", dev_id );
      } else {
        NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                      "Failed to remove registered instance ID:",
                      nom::error() );
      }
    } break;

    case SDL_JOYBUTTONDOWN:
    {
      Event event;
      event.type = Event::JOYSTICK_BUTTON_PRESS;
      event.timestamp = ev->jbutton.timestamp;
      event.jbutton.id = ev->jbutton.which;
      event.jbutton.button = ev->jbutton.button;
      event.jbutton.state = ev->jbutton.state;
      this->push_event(event);
    } break;

    case SDL_JOYBUTTONUP:
    {
      Event event;
      event.type = Event::JOYSTICK_BUTTON_RELEASE;
      event.timestamp = ev->jbutton.timestamp;
      event.jbutton.id = ev->jbutton.which;
      event.jbutton.button = ev->jbutton.button;
      event.jbutton.state = ev->jbutton.state;
      this->push_event(event);
    } break;

    case SDL_JOYAXISMOTION:
    {
      Event event;
      event.type = Event::JOYSTICK_AXIS_MOTION;
      event.timestamp = ev->jaxis.timestamp;
      event.jaxis.id = ev->jaxis.which;
      event.jaxis.axis = ev->jaxis.axis;
      event.jaxis.value = ev->jaxis.value;
      this->push_event(event);
    } break;

    case SDL_JOYHATMOTION:
    {
      Event event;
      event.type = Event::JOYSTICK_HAT_MOTION;
      event.timestamp = ev->jhat.timestamp;
      event.jhat.id = ev->jhat.which;
      event.jhat.hat = ev->jhat.hat;
      event.jhat.value = ev->jhat.value;
      this->push_event(event);
    } break;
  }
}

void EventHandler::process_game_controller_event(const SDL_Event* ev)
{
  NOM_ASSERT(this->joystick_event_type() == GAME_CONTROLLER_EVENT_HANDLER);

  auto evt_handler = this->game_controller_event_handler();
  NOM_ASSERT(evt_handler != nullptr);

  switch(ev->type)
  {
    default: break;

    case SDL_CONTROLLERAXISMOTION:
    {
      Event event;
      event.type = Event::GAME_CONTROLLER_AXIS_MOTION;
      event.timestamp = ev->caxis.timestamp;
      event.caxis.id = ev->caxis.which;
      event.caxis.axis = ev->caxis.axis;
      event.caxis.value = ev->caxis.value;
      this->push_event(event);
    } break;

    case SDL_CONTROLLERBUTTONDOWN:
    {
      Event event;
      event.type = Event::GAME_CONTROLLER_BUTTON_PRESS;
      event.timestamp = ev->cbutton.timestamp;
      event.cbutton.id = ev->cbutton.which;
      event.cbutton.button = ev->cbutton.button;
      event.cbutton.state = ev->cbutton.state;
      this->push_event(event);
    } break;

    case SDL_CONTROLLERBUTTONUP:
    {
      Event event;
      event.type = Event::GAME_CONTROLLER_BUTTON_RELEASE;
      event.timestamp = ev->cbutton.timestamp;
      event.cbutton.id = ev->cbutton.which;
      event.cbutton.button = ev->cbutton.button;
      event.cbutton.state = ev->cbutton.state;
      this->push_event(event);
    } break;

    case SDL_CONTROLLERDEVICEADDED:
    {
      Event event;
      event.type = Event::GAME_CONTROLLER_ADDED;
      event.timestamp = ev->cdevice.timestamp;
      event.cdevice.id = ev->cdevice.which;
      this->push_event(event);

      auto dev_index = event.cdevice.id;
      auto joy_dev = evt_handler->add_joystick(dev_index);
      if( joy_dev != nullptr ) {
        auto dev_id = joy_dev->device_id();
        NOM_LOG_INFO( NOM_LOG_CATEGORY_EVENT,
                      "Registered game controller instance ID",
                      dev_id, "for", joy_dev->name() );
      } else {
        NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                      "Failed to register game controller:", nom::error() );
      }
    } break;

    // IMPORTANT: The joystick device state is invalid by the time we get this
    // message!
    case SDL_CONTROLLERDEVICEREMOVED:
    {
      Event event;
      event.type = Event::GAME_CONTROLLER_REMOVED;
      event.timestamp = ev->cdevice.timestamp;
      event.cdevice.id = ev->cdevice.which;
      this->push_event(event);

      auto dev_id = event.cdevice.id;
      if( evt_handler->remove_joystick(dev_id) == true ) {
        NOM_LOG_INFO( NOM_LOG_CATEGORY_EVENT,
                      "Removing registered instance ID", dev_id );
      } else {
        NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                      "Failed to remove registered instance ID:",
                      nom::error() );
      }
    } break;

    // TODO: I have no idea how this event is suppose to work ... we receive
    // more than one of these events at a time -- which instance ID do we use??
    case SDL_CONTROLLERDEVICEREMAPPED:
    {
      Event event;
      event.type = Event::GAME_CONTROLLER_REMAPPED;
      event.timestamp = ev->cdevice.timestamp;
      event.cdevice.id = ev->cdevice.which;
      this->push_event(event);
    } break;
  }
}

void EventHandler::flush_event(Event::EventType type)
{
  for( auto itr = this->events_.begin(); itr != this->events_.end(); ++itr ) {
    if( (*itr).type == type ) {
      this->events_.erase(itr);
      return;
    }
  }
}

void EventHandler::flush_events(Event::EventType type)
{
  for( auto itr = this->events_.begin(); itr != this->events_.end(); ++itr ) {
    if( (*itr).type == type ) {
      this->events_.erase(itr);
    }
  }
}

void EventHandler::flush_events()
{
  this->events_.clear();
}

Event create_key_press(int32 sym, uint16 mod, uint8 repeat)
{
  nom::Event result;
  result.type = Event::KEY_PRESS;
  result.timestamp = nom::ticks();
  result.key.scan_code = SDL_GetScancodeFromKey(sym);
  result.key.sym = sym;
  result.key.mod = mod;
  result.key.state = InputState::PRESSED;
  result.key.repeat = repeat;

  return result;
}

Event create_key_release(int32 sym, uint16 mod, uint8 repeat)
{
  nom::Event result;
  result.type = Event::KEY_RELEASE;
  result.timestamp = nom::ticks();
  result.key.scan_code = SDL_GetScancodeFromKey(sym);
  result.key.sym = sym;
  result.key.mod = mod;
  result.key.state = InputState::RELEASED;
  result.key.repeat = repeat;

  return result;
}

Event create_mouse_button_click(uint8 button, uint8 clicks, uint32 window_id)
{
  nom::Event result;
  result.type = Event::MOUSE_BUTTON_CLICK;
  result.timestamp = nom::ticks();
  result.mouse.button = button;
  result.mouse.state = InputState::PRESSED;
  result.mouse.clicks = clicks;
  result.mouse.window_id = window_id;

  return result;
}

Event create_mouse_button_release(uint8 button, uint8 clicks, uint32 window_id)
{
  nom::Event result;
  result.type = Event::MOUSE_BUTTON_RELEASE;
  result.timestamp = nom::ticks();
  result.mouse.button = button;
  result.mouse.state = InputState::RELEASED;
  result.mouse.clicks = clicks;
  result.mouse.window_id = window_id;

  return result;
}

Event create_joystick_button_press(JoystickID id, uint8 button)
{
  nom::Event result;
  result.type = Event::JOYSTICK_BUTTON_PRESS;
  result.timestamp = nom::ticks();
  result.jbutton.id = id;
  result.jbutton.button = button;
  result.jbutton.state = InputState::PRESSED;

  return result;
}

Event create_joystick_button_release(JoystickID id, uint8 button)
{
  nom::Event result;
  result.type = Event::JOYSTICK_BUTTON_RELEASE;
  result.timestamp = nom::ticks();
  result.jbutton.id = id;
  result.jbutton.button = button;
  result.jbutton.state = InputState::RELEASED;

  return result;
}

Event create_joystick_hat_motion(JoystickID id, uint8 hat, uint8 value)
{
  nom::Event result;
  result.type = Event::JOYSTICK_HAT_MOTION;
  result.timestamp = nom::ticks();
  result.jhat.id = id;
  result.jhat.hat = hat;
  result.jhat.value = value;

  return result;
}

Event create_game_controller_button_press(JoystickID id, uint8 button)
{
  nom::Event result;
  result.type = Event::GAME_CONTROLLER_BUTTON_PRESS;
  result.timestamp = nom::ticks();
  result.cbutton.id = id;
  result.cbutton.button = button;
  result.cbutton.state = InputState::PRESSED;

  return result;
}

Event create_game_controller_button_release(JoystickID id, uint8 button)
{
  nom::Event result;
  result.type = Event::GAME_CONTROLLER_BUTTON_RELEASE;
  result.timestamp = nom::ticks();
  result.cbutton.id = id;
  result.cbutton.button = button;
  result.cbutton.state = InputState::RELEASED;

  return result;
}

Event
create_user_event(int32 code, void* data1, void* data2, uint32 window_id)
{
  nom::Event result;
  result.type = Event::USER_EVENT;
  result.timestamp = nom::ticks();
  result.user.code = code;
  result.user.data1 = data1;
  result.user.data2 = data2;
  result.user.window_id = window_id;

  return result;
}

Event create_quit_event(void* data1, void* data2)
{
  nom::Event result;
  result.type = Event::QUIT_EVENT;
  result.timestamp = nom::ticks();
  result.quit.data1 = data1;
  result.quit.data2 = data2;

  return result;
}

} // namespace nom
