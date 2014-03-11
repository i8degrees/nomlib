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
#ifndef NOMLIB_SYSTEM_INPUT_MAPPER_KEYBOARD_ACTION_HPP
#define NOMLIB_SYSTEM_INPUT_MAPPER_KEYBOARD_ACTION_HPP

#include "nomlib/config.hpp"

namespace nom {

/// \brief A structure containing information on a keyboard action.
struct KeyboardAction
{
  /// \brief Default constructor; initializes the object to an invalid action
  /// state.
  KeyboardAction( void ) :
    type( 0 ),
    sym( 0 ),
    mod( KMOD_NONE ),
    window_id( 0 )
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief Destructor.
  ~KeyboardAction( void )
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief Constructor for initializing an object to a valid action state.
  KeyboardAction( uint32 type, int32 sym ) :
    type( type ),
    sym( sym ),
    mod( KMOD_NONE ),
    window_id( 0 )
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief Constructor for initializing an object to a valid action state.
  KeyboardAction( uint32 type, int32 sym, uint16 mod ) :
    type( type ),
    sym( sym ),
    mod( mod ),
    window_id( 0 )
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief Diagnostic output of the object state.
  void dump( void ) const
  {
    NOM_DUMP( type );
    NOM_DUMP( sym );
    NOM_DUMP( mod );
    // NOM_DUMP( window_id );
  }

  /// \brief The event type.
  ///
  /// \remarks SDL_KEYDOWN or SDL_KEYUP.
  uint32 type;

  /// \brief Symbol of the key press event.
  ///
  /// \TODO Implement using the KeyboardSymbol structure.
  // SDL_Keysym sym;
  int32 sym;

  /// \brief The modifiers of the key press event; KMOD_CTRL, KMOD_ALT and so
  /// on.
  ///
  /// \remarks Multiple modifier keys can be combined as a bit-mask.
  uint16 mod;

  /// \brief The identifier of the window at the moment of the event.
  ///
  /// \remarks This field is not used and is reserved for future implementation.
  uint32 window_id;
};

} // namespace nom

#endif // include guard defined
