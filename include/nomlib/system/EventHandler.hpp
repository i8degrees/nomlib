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
#include <memory>
#include <map>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/system/SDL_helpers.hpp"
#include "nomlib/system/events/Event.hpp"

/// \brief Enable debugging output of quit (end program execution) events.
// #define NOM_DEBUG_SDL2_QUIT_EVENT

/// \brief Enable debugging output of user defined events.
// #define NOM_DEBUG_SDL2_USER_EVENT

/// \brief Enable debugging output of window events.
// #define NOM_DEBUG_SDL2_WINDOW_INPUT

/// \brief Enable debugging output of window focus events.
// #define NOM_DEBUG_SDL2_WINDOW_FOCUS_INPUT

/// \brief Enable debugging output of key press & release events.
// #define NOM_DEBUG_SDL2_KEYBOARD_INPUT

/// \brief Enable debugging output of mouse click & release events.
// #define NOM_DEBUG_SDL2_MOUSE_INPUT

/// \brief Enable debugging output of mouse motion events.
// #define NOM_DEBUG_SDL2_MOUSE_MOTION_INPUT

/// \brief Enable debugging output of joystick button events.
// #define NOM_DEBUG_SDL2_JOYSTICK_BUTTON_INPUT

/// \brief Enable debugging output of joystick axis events.
// #define NOM_DEBUG_SDL2_JOYSTICK_AXIS_INPUT

/// \brief Enable debugging output of joystick addition & removal events.
#define NOM_DEBUG_SDL2_JOYSTICK_EVENT

/// \brief Enable debugging output of touch motion events.
// #define NOM_DEBUG_SDL2_TOUCH_MOTION_EVENT

/// \brief Enable debugging output of finger touch events.
// #define NOM_DEBUG_SDL2_TOUCH_EVENT

/// \brief Enable debugging output of multiple finger gesture events.
// #define NOM_DEBUG_SDL2_GESTURE_EVENT

/// \brief Enable debugging output of Drag 'N' Drop events.
#define NOM_DEBUG_SDL2_DRAG_DROP_INPUT

/// \brief Enable debugging output of text input events.
// #define NOM_DEBUG_SDL2_TEXT_INPUT_EVENT

/// \brief Enable debugging output of text editing events.
// #define NOM_DEBUG_SDL2_TEXT_EDIT_EVENT

namespace nom {

/// \brief SDL mapping to individual PS3 axes.
enum PSXAXES
{
  LEFTX = 0,
  LEFTY = 1,
  RIGHTX = 2,
  RIGHTY = 3
};

/// \brief SDL mapping to individual PS3 controller buttons.
enum PSXBUTTON
{
  SELECT            = 0,
  JOYCLICKLEFT      = 1,
  JOYCLICKRIGHT     = 2,
  START             = 3,
  UP                = 4,
  RIGHT             = 5,
  DOWN              = 6,
  LEFT              = 7,
  L2                = 8,
  R2                = 9,
  L1                = 10,
  R1                = 11,
  TRIANGLE          = 12,
  CIRCLE            = 13,
  CROSS             = 14,
  SQUARE            = 15,
  PSBUTTON          = 16
};

/// \brief High-level events handling.
class EventHandler
{
  public:
    typedef std::unique_ptr<SDL_Joystick, void (*)(SDL_Joystick*)> JoystickUniquePtr;

    /// \brief Default constructor.
    ///
    /// \remarks The joystick subsystem is initialized here.
    EventHandler( void );

    /// \brief Destructor.
    ///
    /// \remarks The joystick subsystem is shutdown within nom::SDLApp.
    virtual ~EventHandler( void );

    /// \brief The main events handler (think: processor).
    ///
    /// \returns Boolean TRUE when pending events are available; boolean FALSE
    /// when no events are pending.
    virtual bool on_input( SDL_Event* event );

    /// \brief Query pending events.
    virtual bool poll_event( SDL_Event* ev );

  protected:
    /// \brief The event handler for user-defined events.
    virtual void on_user_event( const UserEvent& ev );

    /// \brief The application level event generated by either the user or API
    /// requesting that the program execution be shutdown.
    ///
    /// \NOTE This event is necessary for Command + Q functionality under
    /// Mac OS X.
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
    /// \remarks This event is always preceded by SDL_WINDOWEVENT_SIZE_CHANGED.
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
    /// \remarks This event is followed by SDL_WINDOWEVENT_RESIZED if the size
    /// was changed by an external event, such as the user or the window
    /// manager.
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
    /// \NOTE ev.window.event = 14
    virtual void on_window_close( const Event& ev );

    /// \brief The keyboard event generated by user input.
    ///
    /// \NOTE This method implements the standard key binding for closing a
    /// window when compiled for Mac OS X. You will need to make a call to this
    /// method -- Input::on_key_down( ev ) -- in your own re-implementation of
    /// the on_key_down method if you wish to maintain this functionality.
    virtual void on_key_down( const Event& ev );
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
    /// \remark x > 0 = Left
    /// \remark x < 0 = Right
    /// \remark y > 0 = Up
    /// \remark y < 0 = Down
    ///
    /// \note Coordinates values X & Y are typically inverted; software -- such
    /// as the Mouse Preference Pane under OS X v10.7+ -- allow you to reverse
    /// this while using the Apple Mighty Mouse.
    virtual void on_mouse_wheel( const Event& ev );

    virtual void on_joy_axis( const Event& ev );

    virtual void on_joy_button_down( const Event& ev );
    virtual void on_joy_button_up( const Event& ev );

    virtual void on_joystick_add( void );
    virtual void on_joystick_remove( void );

    virtual void on_touch_motion( const Event& ev );
    virtual void on_touch_down( const Event& ev );
    virtual void on_touch_up( const Event& ev );

    /// \brief The finger touch event generated when a multiple finger touch
    /// gesture event is received.
    virtual void on_gesture( const Event& ev );

    /// \brief The text input event.
    virtual void on_text_input( const Event& ev );

    /// \brief Text editing event.
    ///
    /// \FIXME Finish implementation.
    virtual void on_text_edit( const Event& ev );

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

  private:
    JoystickUniquePtr joystick_;
    SDL_JoystickID joystick_id_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::Input
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
/// \TODO Implement SDL2's GameController API for Joysticks.
