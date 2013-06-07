/******************************************************************************
    SDL_Input.hpp

    SDL Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_INPUT_HEADERS
#define NOMLIB_SDL_INPUT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "gamelib.h"

#include "I_Input.hpp"

namespace nom
{
  // SDL mapping to individual PS3 axes
  // TODO: Needs testing / verification
  enum PSXAXES
  {
      LEFTX = 0,
      LEFTY = 1,
      RIGHTX = 2,
      RIGHTY = 3
  };

  // SDL mapping to individual PS3 controller buttons
  // TODO: Needs testing / verification
  enum PSXBUTTON
  {
      SELECT            = 0,
      JOYCLICKLEFT      = 1,
      JOYCLICKRIGHT     = 2,
      START             = 3,
      UP                = 4,
      RIGHT             = 5,
      DOWN              = 6,
      LEFT              = 7,
      L2                = 8,
      R2                = 9,
      L1                = 10,
      R1                = 11,
      TRIANGLE          = 12,
      CIRCLE            = 13,
      CROSS             = 14,
      SQUARE            = 15,
      PSBUTTON          = 16
  };

  class SDL_Input: public nom::I_Input
  {
    public:
      SDL_Input ( void );
      virtual ~SDL_Input ( void );

      virtual void HandleInput ( void* event );

      virtual void onExit ( void );
      virtual void onResize ( int32_t width, int32_t height );
      virtual void onRestore ( void ); // TODO
      virtual void onMinimize ( void ); // TODO
      virtual void onInputFocus ( void ); // TODO
      virtual void onMouseFocus ( void ); // TODO

      virtual void onKeyDown ( int32_t key, int32_t mod );
      virtual void onKeyUp ( int32_t key, int32_t mod );

      virtual void onMouseMotion ( int32_t x, int32_t y );
      virtual void onMouseWheel ( bool up, bool down );
      virtual void onMouseLeftButtonDown ( int32_t x, int32_t y );
      virtual void onMouseMiddleButtonDown ( int32_t x, int32_t y );
      virtual void onMouseRightButtonDown ( int32_t x, int32_t y );
      virtual void onMouseLeftButtonUp ( int32_t x, int32_t y );
      virtual void onMouseMiddleButtonUp ( int32_t x, int32_t y );
      virtual void onMouseRightButtonUp ( int32_t x, int32_t y );

      virtual void onJoyButtonDown ( int32_t which, int32_t button );
      virtual void onJoyButtonUp ( int32_t which, int32_t button );
      virtual void onJoyAxis ( int32_t which, int32_t axis, short value );

    private:
      SDL_Joystick *joystick;
  };
}

#endif // NOMLIB_SDL_INPUT_HEADERS defined
