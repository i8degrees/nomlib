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
#ifndef NOMLIB_SYSTEM_SDLAPP_HPP
#define NOMLIB_SYSTEM_SDLAPP_HPP

#include <string>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/system/Timer.hpp"

namespace nom {

// Forward declarations
struct Event;
class EventHandler;
class RenderWindow;
class IState;
class StateMachine;

/// \brief Convenience template class for video games
///
/// \note http://docs.wxwidgets.org/trunk/classwx_app.html
/// \note http://doc.qt.digia.com/4.6/qapplication.html
class SDLApp
{
  public:
    typedef SDLApp self_type;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief An enumeration of platform or application scope features
    /// available for explicit request of enabling or disabling.
    enum Hints
    {
      /// SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS
      OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS = 0x1,

      /// SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES
      OSX_DISABLE_FULLSCREEN_SPACES = 0x2
    };

    /// \brief Default constructor; initialize with the default set of hints.
    ///
    /// \remarks The hints OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS and
    /// OSX_DISABLE_FULLSCREEN_SPACES are passed.
    SDLApp( void );

    /// \see SDLApp::Hints enumeration.
    SDLApp( uint32 flags );

    virtual ~SDLApp( void );

    bool app_state( void ) const;

    virtual bool on_init( void );

    /// \todo Rename to exec..?
    virtual sint Run( void );

    /// \brief The application-level handler for logic.
    ///
    /// \remarks This method is called once every frame from within the main
    /// loop.
    virtual void on_update( float );

    /// \brief The application-level handler for rendering.
    ///
    /// \remarks This method is called once every frame from within the main
    /// loop.
    ///
    /// \todo Consider removing RenderTarget argument; I *think* we can get
    /// away with this!
    virtual void on_draw( RenderWindow& );

    /// \brief Query status of the application state.
    ///
    /// \returns Boolean true or false.
    ///
    /// \remarks The default handling of the application state is to end program
    /// execution upon a false result from this method.
    virtual bool running( void );

    /// \brief Obtain the current timestamp.
    ///
    /// \returns Number of ticks from the start of class construction.
    uint32 ticks ( void );

    /// \brief End program execution.
    ///
    /// \note Sets the internal class variable app_state_ to false.
    virtual void quit( void );

    void set_app_state( bool state );

    bool show_fps ( void ) const;
    void set_show_fps ( bool toggle );

    /// Helper method for toggling the state of nom::SDLApp::show_fps
    ///
    /// \return State of nom::SDLApp::show_fps_ after call to nom::SDLApp::set_show_fps
    bool toggle_fps ( void );

    StateMachine* state( void ) const;

    void set_state_machine( StateMachine* mech );

    /// \brief Install the event handler used by the interface.
    ///
    /// \remarks The application's events will not be processed until a call is
    /// made to the event handler's ::poll_event method.
    ///
    /// \note The installed event handler must outlive the destruction of
    /// this interface!
    void set_event_handler(EventHandler& evt_handler);

  protected:
    /// \brief Default event handler for input events.
    virtual void on_input_event(const Event& ev);

    /// \brief The event handler for when a request for halting program
    /// execution occurs.
    ///
    /// \remarks The default implementation calls nom::SDLApp::quit.
    ///
    /// \note This event handles the system's default quit handler, i.e.:
    /// COMMAND + Q on Mac OS X and ALT + F4 on Windows.
    virtual void on_app_quit(const Event& ev);

    virtual void on_window_shown(const Event& ev);

    virtual void on_window_hidden(const Event& ev);

    virtual void on_window_exposed(const Event& ev);

    virtual void on_window_moved(const Event& ev);

    virtual void on_window_resized(const Event& ev);

    virtual void on_window_size_changed(const Event& ev);

    virtual void on_window_minimized(const Event& ev);

    virtual void on_window_maximized(const Event& ev);

    virtual void on_window_restored(const Event& ev);

    virtual void on_window_mouse_focus(const Event& ev);

    virtual void on_window_mouse_focus_lost(const Event& ev);

    virtual void on_window_keyboard_focus(const Event& ev);

    virtual void on_window_keyboard_focus_lost(const Event& ev);

    /// \brief The event handler for when a request for closing a window
    /// occurs.
    ///
    /// \remarks The default implementation calls nom::SDLApp::on_app_quit.
    virtual void on_window_close(const Event& ev);

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
    virtual void on_drag_drop(const Event& ev);

    /// \remarks This is applicable only to Direct3D renderers.
    virtual void on_render_targets_reset(const Event& ev);

    /// \remarks This is applicable only to Direct3D renderers.

// NOTE: Not available until the release of SDL 2.0.4
#if 0
    virtual void on_render_device_reset(const Event& ev);
#endif

    // NOTE: Proposed naming scheme for iOS / Android event handlers
    // virtual void on_app_destroy(const Event& ev);
    // virtual void on_app_low_memory(const Event& ev);
    // virtual void on_app_enter_background(const Event& ev);
    // virtual void on_app_background(const Event& ev);
    // virtual void on_app_enter_foreground(const Event& ev);
    // virtual void on_app_foreground(const Event& ev);

    /// \brief The default event handler for user-defined events.
    virtual void on_user_event(const Event& ev);

  private:
    bool initialize( uint32 flags );

    void on_app_event(const Event& ev);

    void process_event(const Event& ev);

    // Non-owned pointer
    EventHandler* event_handler_ = nullptr;

    /// \brief State machine manager.
    ///
    /// \remarks This object pointer must be initialized prior to use.
    ///
    /// \see SDLApp::set_state_machine
    std::unique_ptr<StateMachine> state_;

    /// \brief Global application state.
    bool app_state_;

    /// \brief FPS counter.
    bool show_fps_;

    /// \brief Global application timer.
    Timer app_timer_;
};

} // namespace nom

#endif // include guard defined
