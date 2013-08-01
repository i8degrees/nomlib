/******************************************************************************

    SDL Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/sys/Input.hpp"

namespace nom {

Input::Input ( void )
{
NOM_LOG_CLASSINFO;

  if ( SDL_InitSubSystem ( SDL_INIT_JOYSTICK ) == -1 )
  {
NOM_LOG_ERR ( SDL_GetError() );
    return;
  }

  this->joystick = nullptr;

  // Only initialize this joystick if we have yet to
  if ( SDL_JoystickOpened ( 0 ) == 0 )
  {
    SDL_JoystickEventState ( SDL_ENABLE );

    this->joystick = SDL_JoystickOpen ( 0 );

NOM_LOG_INFO ( std::to_string ( SDL_NumJoysticks() ) + " joysticks were found" );

    if ( SDL_NumJoysticks() > 0 )
    {
      for( int idx = 0; idx < SDL_NumJoysticks(); idx++ )
      {
NOM_LOG_INFO ( SDL_JoystickName ( idx ) );
      }
    }
  }
}

Input::~Input ( void )
{
NOM_LOG_CLASSINFO;

  // Only close joysticks we have opened
  //if ( SDL_JoystickOpened ( 0 ) == 1 )
  //{
    //SDL_JoystickClose ( this->joystick );

    //if ( this->joystick )
      //this->joystick = nullptr;
  //}

  //SDL_QuitSubSystem ( SDL_INIT_JOYSTICK );
}


void Input::HandleInput ( void* event )
{
  SDL_Event* input = static_cast<SDL_Event*> ( event );

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

    case SDL_USEREVENT:
      this->onUserEvent ( input->user.type, input->user.code, input->user.data1,
                          input->user.data2
                        );
    break;

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

        case SDL_BUTTON_X1:
          this->onMouseButtonSixDown ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_X2:
          this->onMouseButtonSevenDown ( input->button.x, input->button.y );
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

        case SDL_BUTTON_X1:
          this->onMouseButtonSixUp ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_X2:
          this->onMouseButtonSevenUp ( input->button.x, input->button.y );
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

void Input::onUserEvent ( uint8_t type, int32_t code, void* data1, void* data2 )
{
  // virtual implementation
}

void Input::onQuit ( void )
{
  // virtual implementation
}

void Input::onResize ( int32_t width, int32_t height )
{
  // virtual implementation
}

void Input::onRestore ( void )
{
  // virtual implementation
}

void Input::onMinimize ( void )
{
  // virtual implementation
}

void Input::onInputFocus ( void )
{
  // virtual implementation
}

void Input::onInputBlur ( void )
{
  // virtual implementation
}

void Input::onMouseFocus ( void )
{
  // virtual implementation
}

void Input::onMouseBlur ( void )
{
  // virtual implementation
}

void Input::onExpose ( void )
{
  // virtual implementation
}

void Input::onKeyDown ( int32_t key, int32_t mod )
{
  // virtual implementation
}

void Input::onKeyUp ( int32_t key, int32_t mod )
{
  // virtual implementation
}

void Input::onMouseMotion ( int32_t x, int32_t y )
{
  // virtual implementation
}

void Input::onMouseWheel ( bool up, bool down )
{
  // virtual implementation
}

void Input::onMouseLeftButtonDown ( int32_t x, int32_t y )
{
  // virtual implementation
}

void Input::onMouseMiddleButtonDown ( int32_t x, int32_t y )
{
  // virtual implementation
}

void Input::onMouseRightButtonDown ( int32_t x, int32_t y )
{
  // virtual implementation
}

void Input::onMouseButtonSixDown ( nom::int32 x, nom::int32 y )
{
  // virtual implementation
}

void Input::onMouseButtonSevenDown ( nom::int32 x, nom::int32 y )
{
  // virtual implementation
}

void Input::onMouseLeftButtonUp ( int32_t x, int32_t y )
{
  // virtual implementation
}

void Input::onMouseMiddleButtonUp ( int32_t x, int32_t y )
{
  // virtual implementation
}

void Input::onMouseRightButtonUp ( int32_t x, int32_t y )
{
  // virtual implementation
}

void Input::onMouseButtonSixUp ( nom::int32 x, nom::int32 y )
{
  // virtual implementation
}

void Input::onMouseButtonSevenUp ( nom::int32 x, nom::int32 y )
{
  // virtual implementation
}

void Input::onJoyButtonDown ( int32_t which, int32_t button )
{
  // virtual implementation
}

void Input::onJoyButtonUp ( int32_t which, int32_t button )
{
  // virtual implementation
}

void Input::onJoyAxis ( int32_t which, int32_t axis, short value )
{
  // virtual implementation
}


} // namespace nom
