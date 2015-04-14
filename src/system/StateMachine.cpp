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

// Forward declarations
#include "nomlib/system/IState.hpp"
#include "nomlib/system/Event.hpp"
#include "nomlib/graphics/RenderWindow.hpp"

namespace nom {

StateMachine::StateMachine()
{
  //NOM_LOG_TRACE( NOM );
}

StateMachine::~StateMachine()
{
  //NOM_LOG_TRACE( NOM );
}

// void SDLApp::set_state ( uint32 id, void_ptr data )
// {
// /*
//   std::unique_ptr<IState> state = this->state_factory->state( id );

//   this->state()->set_state( std::move(state), data );
//   if ( state != nullptr )
//   {
//     this->state()->set_state( std::move(state), data );
//   }
// */
// }

uint32 StateMachine::count() const
{
  return this->states_.size();
}

uint32 StateMachine::current_state_id( void ) const
{
  NOM_ASSERT( this->states_.empty() == false );

  return this->states_.back()->id();
}

uint32 StateMachine::next_state_id( void ) const
{
  NOM_ASSERT( this->states_.empty() == false );

  return this->states_.front()->id();
}

void StateMachine::set_state( std::unique_ptr<IState> state, void_ptr data )
{
  NOM_ASSERT( state );

  // Cleanup the current state
  // if ( ! this->states_.empty() )
  {
    // this->states_.back()->on_exit(data);
    // this->states_.pop_back();
  }

  // Store the new state
  // this->states_.push_back( std::move( state ) );
  // this->states_.back()->on_init(data);

  this->set_deferred_state( std::move( state ), data );
}

// void StateMachine::push_state( std::unique_ptr<IState> state, void_ptr data )
// {
//   NOM_ASSERT( state );

//   // Pause current state
//   if ( ! this->states_.empty() )
//   {
//     // this->states_.back()->on_pause(data);
//   }

//   this->set_deferred_state( std::move( state ), data );

//   // Store the new state
//   // this->states_.push_back( std::move( state ) );
//   // this->states_.back()->on_init(data);
// }

void StateMachine::pop_state( void_ptr data )
{
  // Cleanup the current state only if our state stack holds more than one;
  // otherwise we are left in a stateless machine.
  if( this->states_.size() > 1 )
  {
    // this->states_.back()->on_exit(data);
    // this->states_.pop_back();

    this->set_deferred_state( nullptr, data );
  }
  else
  {
    NOM_LOG_ERR( NOM, "Could not pop state: we are the only state in existence." );
    return;
  }

  // NOM_ASSERT( this->states_.back() );

  // Resume previous state
  // this->states_.back()->on_resume( data );
}

void StateMachine::on_event(const Event& ev)
{
  // Ensure that we have a state in which we can handle events on
  if( this->states_.empty() == false ) {
    this->states_.back()->on_event(ev);
  }
}

void StateMachine::update( float delta )
{
  // Ensure that we have a state in which we can handle update on
  if ( ! this->states_.empty() )
  {
    // Honor user's request to update the state behind us first.
    if ( this->states_.back()->flags() & IState::Flags::BackUpdate )
    {
      NOM_ASSERT ( this->states_.front() );

      this->states_.front()->on_update ( delta );
    }

    // Let the state update the scene with regard to the delta time
    this->states_.back()->on_update ( delta );
  }

  // Set next state, if one is in the queue. We must handle the state transition
  // in a particular way, due to the fact that we allow more than one state to
  // be active within the loop.
  if( this->next_state_.deferred == true )
  {
    if( this->next_state_.state != nullptr )
    {
      // Normal state transition
      if( this->next_state_.state->type() == IState::Type::Parent )
      {
        NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "Parent state" );
        NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "State ID:", this->next_state_.state->id() );

        if( ! this->states_.empty() )
        {
          this->states_.back()->on_exit( this->next_state_.data );
          this->states_.pop_back();
        }

        this->states_.push_back( std::move( this->next_state_.state ) );
        this->states_.back()->on_init( this->next_state_.data );

        // Next state is now in existence; no longer a pending state
        this->next_state_.deferred = false;
        this->next_state_.state.reset( nullptr );
        this->next_state_.data = nullptr;
      }
      // Special needs state transition case #1
      else if( this->next_state_.state && this->next_state_.state->type() == IState::Type::Child )
      {
        NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "Child state" );
        NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "State ID:", this->next_state_.state->id() );

        if( ! this->states_.empty() )
        {
          this->states_.back()->on_pause( this->next_state_.data );
        }

        this->states_.push_back( std::move( this->next_state_.state ) );
        this->states_.back()->on_init( this->next_state_.data );

        // Next state is now in existence; no longer a pending state
        this->next_state_.deferred = false;
        this->next_state_.state.reset( nullptr );
        this->next_state_.data = nullptr;
      }
    } // end if next_state is not NULL

    // Special needs state transition case #2 (resume previous state)
    else // if next state is NULL
    {
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "Resume previous (parent or child) state" );
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "State ID:", this->current_state_id() );

      if( this->states_.size() > 1 )
      {
        this->states_.back()->on_exit( this->next_state_.data );
        this->states_.pop_back();
        this->states_.back()->on_resume( this->next_state_.data );
      }
    }
  } // end if next_state is deferred
}

void StateMachine::draw( RenderWindow& target )
{
  // Ensure that we have a state in which we can handle rendering on
  if ( ! this->states_.empty() )
  {
    // Honor user's request to draw the state behind us first; this allows the
    // current state to use the previous state's rendering as a background.
    if ( this->states_.back()->flags() & IState::Flags::BackRender )
    {
      NOM_ASSERT ( this->states_.front() );

      this->states_.front()->on_draw ( target );
    }

    // Let the state draw the scene onto the display
    this->states_.back()->on_draw ( target );
  }
}

// Private scope

void StateMachine::set_deferred_state (
                                        std::unique_ptr<IState> state,
                                        void_ptr data
                                      )
{
  this->next_state_.deferred = true;
  this->next_state_.state = std::move( state );
  this->next_state_.data = data;
}

} // namespace nom
