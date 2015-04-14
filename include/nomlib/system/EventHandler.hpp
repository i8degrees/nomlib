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
#ifndef NOMLIB_SYSTEM_EVENT_HANDLER_HPP
#define NOMLIB_SYSTEM_EVENT_HANDLER_HPP

#include <deque>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/system/Event.hpp"

// Forward declarations (third-party)
union SDL_Event;

namespace nom {

// Forward declarations
struct event_watcher;
class JoystickEventHandler;
class GameControllerEventHandler;

/// \brief Event handling abstraction
class EventHandler
{
  public:
    typedef EventHandler self_type;

    enum JoystickHandlerType
    {
      NO_EVENT_HANDLER = 0,
      SDL_JOYSTICK_EVENT_HANDLER,
      GAME_CONTROLLER_EVENT_HANDLER,
    };

    EventHandler();

    ~EventHandler();

    /// \brief Disabled copy constructor.
    EventHandler(const EventHandler& rhs) = delete;

    /// \brief Disabled copy assignment operator.
    EventHandler& operator =(const EventHandler& rhs) = delete;

    /// \brief Get the total number of enqueued events.
    nom::size_type num_events() const;

    /// \brief Get the total number of event watchers.
    nom::size_type num_event_watchers() const;

    /// \brief Get a non-owned pointer to the joystick device event handler.
    ///
    /// \remarks This pointer is invalid until an explicit call has been made
    /// to nom::EventHandler::enable_joystick_polling.
    ///
    /// \see nom::EventHandler::joystick_event_type
    JoystickEventHandler* joystick_event_handler() const;

    /// \brief Get a non-owned pointer to the game controller device event
    /// handler.
    ///
    /// \remarks This pointer is invalid until an explicit call has been made
    /// to nom::EventHandler::enable_game_controller_polling.
    ///
    /// \see nom::EventHandler::joystick_event_type
    GameControllerEventHandler* game_controller_event_handler() const;

    JoystickHandlerType joystick_event_type() const;

    /// \brief Initialize joystick events polling.
    ///
    /// \see nom::JoystickEventHandler
    bool enable_joystick_polling();

    /// \brief Initialize game controller events polling.
    ///
    /// \see nom::GameControllerEventHandler
    bool enable_game_controller_polling();

    /// \brief Shutdown the joystick events polling subsystem.
    void disable_joystick_polling();

    /// \brief Shutdown the game controller events polling subsystem.
    void disable_game_controller_polling();

    /// \brief Enumerate the available events.
    ///
    /// \param ev The nom::Event to use for the retrieved event.
    bool poll_event(Event& ev);

    /// \brief Enqueue an event.
    ///
    /// \remarks This can be used to simulate synthetic input events.
    void push_event(const Event& ev);

    /// \brief Clear an event from the queue.
    ///
    /// \param The type of event to remove.
    void flush_event(Event::EventType type);

    /// \brief Clear events from the queue.
    ///
    /// \param The type of event to remove.
    void flush_events(Event::EventType type);

    /// \brief Clear the events queue.
    void flush_events();

    /// \brief Add an event listener.
    ///
    /// \param data User-defined data; it is your responsibility to free this
    /// data pointer!
    void append_event_watch(const event_filter& filter, void* data);

    /// \brief Erase an event listener.
    void remove_event_watch(const event_filter& filter);

    /// \brief Erase all event watchers.
    void remove_event_watchers();

  private:
    bool pop_event(Event& ev);

    /// \brief Enumerate the available events from the underlying platform.
    void process_events();

    /// \brief Enumerate the available events from the underlying platform.
    void process_event(const SDL_Event* ev);

    void process_joystick_event(const SDL_Event* ev);
    void process_game_controller_event(const SDL_Event* ev);

    /// \brief Enqueued events.
    ///
    /// \see nom::EventHandler::process_event
    std::deque<Event> events_;

    std::vector<std::unique_ptr<event_watcher>> event_watchers_;

    /// \brief The maximum number of events processed per queue cycle -- i.e.:
    /// one frame of the game's update loop.
    nom::size_type max_events_count_ = 0;

    void* joystick_event_handler_ = nullptr;
    JoystickHandlerType joystick_event_type_ = NO_EVENT_HANDLER;
};

Event create_key_press(int32 sym, uint16 mod, uint8 repeat);
Event create_key_release(int32 sym, uint16 mod, uint8 repeat);
Event create_mouse_button_click(uint8 button, uint8 clicks, uint32 window_id);
Event create_mouse_button_release(uint8 button, uint8 clicks, uint32 window_id);

Event create_joystick_button_press(JoystickID id, uint8 button);
Event create_joystick_button_release(JoystickID id, uint8 button);

Event create_joystick_hat_motion(JoystickID id, uint8 hat, uint8 value);

Event create_game_controller_button_press(JoystickID id, uint8 button);
Event create_game_controller_button_release(JoystickID id, uint8 button);

Event create_user_event(int32 code, void* data1, void* data2, uint32 window_id);

Event create_quit_event(void* data1, void* data2);

} // namespace nom

#endif // include guard defined

/// \class nom::EventHandler
/// \ingroup system
///
///         [DESCRIPTION STUB]
///
