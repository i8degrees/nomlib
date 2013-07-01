/******************************************************************************
    IState.hpp

  State abstract class

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_ISTATE_HEADERS
#define NOMLIB_ISTATE_HEADERS

#include <iostream>

#include "SDL_Input.hpp"
#include "nomlib.hpp"

namespace nom
{
  class IState: public nom::SDL_Input // "has a" relationship
  {
    public:
      virtual ~IState()
      {
        #ifdef DEBUG_ISTATE_OBJ
          std::cout << "nom::IState::~IState (): " << "Goodbye cruel world!" << std::endl << std::endl;
        #endif
      }

      virtual void onInit ( void ) = 0;
      virtual void onExit ( void ) = 0;

      virtual void Pause ( void ) = 0;
      virtual void Resume ( void ) = 0;

      virtual void Update ( float delta_time ) = 0;
      virtual void Draw ( void* ) = 0;

    private:
      // ...
  };
}

#endif // NOMLIB_ISTATE_HEADERS defined
