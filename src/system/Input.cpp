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
NOM_LOG_TRACE ( NOM );

  if ( SDL_WasInit( SDL_INIT_JOYSTICK ) == false )
  {
    if ( SDL_InitSubSystem ( SDL_INIT_JOYSTICK ) < 0 )
    {
NOM_LOG_ERR ( NOM, SDL_GetError() );
      return;
    }
  }

  this->joystick = nullptr;
/* TODO
NOM_LOG_INFO ( NOM, std::to_string ( SDL_NumJoysticks() ) + " joysticks were found" );

  if ( SDL_NumJoysticks() >= 0 )
  {
    SDL_JoystickEventState ( SDL_ENABLE );

    this->joystick = ( SDL_JoystickOpen( 0 ) );

    for( int idx = 0; idx < SDL_NumJoysticks(); idx++ )
    {
NOM_LOG_INFO ( NOM, SDL_JoystickName ( idx ) );
    }
  }
TODO */
}

Input::~Input ( void )
{
NOM_LOG_TRACE ( NOM );
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


void Input::HandleInput ( EventType* event )
{
  // TODO: Remove me
  EventType* input = event;

  switch ( input->type )
  {
    default: /* Ignore unknown events */ break;

    case SDL_QUIT: this->onQuit(); break;

    case SDL_WINDOWEVENT:
    {
      switch ( input->window.event )
      {
        default: /* Ignore unknown events */ break;

        case SDL_WINDOWEVENT_CLOSE: this->onQuit(); break;

/* TODO
      case SDL_WINDOWEVENT_ENTER: break;
      case SDL_WINDOWEVENT_EXPOSED: break;
      case SDL_WINDOWEVENT_FOCUS_GAINED: break;
      case SDL_WINDOWEVENT_FOCUS_LOST: break;
      case SDL_WINDOWEVENT_HIDDEN: break;
      case SDL_WINDOWEVENT_LEAVE: break;
      case SDL_WINDOWEVENT_MAXIMIZED: break;
      case SDL_WINDOWEVENT_MINIMIZED: break;
      case SDL_WINDOWEVENT_MOVED: break;
      case SDL_WINDOWEVENT_RESIZED: break;
      case SDL_WINDOWEVENT_RESTORED: break;
      case SDL_WINDOWEVENT_SHOWN: break;
TODO */
    } // end input->window.event switch
  } // end input->type switch

    case SDL_USEREVENT:
      this->onUserEvent ( input->user.type, input->user.code, input->user.data1,
                          input->user.data2
                        );
    break;

    case SDL_SYSWMEVENT: /* Ignore */ break;

    case SDL_KEYDOWN:
      this->onKeyDown ( input->key.keysym.sym, input->key.keysym.mod, input->button.windowID );
    break;

    case SDL_KEYUP:
      this->onKeyUp ( input->key.keysym.sym, input->key.keysym.mod, input->button.windowID );
    break;

    case SDL_MOUSEMOTION:
      this->onMouseMotion ( input->motion.x, input->motion.y, input->motion.windowID );
    break;

    case SDL_MOUSEBUTTONDOWN:
    {
      switch ( input->button.button )
      {
        case SDL_BUTTON_LEFT:
          this->onMouseLeftButtonDown ( input->button.x, input->button.y, input->button.windowID );
        break;

        case SDL_BUTTON_MIDDLE:
          this->onMouseMiddleButtonDown ( input->button.x, input->button.y, input->button.windowID );
        break;

        case SDL_BUTTON_RIGHT:
          this->onMouseRightButtonDown ( input->button.x, input->button.y, input->button.windowID );
        break;

        case SDL_BUTTON_X1:
          this->onMouseButtonSixDown ( input->button.x, input->button.y, input->button.windowID );
        break;

        case SDL_BUTTON_X2:
          this->onMouseButtonSevenDown ( input->button.x, input->button.y, input->button.windowID );
        break;
      }
      break;
    } // end SDL_MOUSEBUTTONDOWN event

    case SDL_MOUSEBUTTONUP:
      switch ( input->button.button )
      {
        case SDL_BUTTON_LEFT:
          this->onMouseLeftButtonUp ( input->button.x, input->button.y, input->button.windowID );
        break;

        case SDL_BUTTON_MIDDLE:
          this->onMouseMiddleButtonUp ( input->button.x, input->button.y, input->button.windowID );
        break;

        case SDL_BUTTON_RIGHT:
          this->onMouseRightButtonUp ( input->button.x, input->button.y, input->button.windowID );
        break;

        case SDL_BUTTON_X1:
          this->onMouseButtonSixUp ( input->button.x, input->button.y, input->button.windowID );
        break;

        case SDL_BUTTON_X2:
          this->onMouseButtonSevenUp ( input->button.x, input->button.y, input->button.windowID );
        break;
      }
    break;

    case SDL_MOUSEWHEEL:
    {
      this->onMouseWheel ( input->wheel.x, input->wheel.y, input->wheel.windowID );
    } // end SDL_MOUSEWHEEL event
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

    case SDL_DROPFILE:
    {
      char* file = input->drop.file;
      this->onDragDrop ( file );
      SDL_free ( file );
    } // end SDL_DROPFILE event
    break;

  } // end switch input->type
}

void Input::onUserEvent ( uint8 type, int32 code, void* data1, void* data2 )
{
  // virtual implementation
}

void Input::onQuit ( void )
{
  // virtual implementation
}

void Input::onResize ( int32 width, int32 height )
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

void Input::onKeyDown ( int32 key, int32 mod, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_KEYBOARD_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( key );
  NOM_DUMP_VAR ( mod );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onKeyUp ( int32 key, int32 mod, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_KEYBOARD_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( key );
  NOM_DUMP_VAR ( mod );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseMotion ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseWheel ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseLeftButtonDown ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseMiddleButtonDown ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseRightButtonDown ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseButtonSixDown ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseButtonSevenDown ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseLeftButtonUp ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseMiddleButtonUp ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseRightButtonUp ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseButtonSixUp ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseButtonSevenUp ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onJoyButtonDown ( int32 which, int32 button )
{
  // virtual implementation
}

void Input::onJoyButtonUp ( int32 which, int32 button )
{
  // virtual implementation
}

void Input::onJoyAxis ( int32 which, int32 axis, uint16 value )
{
  // virtual implementation
}

void Input::onDragDrop ( const std::string& file_path )
{
  // Do nothing virtual implementation
}


} // namespace nom
