/******************************************************************************

    SDL Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_INPUT_HEADERS
#define NOMLIB_SDL_INPUT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "nomlib/config.hpp"

namespace nom {

/// SDL mapping to individual PS3 axes
///
/// TODO: Needs testing / verification
enum PSXAXES
{
  LEFTX = 0,
  LEFTY = 1,
  RIGHTX = 2,
  RIGHTY = 3
};

/// SDL mapping to individual PS3 controller buttons
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

// This is an inheritance-only class
/// \brief SDL Input Events Handling
class Input
{
  public:
    Input ( void );
    virtual ~Input ( void );

    virtual void HandleInput ( SDL_Event* event );
  protected:
    virtual void onUserEvent ( uint8 type, int32 code, void* data1, void* data2 );

    virtual void onQuit ( void );
    virtual void onResize ( int32 width, int32 height );
    virtual void onRestore ( void );
    virtual void onMinimize ( void );
    virtual void onInputFocus ( void );
    virtual void onInputBlur ( void );
    virtual void onMouseFocus ( void );
    virtual void onMouseBlur ( void );
    virtual void onExpose ( void );

    virtual void onKeyDown ( int32 key, int32 mod );
    virtual void onKeyUp ( int32 key, int32 mod );

    virtual void onMouseMotion ( int32 x, int32 y );
    virtual void onMouseWheel ( bool up, bool down );
    virtual void onMouseLeftButtonDown ( int32 x, int32 y );
    virtual void onMouseMiddleButtonDown ( int32 x, int32 y );
    virtual void onMouseRightButtonDown ( int32 x, int32 y );
    virtual void onMouseButtonSixDown ( int32 x, int32 y );
    virtual void onMouseButtonSevenDown ( int32 x, int32 y );

    virtual void onMouseLeftButtonUp ( int32 x, int32 y );
    virtual void onMouseMiddleButtonUp ( int32 x, int32 y );
    virtual void onMouseRightButtonUp ( int32 x, int32 y );
    virtual void onMouseButtonSixUp ( int32 x, int32 y );
    virtual void onMouseButtonSevenUp ( int32 x, int32 y );

    virtual void onJoyButtonDown ( int32 which, int32 button );
    virtual void onJoyButtonUp ( int32 which, int32 button );
    virtual void onJoyAxis ( int32 which, int32 axis, uint16 value );

  private:
    SDL_Joystick* joystick;
};


} // namespace nom

#endif // NOMLIB_SDL_INPUT_HEADERS defined
