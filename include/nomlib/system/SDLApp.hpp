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

#include <iostream>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/system/EventHandler.hpp"
#include "nomlib/system/Timer.hpp"
#include "nomlib/system/StateMachine.hpp"
#include "nomlib/system/Event.hpp"
#include "nomlib/system/ResourceCache.hpp"
#include "nomlib/system/resource_types.hpp"

namespace nom {

/// \brief Convenience template class for video games
class SDLApp: public EventHandler
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
      OSX_DISABLE_FULLSCREEN_SPACES = 0x2,

      /// Initialize the cache of font resources available to the engine.
      INIT_ENGINE_FONTS = 0x4
    };

    /// \brief Default constructor; initialize with the default set of hints.
    ///
    /// \remarks The hints OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS,
    /// OSX_DISABLE_FULLSCREEN_SPACES and INIT_ENGINE_FONTS are passed.
    SDLApp( void );

    /// \see SDLApp::Hints enumeration.
    SDLApp( uint32 flags );

    virtual ~SDLApp( void );

    bool app_state( void ) const;

    virtual bool on_init( void );

    /// \todo Rename to exec..?
    virtual sint Run( void );

    /// \brief The application-level handler for events.
    ///
    /// \remarks This method is called once every frame from within the main
    /// loop.
    virtual void on_event( const Event& ev );

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
    virtual void on_draw( IDrawable::RenderTarget& );

    /// \brief Implements the nom::EventHandler::on_window_close method.
    ///
    /// \remarks The default implementation is to let the SDLApp::on_app_quit
    /// method handle things for us.
    virtual void on_window_close( const Event& ev );

    /// \brief Implements the nom::EventHandler::on_app_quit method.
    ///
    /// \remarks The default implementation is to let the SDLApp::quit method
    /// handle things for us.
    virtual void on_app_quit( const Event& ev );

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

    // State management

    virtual uint32 previous_state ( void ) const;
    virtual void set_state ( uint32 id, void_ptr data = nullptr );
    virtual void set_state ( IState::UniquePtr state, void_ptr data = nullptr );
    // TODO: virtual void set_next_state( IState::UniquePtr state, uint32_ptr data = nullptr );
    virtual void push_state ( IState::UniquePtr state, void_ptr data = nullptr );
    virtual void pop_state ( IState::UniquePtr state, void_ptr data = nullptr );
    virtual void pop_state ( void_ptr data = nullptr );

    /// \brief Get the cache of available fonts.
    ///
    /// \remarks The available fonts are determined at run-time within the
    /// initialization of the class.
    FontCache& fonts( void );

  protected:
    /// \brief State machine manager.
    StateMachine states;

    //GameStates* state_factory;

  private:
    bool initialize( uint32 flags );

    /// \brief Global application state.
    bool app_state_;

    /// \brief FPS counter.
    bool show_fps_;

    /// \brief Global application timer.
    Timer app_timer_;

    /// \note A nom::RenderWindow must be initialized before the resource
    /// loading method call can be used -- nom::FontCache::load_resource.
    FontCache fonts_;
};

} // namespace nom

#endif // include guard defined
