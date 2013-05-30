/******************************************************************************
    SDLInput.h

    Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_SDLINPUT_HEADERS
#define GAMELIB_SDLINPUT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "gamelib.h"

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

class SDLInput
{
  public:
    SDLInput ( void );
    virtual ~SDLInput ( void );

    virtual void Input ( SDL_Event *input );

    virtual void onExit ( void );
    virtual void onResize ( unsigned int width, unsigned int height );
    virtual void onRestore ( void ); // TODO
    virtual void onMinimize ( void ); // TODO
    virtual void onInputFocus ( void ); // TODO
    virtual void onMouseFocus ( void ); // TODO

    virtual void onKeyDown ( SDLKey key, SDLMod mod );
    virtual void onKeyUp ( SDLKey key, SDLMod mod );

    virtual void onMouseMotion ( unsigned int x, unsigned int y );
    virtual void onMouseWheel ( bool up, bool down );
    virtual void onMouseLeftButtonDown ( unsigned int x, unsigned int y );
    virtual void onMouseMiddleButtonDown ( unsigned int x, unsigned int y );
    virtual void onMouseRightButtonDown ( unsigned int x, unsigned int y );
    virtual void onMouseLeftButtonUp ( unsigned int x, unsigned int y );
    virtual void onMouseMiddleButtonUp ( unsigned int x, unsigned int y );
    virtual void onMouseRightButtonUp ( unsigned int x, unsigned int y );

    virtual void onJoyButtonDown ( unsigned int which, unsigned int button );
    virtual void onJoyButtonUp ( unsigned int which, unsigned int button );
    virtual void onJoyAxis ( unsigned int which, unsigned int axis, short int value );

  private:
    SDL_Joystick *joystick;
};

#endif // GAMELIB_SDLINPUT_HEADERS defined
