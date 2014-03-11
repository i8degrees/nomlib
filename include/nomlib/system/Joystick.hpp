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
#ifndef NOMLIB_SDL2_SYSTEM_JOYSTICK_HPP
#define NOMLIB_SDL2_SYSTEM_JOYSTICK_HPP

#include <string>
#include <cstring>
#include <memory>
#include <vector>
#include <map>

#include "nomlib/config.hpp"
#include "nomlib/system/IJoystick.hpp"
#include "nomlib/system/SDLJoystick.hpp"

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

/// \brief High-level joystick handling API.
class Joystick
{
  public:
    typedef Joystick SelfType;
    typedef int JoystickID;

    typedef std::pair<JoystickID, std::string> Pair;
    typedef std::vector<std::string> JoystickNames;
    typedef std::map<JoystickID, std::string> Joysticks;

    /// \brief Default constructor.
    Joystick( void );

    /// \brief Destructor.
    ///
    /// \remarks Close opened joystick devices and shutdown the joystick
    /// subsystem.
    ///
    /// \note ~~The joystick subsystem shutdown is presently handled in the
    /// destruction of SDLApp~~.
    ~Joystick( void );

    /// \brief Initialize the underlying joystick subsystem.
    bool initialize( void );

    /// \brief Shutdown the joystick subsystem.
    void shutdown( void );

    /// \brief Obtain the first joystick device identifier as recognized by the
    /// underlying implementation.
    JoystickID first_joystick( void ) const;

    /// \brief Obtain the last joystick device identifier as recognized by the
    /// underlying implementation.
    JoystickID last_joystick( void ) const;

    /// \brief Obtain the number of joystick devices as recognized by the
    /// underlying implementation.
    int num_joysticks( void ) const;

    const std::string& name( JoystickID idx );

    const JoystickNames names( void ) const;

    void enumerate_devices( void );

  private:
    std::unique_ptr<IJoystick> impl_;
    Joysticks joysticks_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::Joystick
/// \ingroup system
///
///         [DESCRIPTION STUB]
///
