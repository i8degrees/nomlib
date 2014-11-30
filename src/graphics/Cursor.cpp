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
#include "nomlib/graphics/Cursor.hpp"

// Private headers (third-party libs)
#include <SDL.h>

namespace nom {

namespace priv {

void free_cursor( SDL_Cursor* c )
{
  // NOM_LOG_TRACE( NOM );

  SDL_FreeCursor( c );
}

} // namespace priv

Cursor::Cursor( void ) //:
  // cursor_{ nullptr, priv::free_cursor }
{
  // NOM_LOG_TRACE( NOM );
}

Cursor::~Cursor( void )
{
  // NOM_LOG_TRACE( NOM );
}

SDL_Cursor* Cursor::cursor( void ) const
{
  return SDL_GetCursor();
}

SDL_Cursor* Cursor::default_cursor( void ) const
{
  return SDL_GetDefaultCursor();
}

MouseState Cursor::mouse_state( void )
{
  int x = 0;
  int y = 0;
  MouseState mouse;

  SDL_PumpEvents();
  mouse.state = SDL_GetMouseState( &x, &y );

  mouse.pos.x = x;
  mouse.pos.y = y;

  return mouse;
}

Cursor::Type Cursor::system_cursor( void ) const
{
  return this->system_cursor_;
}

void Cursor::set_system_cursor( Type id )
{
  SDL_Cursor* c = nullptr;

  // c = SDL_CreateSystemCursor( NOM_SCAST( SDL_SystemCursor, id ) );

  switch( id )
  {
    default:
    case Cursor::Type::Arrow:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_ARROW );
      break;
    }

    case Cursor::Type::Beam:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_IBEAM );
      break;
    }

    case Cursor::Type::Wait:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_WAIT );
      break;
    }

    case Cursor::Type::CrossHair:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_CROSSHAIR );
      break;
    }

    case Cursor::Type::WaitArrow:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_WAITARROW );
      break;
    }

    case Cursor::Type::SizeENWSE:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_SIZENWSE );
      break;
    }

    case Cursor::Type::SizeNESW:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_SIZENESW );
      break;
    }

    case Cursor::Type::SizeWE:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_SIZEWE );
      break;
    }

    case Cursor::Type::SizeNS:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_SIZENS );
      break;
    }

    case Cursor::Type::SizeAll:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_SIZEALL );
      break;
    }

    case Cursor::Type::No:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_NO );
      break;
    }

    case Cursor::Type::Hand:
    {
      c = SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_HAND );
      break;
    }
  }

  if( c == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not set system cursor: ", SDL_GetError() );
    return;
  }

  this->set_cursor( c );

  this->system_cursor_ = id;
}

void Cursor::set_cursor( SDL_Cursor* cursor )
{
  SDL_SetCursor( cursor );
}

bool Cursor::cursor_state( void ) const
{
  if( SDL_ShowCursor( SDL_QUERY ) == SDL_ENABLE )
  {
    return true;
  }
  else if( SDL_ShowCursor( SDL_QUERY ) == SDL_DISABLE )
  {
    return false;
  }

  return false;
}

void Cursor::show_cursor( bool state )
{
  if( state == true )
  {
    SDL_ShowCursor( SDL_ENABLE );
  }
  else
  {
    SDL_ShowCursor( SDL_DISABLE );
  }
}

} // namespace nom
