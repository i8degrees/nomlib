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

#include <iostream>
#include <vector>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/system/IState.hpp"
#include "nomlib/graphics/IDrawable.hpp"

namespace nom {

/// \brief Finite State Machine manager class
class StateMachine
{
  public:
    typedef std::vector<IState::UniquePtr> StateStack;

    /// Default constructor
    StateMachine ( void );

    /// Destructor
    ~StateMachine ( void );

    // State management

    /// \brief Obtain the previous state's identifier
    ///
    /// \returns Identifier of the state on success; identifier number of the
    /// current state on failure (such as if there is no previous state in list).
    ///
    /// \remarks It is not required that the state has an ID.
    uint32 previous_state ( void ) const;
    void set_state ( IState::UniquePtr state, void_ptr data );
    void push_state ( IState::UniquePtr state, void_ptr data );

    void pop_state ( IState::UniquePtr state, void_ptr data );
    void pop_state ( void_ptr data );

    /// State events handling
    void process_event( const Event& ev );

    /// State logic handling
    void update ( float delta );

    /// State rendering handling
    void draw ( IDrawable::RenderTarget& );

  private:
    /// Container of our states
    StateStack states;
};

} // namespace nom

#endif // include guard defined
