/******************************************************************************
    GameStateManager.cpp

  Game State Management

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "GameStateManager.h"

GameStateManager::GameStateManager ( void )
{
  #ifdef DEBUG_GAMESTATE_MANAGER_OBJ
    std::cout << "GameStateManager::GameStateManager (): Hello, world!" << "\n" << std::endl;
  #endif

  //states.clear();
}

GameState::~GameState ( void )
{
  #ifdef DEBUG_GAMESTATE_OBJ
    std::cout << "GameState::~GameState (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

GameStateManager::~GameStateManager ( void )
{
  #ifdef DEBUG_GAMESTATE_MANAGER_OBJ
    std::cout << "GameStateManager::~GameStateManager (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  // cleanup all of the states
  while ( !states.empty() )
    states.pop_back();
}

void GameStateManager::ChangeState( std::unique_ptr<GameState> state )
{
  // cleanup the current state
  if ( !states.empty() )
    states.pop_back();

  // store the new state
  states.push_back( std::move( state ) );
}

void GameStateManager::PushState( std::unique_ptr<GameState> state )
{
  // pause current state
  if ( !states.empty() )
    states.back()->Pause();

  // store the new state
  states.push_back( std::move( state ) );
}

void GameStateManager::PopState ( void )
{
  // cleanup the current state
  if ( !states.empty() )
    states.pop_back();

  // resume previous state
  if ( !states.empty () )
    states.back()->Resume();
}

/*
void GameStateManager::PopStateThenChangeState( std::unique_ptr<GameState> state )
{
  // cleanup the current state
  if ( !states.empty() )
    states.pop_back();

  if ( !states.empty () )
    states.back()->ChangeState( this, std::move( state ) );
}
*/

void GameStateManager::Input ( void )
{
  // let the state handle events
  states.back()->Input();
  //states.back()->Input(this);
}

void GameStateManager::Update ( void )
{
  states.back()->Update();
  //states.back()->Update(this);
}

void GameStateManager::Draw( void )
//void GameStateManager::Draw( float interpolation )
{
  // let the state draw the scene
  states.back()->Draw();
  //states.back()->Draw(this);
}
