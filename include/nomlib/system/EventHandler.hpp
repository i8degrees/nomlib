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
#ifndef NOMLIB_SDL2_SYSTEM_INPUT_HEADERS
#define NOMLIB_SDL2_SYSTEM_INPUT_HEADERS

#include <string>
#include <cstring>
#include <queue>

#include "nomlib/config.hpp"
#include "nomlib/system/SDL_helpers.hpp"
#include "nomlib/system/Joystick.hpp"
#include "nomlib/system/Event.hpp"

// Enable debugging output of quit (end program execution) events.
// #define NOM_DEBUG_SDL2_QUIT_EVENT

// Enable debugging output of user defined events.
// #define NOM_DEBUG_SDL2_USER_EVENT

// Enable debugging output of window events.
// #define NOM_DEBUG_SDL2_WINDOW_INPUT

// Enable debugging output of window focus events.
// #define NOM_DEBUG_SDL2_WINDOW_FOCUS_INPUT

// Enable debugging output of key press & release events.
// #define NOM_DEBUG_SDL2_KEYBOARD_INPUT

// Enable debugging output of mouse button events.
// #define NOM_DEBUG_SDL2_MOUSE_INPUT

// Enable debugging output of mouse motion events.
// #define NOM_DEBUG_SDL2_MOUSE_MOTION_INPUT

// Enable debugging output of joystick button events.
// #define NOM_DEBUG_SDL2_JOYSTICK_BUTTON_INPUT

// Enable debugging output of joystick axis events.
// #define NOM_DEBUG_SDL2_JOYSTICK_AXIS_INPUT

// Enable debugging output of joystick addition & removal events.
// #define NOM_DEBUG_SDL2_JOYSTICK_EVENT

// Enable debugging output of touch motion events.
// #define NOM_DEBUG_SDL2_TOUCH_MOTION_EVENT

// Enable debugging output of finger touch events.
// #define NOM_DEBUG_SDL2_TOUCH_EVENT

// Enable debugging output of multiple finger gesture events.
// #define NOM_DEBUG_SDL2_GESTURE_EVENT

// Enable debugging output of Drag 'N' Drop events.
#define NOM_DEBUG_SDL2_DRAG_DROP_INPUT

// Enable debugging output of text input events.
// #define NOM_DEBUG_SDL2_TEXT_INPUT_EVENT

// Enable debugging output of text editing events.
// #define NOM_DEBUG_SDL2_TEXT_EDIT_EVENT

namespace nom {

/// \brief High-level events handling.
class EventHandler
{
  public:
    /// \brief Default constructor.
    ///
    /// \remarks The joystick subsystem is initialized here.
    EventHandler( void );

    /// \brief Destructor.
    ///
    /// \remarks The joystick subsystem is shutdown within nom::SDLApp.
    virtual ~EventHandler( void );

    /// \brief The high-level events handler.
    ///
    /// \remarks This method is required when overloading the event handlers
    /// provided (via inheritance) from this class.
    void process_event( const Event& ev );

    /// \brief Enumerate the available events from the high-level events
    /// subsystem (queue).
    ///
    /// \param ev The nom::Event structure used to place the top (most current)
    /// event into (if any).
    ///
    /// \note This method must still be called even when using the optional
    /// event handlers provided by this class.
    bool poll_event( Event& ev );

    // virtual bool poll_event( SDL_Event* ev );

  protected:
    /// \brief The application level event generated by either the user or API
    /// requesting that the program execution be shutdown.
    ///
    /// \note This event is necessary for Command + Q functionality under
    /// Mac OS X.
    ///
    /// \remarks type = 256
    virtual void on_app_quit( const Event& ev );

    /// \brief The window event generated once the nom::RenderWindow instance
    /// has been shown.
    ///
    /// \remarks ev.window.event = 1

    virtual void on_window_shown( const Event& ev );

    /// \brief The window event generated once the nom::RenderWindow instance
    /// has been hidden.
    ///
    /// \remarks ev.window.event = 2
    virtual void on_window_hidden( const Event& ev );

    /// \brief The window event generated once the nom::RenderWindow instance
    /// has been exposed and should be redrawn.
    ///
    /// \remarks ev.window.event = 3
    virtual void on_window_exposed( const Event& ev );

