/******************************************************************************
    I_Input.hpp

    Pure abstract class interface for input events

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_INTERFACE_INPUT_HEADERS
#define NOMLIB_INTERFACE_INPUT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "gamelib.h"

namespace nom
{
  class I_Input
  {
    public:
      I_Input ( void ) {}
      virtual ~I_Input ( void ) {}

      virtual void HandleInput ( void* event ) = 0;

      virtual void onUserEvent ( uint8_t type, int32_t code, void* data1, void* data2 ) = 0;
      virtual void onExit ( void ) = 0;
      virtual void onResize ( int32_t width, int32_t height ) = 0;
      virtual void onRestore ( void ) = 0;
      virtual void onMinimize ( void ) = 0;
      virtual void onInputFocus ( void ) = 0;
      virtual void onInputBlur ( void ) = 0;
      virtual void onMouseFocus ( void ) = 0;
      virtual void onMouseBlur ( void ) = 0;
      virtual void onExpose ( void ) = 0;

      virtual void onKeyDown ( int32_t key, int32_t mod ) = 0;
      virtual void onKeyUp ( int32_t key, int32_t mod ) = 0;

      virtual void onMouseMotion ( int32_t x, int32_t y ) = 0;
      virtual void onMouseWheel ( bool up, bool down ) = 0;
      virtual void onMouseLeftButtonDown ( int32_t x, int32_t y ) = 0;;
      virtual void onMouseMiddleButtonDown ( int32_t x, int32_t y ) = 0;
      virtual void onMouseRightButtonDown ( int32_t x, int32_t y ) = 0;
      virtual void onMouseLeftButtonUp ( int32_t x, int32_t y ) = 0;
      virtual void onMouseMiddleButtonUp ( int32_t x, int32_t y ) = 0;
      virtual void onMouseRightButtonUp ( int32_t x, int32_t y ) = 0;

      virtual void onJoyButtonDown ( int32_t which, int32_t button ) = 0;
      virtual void onJoyButtonUp ( int32_t which, int32_t button ) = 0;
      virtual void onJoyAxis ( int32_t which, int32_t axis, short value ) = 0;


    private:
      // ...
  };
}

#endif // NOMLIB_INTERFACE_INPUT_HEADERS defined
