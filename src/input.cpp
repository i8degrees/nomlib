/******************************************************************************
    input.cpp

    Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "input.h"

SDLInput::SDLInput ( void )
{
  #ifdef DEBUG_INPUT_OBJ
    std::cout << "SDLInput::SDLInput (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}

SDLInput::~SDLInput ( void )
{
  #ifdef DEBUG_INPUT_OBJ
    std::cout << "SDLInput::~SDLInput (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}

void SDLInput::HandleInput ( SDL_Event *input )
{
  switch ( input->type )
  {
    case SDL_QUIT:
      onExit();
    break;

    case SDL_VIDEORESIZE:
      onResize ( input->resize.w, input->resize.h );
    break;

    case SDL_KEYDOWN:
      onKeyDown ( input->key.keysym.sym, input->key.keysym.mod );
    break;

    case SDL_KEYUP:
      onKeyUp ( input->key.keysym.sym, input->key.keysym.mod );
    break;

    case SDL_MOUSEMOTION:
      onMouseMotion ( input->motion.x, input->motion.y );
    break;

    case SDL_MOUSEBUTTONDOWN:
      switch ( input->button.button )
      {
        case SDL_BUTTON_LEFT:
          onMouseLeftButtonDown ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_MIDDLE:
          onMouseMiddleButtonDown ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_RIGHT:
          onMouseRightButtonDown ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_WHEELDOWN: // wheel down
          onMouseWheel ( false, true );
        break;

        case SDL_BUTTON_WHEELUP: // wheel up
          onMouseWheel ( true, false );
        break;
      }
    break;

    case SDL_MOUSEBUTTONUP:
      switch ( input->button.button )
      {
        case SDL_BUTTON_LEFT:
          onMouseLeftButtonUp ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_MIDDLE:
          onMouseMiddleButtonUp ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_RIGHT:
          onMouseRightButtonUp ( input->button.x, input->button.y );
        break;
      }
    break;
  } // end switch input->type
}

void SDLInput::onExit ( void )
{
  // virtual implementation
}

void SDLInput::onResize ( unsigned int width, unsigned int height )
{
  // virtual implementation
}

void SDLInput::onRestore ( void )
{
  // virtual implementation
}

void SDLInput::onMinimize ( void )
{
  // virtual implementation
}

void SDLInput::onInputFocus ( void )
{
  // virtual implementation
}

void SDLInput::onMouseFocus ( void )
{
  // virtual implementation
}

void SDLInput::onKeyDown ( SDLKey key, SDLMod mod )
{
  // virtual implementation
}

void SDLInput::onKeyUp ( SDLKey key, SDLMod mod )
{
  // virtual implementation
}

void SDLInput::onMouseMotion ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void SDLInput::onMouseWheel ( bool up, bool down )
{
  // virtual implementation
}

void SDLInput::onMouseLeftButtonDown ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void SDLInput::onMouseMiddleButtonDown ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void SDLInput::onMouseRightButtonDown ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void SDLInput::onMouseLeftButtonUp ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void SDLInput::onMouseMiddleButtonUp ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void SDLInput::onMouseRightButtonUp ( unsigned int x, unsigned int y )
{
  // virtual implementation
}
