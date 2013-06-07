/******************************************************************************
    SDL_Input.cpp

    SDL Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Input.hpp"

nom::SDL_Input::SDL_Input ( void )
{
  #ifdef DEBUG_SDL_INPUT_OBJ
    std::cout << "SDL_Input::SDL_Input (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  if ( SDL_InitSubSystem ( SDL_INIT_JOYSTICK ) == -1 )
  {
    std::cout << "ERR in SDL_Input::SDL_Input() at: " << SDL_GetError() << std::endl;
    return;
  }

  this->joystick = NULL;

  // Only initialize this joystick if we have yet to
  if ( SDL_JoystickOpened ( 0 ) == 0 )
  {
    SDL_JoystickEventState ( SDL_ENABLE );

    this->joystick = SDL_JoystickOpen ( 0 );

    std::cout << SDL_NumJoysticks() << " joysticks were found.";
    std::cout << std::endl << std::endl;

    if ( SDL_NumJoysticks() > 0 )
    {
      for( int idx = 0; idx < SDL_NumJoysticks(); idx++ )
      {
        std::cout << SDL_JoystickName ( idx ) << std::endl << std::endl;
      }
    }
  }
}

nom::SDL_Input::~SDL_Input ( void )
{
  #ifdef DEBUG_SDL_INPUT_OBJ
    std::cout << "SDL_Input::~SDL_Input (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Only close joysticks we have opened
  //if ( SDL_JoystickOpened ( 0 ) == 1 )
  //{
    //SDL_JoystickClose ( this->joystick );

    //if ( this->joystick )
      //this->joystick = NULL;
  //}

  //SDL_QuitSubSystem ( SDL_INIT_JOYSTICK );
}


void nom::SDL_Input::HandleInput ( SDL_Event *input )
{
  switch ( input->type )
  {
    case SDL_KEYDOWN:
      onKeyDown ( input->key.keysym.sym, input->key.keysym.mod );
    break;

    case SDL_KEYUP:
      // Temporary state crash workaround by commenting below line out of exec:
      onKeyUp ( input->key.keysym.sym, input->key.keysym.mod );
    break;

    case SDL_MOUSEMOTION:
      // Temporary state crash workaround by commenting below line out of exec:
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

        case SDL_BUTTON_WHEELDOWN: onMouseWheel ( false, true ); break;

        case SDL_BUTTON_WHEELUP: onMouseWheel ( true, false ); break;
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

    case SDL_JOYBUTTONDOWN:
      onJoyButtonDown ( input->jbutton.which, input->jbutton.button );
    break;

    case SDL_JOYBUTTONUP:
      onJoyButtonUp ( input->jbutton.which, input->jbutton.button );
    break;

    case SDL_JOYAXISMOTION:
      // Temporary state crash workaround by commenting below line out of exec:
      onJoyAxis ( input->jaxis.which, input->jaxis.axis, input->jaxis.value );
    break;

    case SDL_QUIT: onExit(); break;

    case SDL_VIDEORESIZE: onResize ( input->resize.w, input->resize.h ); break;
  } // end switch input->type
}

void nom::SDL_Input::onExit ( void )
{
  // virtual implementation
}

void nom::SDL_Input::onResize ( unsigned int width, unsigned int height )
{
  // virtual implementation
}

void nom::SDL_Input::onRestore ( void )
{
  // virtual implementation
}

void nom::SDL_Input::onMinimize ( void )
{
  // virtual implementation
}

void nom::SDL_Input::onInputFocus ( void )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseFocus ( void )
{
  // virtual implementation
}

void nom::SDL_Input::onKeyDown ( SDLKey key, SDLMod mod )
{
  // virtual implementation
}

void nom::SDL_Input::onKeyUp ( SDLKey key, SDLMod mod )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseMotion ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseWheel ( bool up, bool down )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseLeftButtonDown ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseMiddleButtonDown ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseRightButtonDown ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseLeftButtonUp ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseMiddleButtonUp ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseRightButtonUp ( unsigned int x, unsigned int y )
{
  // virtual implementation
}

void nom::SDL_Input::onJoyButtonDown ( unsigned int which, unsigned int button )
{
  // virtual implementation
}

void nom::SDL_Input::onJoyButtonUp ( unsigned int which, unsigned int button )
{
  // virtual implementation
}

void nom::SDL_Input::onJoyAxis ( unsigned int which, unsigned int axis, short int value )
{
  // virtual implementation
}
