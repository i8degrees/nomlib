/******************************************************************************

    (FSM) States management

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_GAMESTATES_HEADERS
#define NOMLIB_GAMESTATES_HEADERS

#include <iostream>
#include <vector>
#include <memory>

#include "config.hpp"
#include "sys/Logger.hpp"
#include "sys/IState.hpp" // abstract class for our interface

namespace nom {

// Rename to FSM, States or similar?
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

    /// States stack; we hold pointer references to IState inherited classes
    static std::vector<std::unique_ptr<IState>> states;
  private:
    GameStates ( void );
};


} // namespace nom

#endif // NOMLIB_GAMESTATES_HEADERS defined
