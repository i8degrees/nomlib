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
#include "nomlib/system/InputMapper/InputStateMapper.hpp"
#include "nomlib/graphics/RenderWindow.hpp"

namespace nom {

// TransitionTable implementation

void TransitionTable::add( StateSymbol active, EventSymbol event,
                             StateSymbol stack_top, const Transition& transition )
{
  this->transitions_[std::make_tuple( active, event, stack_top )] = transition;
}

void TransitionTable::add( StateSymbol active, EventSymbol event,
                             const Transition& transition )
{
  this->wildcards_[std::make_pair( active, event )] = transition;
}

Transition TransitionTable::query( StateSymbol active, EventSymbol event,
                                      StateSymbol stack_top ) const
{
  auto exact = this->transitions_.find( std::make_tuple( active, event, stack_top ) );
  if( exact != this->transitions_.end() ) {
    return exact->second;
  }

  auto wildcard = this->wildcards_.find( std::make_pair( active, event ) );
  if( wildcard != this->wildcards_.end() ) {
    return wildcard->second;
  }

  return Transition {};
}

void TransitionTable::set_fallback( Callback callback )
{
  this->fallback_ = std::move( callback );
}

Transition TransitionTable::resolve( const IState& active, EventSymbol event,
                                     StateSymbol stack_top ) const
{
  Transition transition = this->query( active.symbol(), event, stack_top );
  if( transition.action == TransitionAction::None && this->fallback_ ) {
    transition = this->fallback_( active, event, stack_top );
  }
  return transition;
}

// StateMachine implementation

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

bool StateMachine::empty() const
{
  return this->states_.empty();
}

void StateMachine::set_input_mapper( InputStateMapper* mapper )
{
  this->input_mapper_ = mapper;
}

InputStateMapper* StateMachine::input_mapper() const
{
  return this->input_mapper_;
}

void StateMachine::activate_input_context( const IState& state )
{
  if( this->input_mapper_ == nullptr ) {
    return;
  }

  const std::string ctx = state.input_context();
  if( ctx.empty() ) {
    return;
  }

  this->input_mapper_->activate( ctx );
}

void StateMachine::disable_input_context( const IState& state )
{
  if( this->input_mapper_ == nullptr ) {
    return;
  }

  const std::string ctx = state.input_context();
  if( ctx.empty() ) {
    return;
  }

  this->input_mapper_->disable( ctx );
}

void StateMachine::erase_input_context( const IState& state )
{
  if( this->input_mapper_ == nullptr ) {
    return;
  }

  const std::string ctx = state.input_context();
  if( ctx.empty() ) {
    return;
  }

  this->input_mapper_->erase( ctx );
}

uint32 StateMachine::current_state_id( void ) const
{
  if( this->states_.empty() == true ) {
    return 0;
  }

  return this->states_.back()->id();
}

uint32 StateMachine::next_state_id( void ) const
{
  if( this->next_state_.deferred == true && this->next_state_.state != nullptr ) {
    return this->next_state_.state->id();
  }

  return this->current_state_id();
}

StateSymbol StateMachine::current_state_symbol() const
{
  if( this->states_.empty() ) {
    return NO_STATE;
  }

  return this->states_.back()->symbol();
}

IState::unique_ptr StateMachine::make_state( StateSymbol symbol, void_ptr data ) const
{
  auto it = this->registry_.find( symbol );
  if( it == this->registry_.end() ) {
    NOM_LOG_ERR( NOM, "Could not create state: symbol", symbol, "is not registered." );
    return nullptr;
  }

  NOM_ASSERT( it->second.factory );
  return it->second.factory( data );
}

bool StateMachine::can_push( StateSymbol child, StateSymbol parent ) const
{
  auto it = this->registry_.find( child );
  if( it == this->registry_.end() ) {
    NOM_LOG_ERR( NOM, "push: symbol", child, "is not registered." );
    return false;
  }

  return it->second.parent == parent;
}

void StateMachine::register_state( const StateDescriptor& desc )
{
  NOM_ASSERT( desc.factory );
  this->registry_[desc.symbol] = desc;
}

void StateMachine::push_state( StateSymbol symbol, void_ptr data )
{
  auto state = this->make_state( symbol, data );
  if( ! state ) {
    return;
  }

  auto it = this->registry_.find( symbol );
  if( it != this->registry_.end() && it->second.parent != NO_STATE ) {
    state->set_type( IState::Type::Child );
  }

  this->set_deferred_state( std::move( state ), data );
}

void StateMachine::push( StateSymbol child, void_ptr data )
{
  const StateSymbol parent = this->current_state_symbol();
  if( parent == NO_STATE ) {
    NOM_LOG_ERR( NOM, "push requires an active parent state." );
    return;
  }

  if( ! this->can_push( child, parent ) ) {
    NOM_LOG_ERR( NOM, "push: symbol", child, "is not declared as a child of", parent );
    return;
  }

  auto state = this->make_state( child, data );
  if( ! state ) {
    return;
  }

  state->set_type( IState::Type::Child );
  this->set_deferred_state( std::move( state ), data );
}

void StateMachine::pop( StateSymbol parent, void_ptr data )
{
  if( this->states_.size() <= 1 ) {
    NOM_LOG_ERR( NOM, "pop: not enough states to unwind." );
    return;
  }

  this->next_state_.deferred = true;
  this->next_state_.state = nullptr;
  this->next_state_.data = data;
  this->next_state_.pop_target = parent;
}

void StateMachine::replace( StateSymbol symbol, void_ptr data )
{
  auto state = this->make_state( symbol, data );
  if( ! state ) {
    return;
  }

  state->set_type( IState::Type::Parent );
  this->set_deferred_state( std::move( state ), data );
}

void StateMachine::add_transition( StateSymbol active, EventSymbol event,
                                   StateSymbol stack_top, const Transition& transition )
{
  this->transitions_.add( active, event, stack_top, transition );
}

void StateMachine::add_transition( StateSymbol active, EventSymbol event,
                                   const Transition& transition )
{
  this->transitions_.add( active, event, transition );
}

void StateMachine::set_transition_fallback( const TransitionTable::Callback& callback )
{
  this->transitions_.set_fallback( callback );
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

void StateMachine::on_event( EventSymbol event_symbol, const Event* ev )
{
  if( this->states_.empty() ) {
    return;
  }

  const IState& active = *this->states_.back();
  const StateSymbol stack_top = this->states_.size() >= 2 ?
                                  this->states_[this->states_.size() - 2]->symbol() :
                                  NO_STATE;

  Transition transition = this->transitions_.resolve( active, event_symbol, stack_top );

  switch( transition.action )
  {
    case TransitionAction::None:
    {
      if( ev != nullptr ) {
        this->states_.back()->on_event( *ev );
      }
      break;
    }

    case TransitionAction::Replace:
    {
      this->replace( transition.target, transition.data );
      break;
    }

    case TransitionAction::Push:
    {
      this->push_state( transition.target, transition.data );
      break;
    }

    case TransitionAction::Pop:
    {
      this->pop_state( transition.data );
      break;
    }

    case TransitionAction::PopToParent:
    {
      this->pop( transition.target, transition.data );
      break;
    }
  }
}

void StateMachine::update( float delta )
{
  // Ensure that we have a state in which we can handle update on
  if ( ! this->states_.empty() )
  {
    // Honor user's request to update the state behind us first.
    if ( this->states_.size() >= 2 &&
         ( this->states_.back()->flags() & IState::Flags::OpaqueUpdate ) )
    {
      const size_t prev = this->states_.size() - 2;
      NOM_ASSERT ( this->states_[prev] );

      this->states_[prev]->on_update ( delta );
    }

    // Let the state update the scene with regard to the delta time
    this->states_.back()->on_update ( delta );
  }

  // Set next state, if one is in the queue. We must handle the state transition
  // in a particular way, due to the fact that we allow more than one state to
  // be active within the loop.
  while( this->next_state_.deferred == true )
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
          this->erase_input_context( *this->states_.back() );
          this->states_.pop_back();
        }

        this->states_.push_back( std::move( this->next_state_.state ) );
        this->states_.back()->on_init( this->next_state_.data );
        this->activate_input_context( *this->states_.back() );

        // Next state is now in existence; no longer a pending state
        this->next_state_.deferred = false;
        this->next_state_.state.reset( nullptr );
        this->next_state_.data = nullptr;
        this->next_state_.pop_target = NO_STATE;
        break;
      }
      // Special needs state transition case #1
      else if( this->next_state_.state && this->next_state_.state->type() == IState::Type::Child )
      {
        NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "Child state" );
        NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "State ID:", this->next_state_.state->id() );

        if( ! this->states_.empty() )
        {
          this->states_.back()->on_pause( this->next_state_.data );
          this->disable_input_context( *this->states_.back() );
        }

        this->states_.push_back( std::move( this->next_state_.state ) );
        this->states_.back()->on_init( this->next_state_.data );
        this->activate_input_context( *this->states_.back() );

        // Next state is now in existence; no longer a pending state
        this->next_state_.deferred = false;
        this->next_state_.state.reset( nullptr );
        this->next_state_.data = nullptr;
        this->next_state_.pop_target = NO_STATE;
        break;
      }
    } // end if next_state is not NULL

    // Special needs state transition case #2 (resume previous state)
    else // if next state is NULL
    {
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "Resume previous (parent or child) state" );
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "State ID:", this->current_state_id() );

      if( this->states_.size() <= 1 )
      {
        this->next_state_.deferred = false;
        this->next_state_.data = nullptr;
        this->next_state_.pop_target = NO_STATE;
        continue;
      }

      this->states_.back()->on_exit( this->next_state_.data );
      this->erase_input_context( *this->states_.back() );
      this->states_.pop_back();

      // If a target parent is requested, keep popping until it is active or we
      // hit the bottom of the stack.
      if( this->next_state_.pop_target != NO_STATE &&
          this->states_.back()->symbol() != this->next_state_.pop_target &&
          this->states_.size() > 1 )
      {
        continue;
      }

      this->states_.back()->on_resume( this->next_state_.data );
      this->activate_input_context( *this->states_.back() );

      this->next_state_.deferred = false;
      this->next_state_.data = nullptr;
      this->next_state_.pop_target = NO_STATE;
    }
  } // end while next_state is deferred
}

void StateMachine::draw( RenderWindow& target )
{
  // Ensure that we have a state in which we can handle rendering on
  if ( ! this->states_.empty() )
  {
    // Honor user's request to draw the state behind us first; this allows the
    // current state to use the previous state's rendering as a background.
    if ( this->states_.size() >= 2 &&
         ( this->states_.back()->flags() & IState::Flags::OpaqueRender ) )
    {
      const size_t prev = this->states_.size() - 2;
      NOM_ASSERT ( this->states_[prev] );

      this->states_[prev]->on_draw ( target );
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
  this->next_state_.pop_target = NO_STATE;
}

} // namespace nom