    /// \brief The window event generated once the nom::RenderWindow instance
    /// has been moved.
    ///
    /// \param ev The resulting X & Y coordinates of the moved window is
    /// assigned in ev.window.data1 and ev.window.data2, respectively.
    ///
    /// \remarks ev.window.event = 4
    virtual void on_window_moved( const Event& ev );

    /// \brief The window event generated once the nom::RenderWindow instance
    /// has been resized.
    ///
    /// \param ev The resulting width & height coordinates of the resized
    /// nom::RenderWindow instance is assigned in ev.window.data1 and
    /// ev.window.data2, respectively.
    ///
    /// \see ::on_window_size_changed
    ///
    /// \remarks ev.window.event = 5
    virtual void on_window_resized( const Event& ev );

    /// \brief The window event generated after the nom::RenderWindow instance
    /// size -- width & height (in pixels) -- has been changed, either as a
    /// result of an API call, through the system (window manager?) or by the
    /// end-user.
    ///
    /// \param ev The resulting width & height coordinates of the resized
    /// nom::RenderWindow instance is assigned in ev.window.data1 and
    /// ev.window.data2, respectively.
    ///
    /// \remarks This event will be posted **after** ::on_window_resized
    /// (SDL_WINDOWEVENT_RESIZED) if the size was changed by an external event,
    /// such as the user or the window manager.
    ///
    /// \remarks ev.window.event = 6
    virtual void on_window_size_changed( const Event& ev );

    /// \brief The window event generated after the nom::RenderWindow instance
    /// has been minimized.
    ///
    /// \remarks ev.window.event = 7
    virtual void on_window_minimized( const Event& ev );

    /// \brief The window event generated after the nom::RenderWindow instance
    /// has been maximized.
    ///
    /// \remarks ev.window.event = 8
    virtual void on_window_maximized( const Event& ev );

    /// \brief The window event generated after the nom::RenderWindow instance
    /// has been restored to normal size and position.
    ///
    /// \remarks ev.window.event = 9
    virtual void on_window_restored( const Event& ev );

    /// \brief The window event generated once the nom::RenderWindow instance
    /// has gained mouse focus.
    ///
    /// \remarks ev.window.event = 10
    virtual void on_window_mouse_focus( const Event& ev );

    /// \brief The window event generated once the nom::RenderWindow instance
    /// has lost mouse focus.
    ///
    /// \remarks ev.window.event = 11
    virtual void on_window_mouse_focus_lost( const Event& ev );

    /// \brief The window event generated once the nom::RenderWindow instance
    /// has gained keyboard focus.
    ///
    /// \remarks ev.window.event = 12
    virtual void on_window_keyboard_focus( const Event& ev );

    /// \brief The window event generated once the nom::RenderWindow instance
    /// has lost keyboard focus.
    ///
    /// \remarks ev.window.event = 13
    virtual void on_window_keyboard_focus_lost( const Event& ev );

    /// \brief The window event generated (by from the window manager)
    /// requesting that the nom::RenderWindow instance be closed.
    ///
    /// \note ev.window.event = 14
    virtual void on_window_close( const Event& ev );

    /// \brief The keyboard event generated by a key press.
    ///
    /// \remarks type = 768
    virtual void on_key_down( const Event& ev );

    /// \brief The keyboard event generated by a key release.
    ///
    /// \remarks type = 769
    virtual void on_key_up( const Event& ev );

    virtual void on_mouse_motion( const Event& ev );

    virtual void on_mouse_left_button_down( const Event& ev );
    virtual void on_mouse_middle_button_down( const Event& ev );
    virtual void on_mouse_right_button_down( const Event& ev );
    virtual void on_mouse_button_four_down( const Event& ev );
    virtual void on_mouse_button_five_down( const Event& ev );

    virtual void on_mouse_left_button_up( const Event& ev );
    virtual void on_mouse_middle_button_up( const Event& ev );
    virtual void on_mouse_right_button_up( const Event& ev );
    virtual void on_mouse_button_four_up( const Event& ev );
    virtual void on_mouse_button_five_up( const Event& ev );

