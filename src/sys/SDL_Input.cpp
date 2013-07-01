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

  this->joystick = nullptr;

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
      //this->joystick = nullptr;
  //}

  //SDL_QuitSubSystem ( SDL_INIT_JOYSTICK );
}


void nom::SDL_Input::HandleInput ( void* event )
{
  SDL_Event *input = (SDL_Event*) event;

  switch ( input->type )
  {
    case SDL_ACTIVEEVENT:
    {
      switch ( input->active.state )
      {
        case SDL_APPMOUSEFOCUS:
        {
          if ( input->active.gain )
            this->onMouseFocus();
          else
            this->onMouseBlur();
          break;
        }

        case SDL_APPINPUTFOCUS:
        {
          if ( input->active.gain )
            this->onInputFocus();
          else
            this->onInputBlur();
          break;
        }
        case SDL_APPACTIVE:
        {
          if ( input->active.gain )
            this->onRestore();
          else
            this->onMinimize();
          break;
        }

      }
    break;
    }

    default:
    {
      this->onUserEvent ( input->user.type, input->user.code, input->user.data1,
                    input->user.data2
                  );
      break;
    }

    case SDL_VIDEORESIZE: this->onResize ( input->resize.w, input->resize.h ); break;
    case SDL_VIDEOEXPOSE: this->onExpose (); break;
    case SDL_SYSWMEVENT: /* Ignore */ break;
    case SDL_QUIT: this->onQuit(); break;

    case SDL_KEYDOWN:
      this->onKeyDown ( input->key.keysym.sym, input->key.keysym.mod );
    break;

    case SDL_KEYUP:
      this->onKeyUp ( input->key.keysym.sym, input->key.keysym.mod );
    break;

    case SDL_MOUSEMOTION:
      this->onMouseMotion ( input->motion.x, input->motion.y );
    break;

    case SDL_MOUSEBUTTONDOWN:
      switch ( input->button.button )
      {
        case SDL_BUTTON_LEFT:
          this->onMouseLeftButtonDown ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_MIDDLE:
          this->onMouseMiddleButtonDown ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_RIGHT:
          this->onMouseRightButtonDown ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_WHEELDOWN: this->onMouseWheel ( false, true ); break;

        case SDL_BUTTON_WHEELUP: this->onMouseWheel ( true, false ); break;
      }
    break;

    case SDL_MOUSEBUTTONUP:
      switch ( input->button.button )
      {
        case SDL_BUTTON_LEFT:
          this->onMouseLeftButtonUp ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_MIDDLE:
          this->onMouseMiddleButtonUp ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_RIGHT:
          this->onMouseRightButtonUp ( input->button.x, input->button.y );
        break;
      }
    break;

    case SDL_JOYBUTTONDOWN:
      this->onJoyButtonDown ( input->jbutton.which, input->jbutton.button );
    break;

    case SDL_JOYBUTTONUP:
      this->onJoyButtonUp ( input->jbutton.which, input->jbutton.button );
    break;

    case SDL_JOYAXISMOTION:
      this->onJoyAxis ( input->jaxis.which, input->jaxis.axis, input->jaxis.value );
    break;
  } // end switch input->type
}

void nom::SDL_Input::onUserEvent ( uint8_t type, int32_t code, void* data1, void* data2 )
{
  // virtual implementation
}

void nom::SDL_Input::onQuit ( void )
{
  // virtual implementation
}

void nom::SDL_Input::onResize ( int32_t width, int32_t height )
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

void nom::SDL_Input::onInputBlur ( void )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseFocus ( void )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseBlur ( void )
{
  // virtual implementation
}

void nom::SDL_Input::onExpose ( void )
{
  // virtual implementation
}

void nom::SDL_Input::onKeyDown ( int32_t key, int32_t mod )
{
  // virtual implementation
}

void nom::SDL_Input::onKeyUp ( int32_t key, int32_t mod )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseMotion ( int32_t x, int32_t y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseWheel ( bool up, bool down )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseLeftButtonDown ( int32_t x, int32_t y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseMiddleButtonDown ( int32_t x, int32_t y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseRightButtonDown ( int32_t x, int32_t y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseLeftButtonUp ( int32_t x, int32_t y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseMiddleButtonUp ( int32_t x, int32_t y )
{
  // virtual implementation
}

void nom::SDL_Input::onMouseRightButtonUp ( int32_t x, int32_t y )
{
  // virtual implementation
}

void nom::SDL_Input::onJoyButtonDown ( int32_t which, int32_t button )
{
  // virtual implementation
}

void nom::SDL_Input::onJoyButtonUp ( int32_t which, int32_t button )
{
  // virtual implementation
}

void nom::SDL_Input::onJoyAxis ( int32_t which, int32_t axis, short value )
{
  // virtual implementation
}
