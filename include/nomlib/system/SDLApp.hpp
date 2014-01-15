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

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/system/Input.hpp"
#include "nomlib/system/Timer.hpp"
#include "nomlib/system/StateMachine.hpp"
#include "nomlib/system/SDL_helpers.hpp"

namespace nom {

/// \brief Convenience template class for video games
class SDLApp: public Input
{
  public:
    typedef SDLApp* RawPtr;
    typedef std::shared_ptr<SDLApp> SharedPtr;

    SDLApp ( void );
    virtual ~SDLApp ( void );

    virtual bool on_init ( void );

    /// Re-implements nom::Input::onQuit()
    virtual void on_quit ( void );

    bool running ( void );
    void quit ( void );

    uint32 ticks ( void );

    bool show_fps ( void ) const;
    void set_show_fps ( bool toggle );

    /// Helper method for toggling the state of nom::SDLApp::show_fps
    ///
    /// \return State of nom::SDLApp::show_fps_ after call to nom::SDLApp::set_show_fps
    bool toggle_fps ( void );

    /// Let the user know if there are pending events
    virtual bool poll_events ( EventType* );

    /// State management
    virtual void set_state ( uint32 id, void_ptr data = nullptr );
    void set_state ( IState::UniquePtr state, void_ptr data = nullptr );
    // TODO: virtual void set_next_state( IState::UniquePtr state, uint32_ptr data = nullptr );
    void push_state ( IState::UniquePtr state, void_ptr data = nullptr );
    void pop_state ( IState::UniquePtr state, void_ptr data = nullptr );
    void pop_state ( void_ptr data = nullptr );

    virtual void on_event ( EventType* ); // TODO: rename to on_event

    /// State logic
    virtual void on_update( float );

    /// State rendering
    virtual void on_draw( IDrawable::RenderTarget );

  protected:
    /// Input events
    EventType event;

    /// State machine manager
    StateMachine states;

    //GameStates* state_factory;
  private:
    /// global app state
    bool app_state_;

    /// fps counter
    bool show_fps_;

    /// global app timer
    Timer app_timer_;
};

} // namespace nom

#endif // include guard defined
