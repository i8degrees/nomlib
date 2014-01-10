/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#ifndef NOMLIB_SDL2_CURSOR_HPP
#define NOMLIB_SDL2_CURSOR_HPP

#include <iostream>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/graphics/sprite/AnimatedSprite.hpp"

namespace nom {

/// \brief Helper class for loading, positioning and keeping track of a cursor
/// controlled by an user-defined input.
class Cursor:
              public AnimatedSprite
{
  public:
    /// Default construct for initializing instance variables to their
    /// respective defaults.
    Cursor ( void );

    /// Destructor
    virtual ~Cursor ( void );

    /// Construct a new Cursor object, initializing with a SpriteSheet object.
    Cursor ( const SpriteSheet& sheet );

    /// Construct a Cursor from an existing sprite sheet filename.
    Cursor ( const std::string& filename );

    /// Copy assignment operator
    Cursor& operator = ( const Cursor& other );

    /// X axis position
    ///
    /// \return The position this cursor is on in the X axis
    int32 x ( void ) const;

    /// Y axis position
    ///
    /// \return The position this cursor is on in the Y axis
    int32 y ( void ) const;

    /// Getter for obtaining locked status.
    bool locked ( void ) const;

    /// Toggle the lock variable.
    void toggle_lock ( bool toggle );

  protected:
    /// The deriving classes must implement this iteration for the 'up' action
    ///
    /// \return -1 when this base class method is left undefined
    virtual int32 move_up ( void );

    /// The deriving classes must implement this iteration for the 'down' action
    ///
    /// \return -1 when this base class method is left undefined
    virtual int32 move_down ( void );

    /// The deriving classes must implement this iteration for the 'left' action
    ///
    /// \return -1 when this base class method is left undefined
    virtual int32 move_left ( void );

    /// The deriving classes must implement this iteration for the 'right' action
    ///
    /// \return -1 when this base class method is left undefined
    virtual int32 move_right ( void );

  private:
    /// Convenience variable left to the end-user to define, if chosen.
    ///
    /// Initialized to false at object creation.
    bool locked_;
};

} // namespace nom

#endif // include guard defined
