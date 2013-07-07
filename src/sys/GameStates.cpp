/******************************************************************************

    (FSM) States management

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved.

******************************************************************************/
#include "GameStates.hpp"

// Initialize our static vars
std::vector<std::unique_ptr<nom::IState>> nom::GameStates::states;

// Default constructor; something is terribly amiss if you ever see this
// initialized!
nom::GameStates::GameStates ( void )
{
NOMLIB_LOG_INFO;
}

void nom::GameStates::onEvent ( void* event )
{
  // let the state handle events
  states.back()->HandleInput ( event );
}

void nom::GameStates::Update ( float delta_time )
{
  // let the state update the scene with regard to the delta (change) in timing
  states.back()->Update ( delta_time );
}

void nom::GameStates::Draw ( void* video_buffer )
{
  // let the state draw the scene onto the display buffer
  states.back()->Draw ( video_buffer );
}

void nom::GameStates::ChangeState ( std::unique_ptr<IState> state )
{
  // cleanup the current state
  if ( ! states.empty() )
  {
    states.back()->onExit();
    states.pop_back();
  }

NOMLIB_ASSERT ( state );

  // store the new state
  states.push_back( std::move( state ) );

  states.back()->onInit();
}

void nom::GameStates::PushState ( std::unique_ptr<IState> state )
{
NOMLIB_ASSERT ( state );

  // pause current state
  if ( ! states.empty() )
    states.back()->Pause();

  // store the new state
  states.push_back( std::move( state ) );
}

void nom::GameStates::PopState ( void )
{
  // cleanup the current state
  if ( ! states.empty() )
    states.pop_back();

  // resume previous state
  states.back()->Resume();
}

void nom::GameStates::PopStateThenChangeState ( std::unique_ptr<IState> state )
{
  // cleanup the current state
  if ( ! states.empty() )
    states.pop_back();

NOMLIB_ASSERT ( state );

  //if ( ! states.empty () )
  GameStates::ChangeState( std::move( state ) );
}
