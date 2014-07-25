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
#ifndef NOMLIB_SYSTEM_INPUT_MAPPER_INPUT_STATE_MAPPER_HPP
#define NOMLIB_SYSTEM_INPUT_MAPPER_INPUT_STATE_MAPPER_HPP

#include <map>

#include "nomlib/config.hpp"
#include "nomlib/system/Event.hpp"
#include "nomlib/system/InputMapper/InputActionMapper.hpp"

namespace nom {

struct InputActionState
{
  bool active;
  InputActionMapper::ActionMap actions;
};

/// \brief High-level API for collections of input action maps.
///
/// \remarks See also, nom::InputActionMapper, nom::InputAction.
class InputStateMapper
{
  public:
    typedef InputStateMapper SelfType;
    typedef std::pair<std::string, InputActionState> Pair;
    typedef std::map<std::string, InputActionState> InputStateMap;

    /// \brief Default constructor.
    InputStateMapper( void );

    /// \brief Destructor.
    ~InputStateMapper( void );

    /// \brief Obtain active state status.
    ///
    /// \param key The state string index to query.
    bool active( const std::string& key ) const;

    /// \brief Add action mappings to an input state.
    ///
    /// \param key The state string index associated with the input action
    /// container.
    ///
    /// \param map A nom::InputActionMapper object with your input action maps
    /// to be assigned to a state.
    ///
    /// \param active Toggle the active state of the input mappings.
    bool insert( const std::string& key, const InputActionMapper& map, bool active );

    /// \brief Remove a state.
    ///
    /// \param key The state string index to remove.
    bool erase( const std::string& key );

    /// \brief Activate a state.
    ///
    /// \param key The state string index to activate.
    bool activate( const std::string& key );

    /// \brief Disable a state.
    ///
    /// \param key The state string index to disable.
    bool disable( const std::string& key );

    /// \brief Disable all input states.
    void disable( void );

    /// \brief Enable only a particular state.
    ///
    /// \remarks All other states, less and except specified state, will be
    /// marked inactive.
    bool activate_only( const std::string& key );

    /// \brief Empty the container of input states.
    void clear( void );

    /// \brief Diagnostic output.
    ///
    /// remarks Dumps only the active states.
    void dump( void );

    /// \brief Event handler for triggering mapped action states.
    ///
    /// \remarks This method must be located within the main input loop.
    void on_event( const Event& ev );

  private:
    /// \brief Internal event handler for matching a keyboard action to a
    /// keyboard event.
    bool on_key_press( const InputAction& mapping, const Event& ev );

    /// \brief Internal event handler for matching a mouse button action to a
    /// mouse button event.
    bool on_mouse_button( const InputAction& mapping, const Event& ev );

    /// \brief Internal event handler for matching a mouse wheel action to a
    /// mouse wheel event.
    bool on_mouse_wheel( const InputAction& mapping, const Event& ev );

    /// \brief Internal event handler for matching a joystick button action to
    /// a joystick button event.
    bool on_joystick_button( const InputAction& mapping, const Event& ev );

    /// \brief Internal event handler for matching a joystick axis action to
    /// a joystick axis event.
    ///
    /// \todo Do not use; the implementation is incomplete!
    bool on_joystick_axis( const InputAction& mapping, const Event& ev );

    InputStateMap states_;
};

} // namespace nom

#endif // include guard defined
