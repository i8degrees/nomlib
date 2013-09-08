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
#include "nomlib/system/GameStates.hpp"

namespace nom {

// Initialize our static vars
std::vector<std::unique_ptr<IState>> GameStates::states;

// Default constructor; something is terribly amiss if you ever see this
// initialized!
GameStates::GameStates ( void )
{
NOM_LOG_TRACE ( NOM );
}

void GameStates::onEvent ( void* event )
{
  // let the state handle events
  states.back()->HandleInput ( event );
}

void GameStates::Update ( float delta_time )
{
  // let the state update the scene with regard to the delta (change) in timing
  states.back()->Update ( delta_time );
}

void GameStates::Draw ( void* video_buffer )
{
  // let the state draw the scene onto the display buffer
  states.back()->Draw ( video_buffer );
}

void GameStates::ChangeState ( std::unique_ptr<IState> state )
{
  // cleanup the current state
  if ( ! states.empty() )
  {
    states.back()->onExit();
    states.pop_back();
  }

NOM_ASSERT ( state );

  // store the new state
  states.push_back( std::move( state ) );

  states.back()->onInit();
}

void GameStates::PushState ( std::unique_ptr<IState> state )
{
NOM_ASSERT ( state );

  // pause current state
  if ( ! states.empty() )
  {
    states.back()->Pause();
  }

  // store the new state
  states.push_back( std::move( state ) );

  states.back()->onInit();
}

void GameStates::PopState ( void )
{
  // cleanup the current state
  if ( ! states.empty() )
  {
    states.pop_back();
  }

  // resume previous state
  states.back()->Resume();
}

void GameStates::PopStateThenChangeState ( std::unique_ptr<IState> state )
void GameStates::PopState ( int32 response )
{
  // cleanup the current state
  if ( ! states.empty() )
  {
    states.pop_back();
  }

  // resume previous state
  states.back()->Resume( response );
}

{
  // cleanup the current state
  if ( ! states.empty() )
  {
    states.pop_back();
  }

NOM_ASSERT ( state );

  //if ( ! states.empty () )
  GameStates::ChangeState( std::move( state ) );
}


} // namespace nom
