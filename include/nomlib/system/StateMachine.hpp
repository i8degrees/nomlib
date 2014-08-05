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
#ifndef NOMLIB_SYSTEM_STATE_MACHINE_HPP
#define NOMLIB_SYSTEM_STATE_MACHINE_HPP

#include <memory>
#include <vector>
#include <queue>

#include "nomlib/config.hpp"

namespace nom {

// Forward declarations
struct Event;
class IState;
class RenderWindow;

namespace priv {

/// \internal
/// \see nom::StateMachine::set_deferred_state
/// \endinternal
struct DeferredState
{
  bool deferred;
  std::unique_ptr<IState> state;
  void* data;
};

} // namespace priv

/// \brief Finite State Machine manager class
class StateMachine
{
  public:
    /// Default constructor
    StateMachine();

    /// Destructor
    ~StateMachine();

    // virtual void set_state ( uint32 id, void_ptr data = nullptr );

    /// \brief Get the number of active states.
    ///
    /// \returns Total number of states in the stack.
    uint32 count() const;

    /// \brief Obtain the current state's identifier
    ///
    /// \returns The identifier of the state, or an assertion on
    /// failure, such as if the the states stack is empty.
    ///
    /// \remarks The state identifier is user-defined.
    uint32 current_state_id( void ) const;

    /// \brief Obtain the next state's identifier
    ///
    /// \returns The identifier of the next state (in the stack), or an
    /// assertion on failure, such as if the the states stack is empty.
    ///
    /// \remarks The state identifier is user-defined.
    uint32 next_state_id( void ) const;

    /// \param state  The nom::IState deriving object to be set as the current
    /// state.
    /// \param data   User-defined data; used for passing data in between
    /// states.
    ///
    /// \see StateMachine::set_deferred_state
    void set_state( std::unique_ptr<IState> state, void_ptr data );

    /// \param state  The nom::IState deriving object to be set as the current
    /// state.
    /// \param data   User-defined data; used for passing data in between
    /// states.
    ///
    /// \see StateMachine::set_deferred_state
    void pop_state( void_ptr data );

    /// State events handling
    void on_event( const Event& ev );

    /// \brief State logic handling
    ///
    /// \remarks Deferred state transition logic is handled here.
    void update( float delta );

    /// State rendering handling
    void draw( RenderWindow& );

  private:
    /// \brief Set the next state.
    ///
    /// \param state  The state to be put onto the stack. If NULL is passed,
    /// the current state exits and the next one after that is resumed.
    ///
    /// \note Where on the stack the state is put is dependent upon the type of
    /// state defined (see also: nom::IState::Type enumeration).
    ///
    /// \remarks The transition of the state must be deferred until the next
    /// frame update in order for engine callback systems, i.e.:
    /// nom::InputMapper and nom::UIWidget, to be used within application-level
    /// code that expects to be able to change its state from within said
    /// callback system. If not deferred, the result is invalid memory access
    /// (a crash if you are lucky!) due to the deletion of the state before the
    /// callback is completed.
    void set_deferred_state( std::unique_ptr<IState> state, void_ptr data );

    /// \brief The pending state to be set on the next frame update
    priv::DeferredState next_state_;

    /// \brief Stack of states
    std::vector<std::unique_ptr<IState>> states_;
};

} // namespace nom

#endif // include guard defined
