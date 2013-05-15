/******************************************************************************
    GameStateManager.h

  Game State Management

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston

******************************************************************************/
#ifndef GAMEAPP_GAMESTATE_MANAGER_HEADERS
#define GAMEAPP_GAMESTATE_MANAGER_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "GameState.h"

#define DEBUG_GAMESTATE_MANAGER
#define DEBUG_GAMESTATE_MANAGER_OBJ

class GameStateManager: public GameState
{
  public:
    GameStateManager ( void );
    ~GameStateManager ( void );

    void ChangeState ( std::unique_ptr<GameState> state );
    void PushState ( std::unique_ptr<GameState> state );
    void PopState ( void );
    void PopStateThenChangeState ( std::unique_ptr<GameState> state );

    void Input ( void );
    void Update ( void );
    void Draw ( void );
    //void Draw ( float interpolation );

  private:
    // state stack
    std::vector<std::unique_ptr<GameState>> states;
};

#endif // GAMEAPP_GAMESTATE_MANAGER_HEADERS defined
