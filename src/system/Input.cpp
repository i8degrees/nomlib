/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "nomlib/system/Input.hpp"

namespace nom {

Input::Input ( void )
{
NOM_LOG_CLASSINFO;

  if ( SDL_WasInit( SDL_INIT_JOYSTICK ) == false )
  {
    if ( SDL_InitSubSystem ( SDL_INIT_JOYSTICK ) < 0 )
    {
NOM_LOG_ERR ( SDL_GetError() );
      return;
    }
  }

  this->joystick = nullptr;

NOM_LOG_INFO ( std::to_string ( SDL_NumJoysticks() ) + " joysticks were found" );

  if ( SDL_NumJoysticks() >= 0 )
  {
    SDL_JoystickEventState ( SDL_ENABLE );

    this->joystick = ( SDL_JoystickOpen( 0 ) );

    for( int idx = 0; idx < SDL_NumJoysticks(); idx++ )
    {
NOM_LOG_INFO ( SDL_JoystickName ( idx ) );
    }
  }
}

Input::~Input ( void )
{
NOM_LOG_CLASSINFO;
/*
  if ( this->joystick != nullptr )
  {
    // Only close joysticks we have opened
    if ( SDL_JoystickOpened ( 0 ) == 1 )
    {
      SDL_JoystickClose ( this->joystick.get() );

      if ( this->joystick != nullptr )
        this->joystick = nullptr;
    }
  }
*/
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

void Input::onMouseButtonSixDown ( int32 x, int32 y )
{
  // virtual implementation
}

void Input::onMouseButtonSevenDown ( int32 x, int32 y )
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

void Input::onMouseButtonSixUp ( int32 x, int32 y )
{
  // virtual implementation
}

void Input::onMouseButtonSevenUp ( int32 x, int32 y )
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