    /// \brief Handler for mouse wheel events.
    ///
    /// \remarks Upward wheel motion (scroll forward) generates a positive y
    /// value and downward wheel motion (scroll backward) generates a negative
    /// Y value.
    ///     Wheel motion to the left generates a negative X value and motion to
    /// the right generates a positive X value.
    ///
    /// \note The end-user's platform may invert the wheel values documented
    /// here (i.e.: Mac OS X's "Scroll direction: natural" Mouse preference).
    ///
    /// \code
    /// y > 0 = Up
    /// y < 0 = Down
    /// x > 0 = Left
    /// x < 0 = Right
    /// \endcode
    ///
    /// \todo Verify documentation notes regarding left and right wheel axis
    /// values mapping (it's presently only been checked with virtual / fake
    /// hardware).
    virtual void on_mouse_wheel( const Event& ev );

    virtual void on_joy_axis( const Event& ev );

    virtual void on_joy_button_down( const Event& ev );
    virtual void on_joy_button_up( const Event& ev );

    virtual void on_joystick_connected( const Event& ev );
    virtual void on_joystick_disconnected( const Event& ev );

    virtual void on_touch_motion( const Event& ev );
    virtual void on_touch_down( const Event& ev );
    virtual void on_touch_up( const Event& ev );

    /// \brief The finger touch event generated when a multiple finger touch
    /// gesture event is received.
    virtual void on_gesture( const Event& ev );

    /// Drag 'N' Drop events
    ///
    /// \remarks To enable drag and drop events on Mac OS X, you must add the
    /// appropriate keys in your application bundle's Info.plist, like so:
    ///
    /// <key>CFBundleDocumentTypes</key>
    /// <array>
    ///   <dict>
    ///     <key>CFBundleTypeRole</key>
    ///     <string>Editor</string>
    ///     <key>CFBundleTypeName</key>
    ///     <string>TTcards</string>
    ///     <key>CFBundleTypeExtensions</key>
    ///     <array>
    ///         <string>json</string>
    ///     </array>
    ///     <key>CFBundleTypeIconFile</key>
    ///     <string>TTcards</string>
    ///   </dict>
    /// </array>
    ///
    /// \note The SDL2 documentation stated for CFBundleMIMETypes did not work
    /// for me.
    virtual void on_drag_drop( const Event& ev );

    /// \brief The text input event.
    virtual void on_text_input( const Event& ev );

    /// \brief Text editing event.
    ///
    /// \todo Finish implementation.
    virtual void on_text_edit( const Event& ev );

    /// \brief The event handler for user-defined events.
    virtual void on_user_event( const Event& ev );

    /// \brief Remove the top event from the high-level events queue.
    bool pop_event( Event& ev );

    /// \brief Insert an event into the high-level events queue.
    ///
    /// \remarks This can be used to simulate input events; i.e.: key press,
    /// mouse click, ...
    void push_event( const Event& ev );

  private:
    /// \brief Enumerate the available events from the underlying events
    /// subsystem (SDL_Event).
    ///
    /// \remarks This is where the wrapping of SDL_Event(s) onto our high-level
    /// events queue happens.
    void process_event( const SDL_Event* ev );

    /// \brief Enumerate the available events from the underlying events
    /// subsystem (SDL_Event).
    void process_events( void );

    /// \brief The high-level queue of available events.
    ///
    ///
    /// \remarks Decouple our high-level events handling system from this class?
    ///
    /// \note This queue wraps the underlying events subsystem (SDL_Event) and
    /// should always contain the same top event as a polled SDL_Event union
    /// does, less and except any events omitted from processing within the
    /// nom::EventHandler::process_event method.
    std::queue<Event> events_;

    Joystick joystick;
};

} // namespace nom

#endif // include guard defined

/// \class nom::EventHandler
/// \ingroup system
///
///         [DESCRIPTION STUB]
///
/// See also
///
///   nom::SDLApp,
///   examples/app/app.cpp,
///   Resources/SharedSupport/ExamplesTemplate.cpp
///
/// References
///
/// 1. http://www.sdltutorials.com/sdl-app-states
///
/// \todo Relocate the debugging preprocessors & code to the events example.
///
