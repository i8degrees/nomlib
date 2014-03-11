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
#ifndef NOMLIB_SDL2_SYSTEM_SDL_JOYSTICK_HPP
#define NOMLIB_SDL2_SYSTEM_SDL_JOYSTICK_HPP

#include <string>
#include <cstring>
#include <memory>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/system/IJoystick.hpp"

namespace nom {

/// \brief Low-level joystick handling API.
class SDLJoystick: public IJoystick
{
  public:
    typedef SDLJoystick SelfType;
    typedef std::unique_ptr<SDL_Joystick, void (*)(SDL_Joystick*)> UniquePtr;

    /// \brief Default constructor.
    SDLJoystick( void );

    /// \brief Destructor.
    ~SDLJoystick( void );

    /// \brief Initialize the joystick subsystem.
    ///
    /// \returns Boolean TRUE when the SDL joystick subsystem is initialized
    /// and the joystick event state is enabled successfully.
    bool initialize( void );

    /// \brief Shutdown the joystick subsystem.
    void shutdown( void );

    /// \brief Obtain the number of joysticks attached to the system.
    ///
    /// \returns The number of joysticks attached on success, or a negative
    /// error code on failure.
    int num_joysticks( void ) const;

    /// \brief Obtain the status of the joystick device.
    ///
    /// \returns Boolean TRUE if the joystick has been opened, boolean FALSE
    /// otherwise.
    bool attached( void ) const;

    /// \brief Obtain the device index of an opened joystick.
    ///
    /// \returns An instance identifier of the joystick device on success, or a
    /// negative error code on failure.
    int id( void ) const;

    /// \brief Obtain the implementation dependent name of the joystick device.
    ///
    /// \returns Name of the joystick device on success, or a null-terminated
    /// string on failure (such as an invalid device state).
    const std::string name( void ) const;

    /// \brief Open a joystick device
    ///
    /// \param idx Device index of the joystick as recognized by SDL2.
    bool open( int idx );

    /// \brief Close a previously opened joystick device.
    void close( void );

    /// \brief Obtain the implementation-dependent GUID for the joystick.
    ///
    /// \param idx The joystick device index identifier as recognized by SDL2.
    ///
    /// \returns A valid GUID of the given joystick on success, if an invalid
    /// index, zero GUID.
    SDL_JoystickGUID device_guid( int idx ) const;

    /// \brief Obtain the implementation-dependent GUID for the opened joystick.
    ///
    /// \returns A valid GUID of the given joystick on success, if an invalid
    /// index, zero GUID.
    SDL_JoystickGUID device_guid( void ) const;

    const std::string device_guid_string( void ) const;

    bool game_controller( void ) const;

  private:
    /// \brief Toggle the joystick event polling state.
    ///
    /// \param state One of the following: SDL_QUERY, SDL_IGNORE or SDL_ENABLE.
    ///
    /// \returns One (1) if enabled, zero (0) if disabled or a negative error
    /// code on failure. If the state was SDL_QUERY, then the current state is
    /// returned, otherwise the new processing state is returned.
    ///
    /// \remarks If joystick events are disabled, you must call
    /// SDLJoystick::update and manually check the state of the joystick when
    /// event data is needed.
    ///
    /// \note Calling this method may delete all events currently in SDL's event
    /// queue.
    int set_event_state( int state ) const;

    /// \brief Update the current state of the opened joystick(s).
    ///
    /// \remarks This method is automatically called by the event loop if any
    /// joystick events are enabled.
    void update( void ) const;

    SDLJoystick::UniquePtr device_;
};

namespace priv {

/// Custom deleter for SDL_Joystick* structures.
void Free_Joystick( SDL_Joystick* );

} // namespace priv
} // namespace nom

#endif // include guard defined

/// \class nom::SDLJoystick
/// \ingroup system
///
///         [DESCRIPTION STUB]
///
/// \TODO Implement SDL2's GameController API for Joysticks.
///
