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
#ifndef NOMLIB_GAMESTATES_HEADERS
#define NOMLIB_GAMESTATES_HEADERS

#include <iostream>
#include <vector>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/system/IState.hpp" // abstract class for our interface

namespace nom {

// Rename to FSM, States or similar?
/// \brief (FSM) States management
class GameStates
{
  public:
    /// State events handling
    static void onEvent ( void* );

    /// State logic
    static void Update ( float delta_time );

    /// State rendering
    static void Draw ( void* video_buffer );

    /// State management
    static void ChangeState ( std::unique_ptr<IState> state );
    static void PushState ( std::unique_ptr<IState> state );
    static void PopState ( void );
    static void PopStateThenChangeState ( std::unique_ptr<IState> state );
    static void PopState ( int32 response );

    /// States stack; we hold pointer references to IState inherited classes
    static std::vector<std::unique_ptr<IState>> states;
  private:
    GameStates ( void );
};


} // namespace nom

#endif // NOMLIB_GAMESTATES_HEADERS defined
