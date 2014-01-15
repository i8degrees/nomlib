/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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

Input::Input ( void ) :
    joystick_ { Input::JoystickUniquePtr ( nullptr, priv::Free_Joystick ) },
    joystick_id_ ( 0 )
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

  NOM_LOG_INFO ( NOM, std::to_string ( SDL_NumJoysticks() ) + " joysticks were found" );

  if ( SDL_NumJoysticks() > 0 )
  {
    this->joystick_.reset ( SDL_JoystickOpen ( 0 ) );

    if ( this->joystick_.get() )
    {
      for( int idx = 0; idx < SDL_NumJoysticks(); idx++ )
      {
        NOM_LOG_INFO ( NOM, SDL_JoystickNameForIndex ( idx ) );
      }

      SDL_JoystickEventState ( SDL_ENABLE );

      this->joystick_id_ = SDL_JoystickInstanceID ( this->joystick_.get() );
    }
  }
}

Input::~Input ( void )
{
  NOM_LOG_TRACE ( NOM );
}


void Input::HandleInput ( EventType* event )
{
  switch ( event->type )
  {
    default: /* Ignore unknown events */ break;

    case SDL_QUIT: this->on_quit(); break;

    case SDL_WINDOWEVENT:
    {
      switch ( event->window.event )
      {
        default: /* Ignore unknown events */ break;

        case SDL_WINDOWEVENT_CLOSE: this->on_quit(); break;

/* TODO: SDL2 window events handling
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
    } // end event->window.event switch
  } // end event->type switch

    case SDL_USEREVENT:
      this->onUserEvent ( event->user.type, event->user.code, event->user.data1,
                          event->user.data2
                        );
    break;

    case SDL_SYSWMEVENT: /* Ignore */ break;

    case SDL_KEYDOWN:
      this->onKeyDown ( event->key.keysym.sym, event->key.keysym.mod, event->button.windowID );
    break;

    case SDL_KEYUP:
      this->onKeyUp ( event->key.keysym.sym, event->key.keysym.mod, event->button.windowID );
    break;

    case SDL_MOUSEMOTION:
      this->onMouseMotion ( event->motion.x, event->motion.y, event->motion.windowID );
    break;

    case SDL_MOUSEBUTTONDOWN:
    {
      switch ( event->button.button )
      {
        case SDL_BUTTON_LEFT:
          this->onMouseLeftButtonDown ( event->button.x, event->button.y, event->button.windowID );
        break;

        case SDL_BUTTON_MIDDLE:
          this->onMouseMiddleButtonDown ( event->button.x, event->button.y, event->button.windowID );
        break;

        case SDL_BUTTON_RIGHT:
          this->onMouseRightButtonDown ( event->button.x, event->button.y, event->button.windowID );
        break;

        case SDL_BUTTON_X1:
          this->onMouseButtonFourDown ( event->button.x, event->button.y, event->button.windowID );
        break;

        case SDL_BUTTON_X2:
          this->onMouseButtonFiveDown ( event->button.x, event->button.y, event->button.windowID );
        break;
      }
      break;
    } // end SDL_MOUSEBUTTONDOWN event

    case SDL_MOUSEBUTTONUP:
      switch ( event->button.button )
      {
        case SDL_BUTTON_LEFT:
          this->onMouseLeftButtonUp ( event->button.x, event->button.y, event->button.windowID );
        break;

        case SDL_BUTTON_MIDDLE:
          this->onMouseMiddleButtonUp ( event->button.x, event->button.y, event->button.windowID );
        break;

        case SDL_BUTTON_RIGHT:
          this->onMouseRightButtonUp ( event->button.x, event->button.y, event->button.windowID );
        break;

        case SDL_BUTTON_X1:
          this->onMouseButtonFourUp ( event->button.x, event->button.y, event->button.windowID );
        break;

        case SDL_BUTTON_X2:
          this->onMouseButtonFiveUp ( event->button.x, event->button.y, event->button.windowID );
        break;
      }
    break;

    case SDL_MOUSEWHEEL:
    {
      this->onMouseWheel ( event->wheel.x, event->wheel.y, event->wheel.windowID );
    } // end SDL_MOUSEWHEEL event
    break;

    case SDL_JOYBUTTONDOWN:
      this->onJoyButtonDown ( this->joystick_id_, event->jbutton.button );
    break;

    case SDL_JOYBUTTONUP:
      this->onJoyButtonUp ( this->joystick_id_, event->jbutton.button );
    break;

    case SDL_JOYAXISMOTION:
      this->onJoyAxis ( this->joystick_id_, event->jaxis.axis, event->jaxis.value );
    break;

    case SDL_JOYDEVICEADDED:
    {
      this->onJoystickAdd();
      break;
    }
    case SDL_JOYDEVICEREMOVED:
    {
      this->onJoystickRemove();
      break;
    }
/*
    case SDL_FINGERMOTION:
    {
      NOM_DUMP_VAR( event->tfinger.timestamp );
      NOM_DUMP_VAR( event->tfinger.touchId );
      NOM_DUMP_VAR( event->tfinger.fingerId );
      NOM_DUMP_VAR( event->tfinger.x );
      NOM_DUMP_VAR( event->tfinger.y );
      NOM_DUMP_VAR( event->tfinger.dx );
      NOM_DUMP_VAR( event->tfinger.dy );
      NOM_DUMP_VAR( event->tfinger.pressure );
      break;
    }

    case SDL_FINGERDOWN:
    {
      NOM_DUMP_VAR( event->tfinger.timestamp );
      NOM_DUMP_VAR( event->tfinger.touchId );
      NOM_DUMP_VAR( event->tfinger.fingerId );
      NOM_DUMP_VAR( event->tfinger.x );
      NOM_DUMP_VAR( event->tfinger.y );
      NOM_DUMP_VAR( event->tfinger.dx );
      NOM_DUMP_VAR( event->tfinger.dy );
      NOM_DUMP_VAR( event->tfinger.pressure );
      break;
    }

    case SDL_FINGERUP:
    {
      NOM_DUMP_VAR( event->tfinger.timestamp );
      NOM_DUMP_VAR( event->tfinger.touchId );
      NOM_DUMP_VAR( event->tfinger.fingerId );
      NOM_DUMP_VAR( event->tfinger.x );
      NOM_DUMP_VAR( event->tfinger.y );
      NOM_DUMP_VAR( event->tfinger.dx );
      NOM_DUMP_VAR( event->tfinger.dy );
      NOM_DUMP_VAR( event->tfinger.pressure );
      break;
    }

    case SDL_MULTIGESTURE:
    {
      NOM_DUMP_VAR( event->mgesture.timestamp );
      NOM_DUMP_VAR( event->mgesture.touchId );
      NOM_DUMP_VAR( event->mgesture.dTheta );
      NOM_DUMP_VAR( event->mgesture.dDist );
      NOM_DUMP_VAR( event->mgesture.x );
      NOM_DUMP_VAR( event->mgesture.y );
      NOM_DUMP_VAR( event->mgesture.numFingers );
      break;
    }
*/
    case SDL_DROPFILE:
    {
      char* file = event->drop.file;
      this->onDragDrop ( file, event->drop.timestamp );

      SDL_free ( file );
      file = nullptr;
      break;
    } // SDL_DROPFILE
  } // end switch event->type
}

void Input::onUserEvent ( uint32 type, int32 code, void* data1, void* data2 )
{
  // virtual implementation
}

void Input::on_quit ( void )
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

void Input::onMouseButtonFourDown ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseButtonFiveDown ( int32 x, int32 y, uint32 window_id )
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

void Input::onMouseButtonFourUp ( int32 x, int32 y, uint32 window_id )
{
#if defined ( NOM_DEBUG_SDL2_MOUSE_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( x );
  NOM_DUMP_VAR ( y );
  NOM_DUMP_VAR ( window_id );
#endif
}

void Input::onMouseButtonFiveUp ( int32 x, int32 y, uint32 window_id )
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
#if defined ( NOM_DEBUG_SDL2_JOYSTICK_BUTTON_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( which );
  NOM_DUMP_VAR ( button );
#endif
}

void Input::onJoyButtonUp ( int32 which, int32 button )
{
#if defined ( NOM_DEBUG_SDL2_JOYSTICK_BUTTON_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( which );
  NOM_DUMP_VAR ( button );
#endif
}

void Input::onJoyAxis ( int32 which, int32 axis, uint16 value )
{
#if defined ( NOM_DEBUG_SDL2_JOYSTICK_AXIS_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( which );
  NOM_DUMP_VAR ( axis );
  NOM_DUMP_VAR ( value );
#endif
}

void Input::onJoystickAdd ( void )
{
  #if defined(NOM_DEBUG_SDL2_JOYSTICK_EVENT)
    NOM_LOG_TRACE(NOM);
  #endif
}
void Input::onJoystickRemove ( void )
{
  #if defined(NOM_DEBUG_SDL2_JOYSTICK_EVENT)
    NOM_LOG_TRACE(NOM);
  #endif
}

void Input::onDragDrop ( const std::string& file_path, uint32 timestamp )
{
#if defined ( NOM_DEBUG_SDL2_DRAG_DROP_INPUT )
  NOM_LOG_TRACE ( NOM );
  NOM_DUMP_VAR ( file_path );
  NOM_DUMP_VAR ( timestamp );
#endif
}

} // namespace nom
