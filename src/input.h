/******************************************************************************
    input.h

    Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_INPUT_HEADERS
#define GAMELIB_INPUT_HEADERS

#include <iostream>
#include <string>

#include "SDL/SDL.h"

class SDLInput
{
  public:
    //SDLInput ( void );
    virtual ~SDLInput ( void );

    virtual void HandleInput ( SDL_Event *input );

    virtual void onExit ( void );
    virtual void onResize ( void );

    virtual void keyDown ( SDLKey key, SDLMod mod );
    virtual void keyUp ( SDLKey key, SDLMod mod );
    virtual void mouseMotion ( unsigned int x, unsigned int y );
    virtual void mouseLeftButtonDown ( unsigned int x, unsigned int y );
    virtual void mouseMiddleButtonDown ( unsigned int x, unsigned int y );
    virtual void mouseRightButtonDown ( unsigned int x, unsigned int y );
    virtual void mouseLeftButtonUp ( unsigned int x, unsigned int y );
    virtual void mouseMiddleButtonUp ( unsigned int x, unsigned int y );
    virtual void mouseRightButtonUp ( unsigned int x, unsigned int y );

  private:
    //
};

#endif // GAMELIB_INPUT_HEADERS defined
