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
#include "nomlib/system/StateMachine.hpp"

namespace nom {

StateMachine::StateMachine ( void )
{
  //NOM_LOG_TRACE( NOM );
}

StateMachine::~StateMachine ( void )
{
  //NOM_LOG_TRACE( NOM );
}

uint32 StateMachine::previous_state ( void ) const
{
  // Verify first that we have a previous state in the list
  if ( this->states.size() > 1 )
  {
    return this->states.front()->id();
  }

  return this->states.back()->id();
}

void StateMachine::set_state ( IState::UniquePtr state, void_ptr data )
{
  NOM_ASSERT( state );

  // Cleanup the current state
  if ( ! this->states.empty() )
  {
    this->states.back()->on_exit(data);
    this->states.pop_back();
  }

  // Store the new state
  this->states.push_back( std::move( state ) );

  this->states.back()->on_init(data);
}

void StateMachine::push_state ( IState::UniquePtr state, void_ptr data )
{
  NOM_ASSERT( state );

  // Pause current state
  if ( ! this->states.empty() )
  {
    this->states.back()->on_pause(data);
  }

  // Store the new state
  this->states.push_back( std::move( state ) );

  this->states.back()->on_init(data);
}

void StateMachine::pop_state ( IState::UniquePtr state, void_ptr data )
{
  NOM_ASSERT( state );

  // Cleanup the current state
  if ( ! this->states.empty() )
  {
    this->states.back()->on_exit(data);
    this->states.pop_back();
  }

  this->set_state( std::move( state ), data );
}

void StateMachine::pop_state ( void_ptr data )
{
  // Cleanup the current state only if our state stack holds more than one;
  // otherwise we are left in a stateless machine.
  if ( this->states.size() > 1 )
  {
    this->states.pop_back();
  }
  else
  {
    NOM_LOG_ERR( NOM, "Could not pop state: we are the only state in existence." );
    return;
  }

  NOM_ASSERT( this->states.back() );

  // Resume previous state
  this->states.back()->on_resume( data );
}

void StateMachine::event( EventType* event )
{
  // Ensure that we have a state in which we can handle events on
  if ( ! this->states.empty() )
  {
    this->states.back()->HandleInput( event );
  }
}

void StateMachine::update ( float delta )
{
  // Ensure that we have a state in which we can handle update on
  if ( ! this->states.empty() )
  {
    // Honor user's request to update the state behind us first.
    if ( this->states.back()->flags() & IState::StateFlags::BackUpdate )
    {
      NOM_ASSERT ( this->states.front() );

      this->states.front()->on_update ( delta );
    }

    // Let the state update the scene with regard to the delta time
    this->states.back()->on_update ( delta );
  }
}

void StateMachine::draw ( IDrawable::RenderTarget target )
{
  // Ensure that we have a state in which we can handle rendering on
  if ( ! this->states.empty() )
  {
    // Honor user's request to draw the state behind us first; this allows the
    // current state to use the previous state's rendering as a background.
    if ( this->states.back()->flags() & IState::StateFlags::BackRender )
    {
      NOM_ASSERT ( this->states.front() );

      this->states.front()->on_draw ( target );
    }

    // Let the state draw the scene onto the display
    this->states.back()->on_draw ( target );
  }
}

} // namespace nom
