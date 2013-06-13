/******************************************************************************
    GameStates.cpp

    Game states interface class

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved.

******************************************************************************/
#include "GameStates.hpp"

nom::GameStates::GameStates ( void )
{
  #ifdef DEBUG_GAMESTATES_OBJ
    std::cout << "GameStates::GameStates(): Hello, world!" << "\n" << std::endl;
  #endif
}

void nom::GameStates::onEvent ( SDL_Event *event )
{
  // let the state handle events
  states.back()->HandleInput ( event );
}

void nom::GameStates::Update ( void )
{
  // let the state update the scene
  states.back()->Update();
}

void nom::GameStates::Draw ( void* video_buffer )
{
  // let the state draw the scene
  states.back()->Draw ( video_buffer );
}

void nom::GameStates::ChangeState ( std::unique_ptr<GameState> state )
{
  // cleanup the current state
  if ( ! states.empty() )
  {
    states.back()->onClose();
    states.pop_back();
  }

  // store the new state
  states.push_back( std::move( state ) );

  states.back()->Load();
}

void nom::GameStates::PushState ( std::unique_ptr<GameState> state )
{
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
  if ( !states.empty () )
    states.back()->Resume();
}

void nom::GameStates::PopStateThenChangeState ( std::unique_ptr<GameState> state )
{
  // cleanup the current state
  if ( ! states.empty() )
    states.pop_back();

  if ( ! states.empty () )
    GameStates::ChangeState( std::move( state ) );
}
