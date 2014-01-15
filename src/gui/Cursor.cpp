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
#include "nomlib/gui/Cursor.hpp"

namespace nom {

Cursor::Cursor ( void ):  locked_ ( false )
{
NOM_LOG_TRACE ( NOM );
}

Cursor::Cursor ( const SpriteSheet& sheet ):  AnimatedSprite ( sheet ),
    locked_ ( false )
{
NOM_LOG_TRACE ( NOM );
}

Cursor::Cursor ( const std::string& filename ):  AnimatedSprite ( filename ),
    locked_ ( false )
{
NOM_LOG_TRACE ( NOM );
}

Cursor::~Cursor ( void )
{
NOM_LOG_TRACE ( NOM );
}

Cursor& Cursor::operator = ( const Cursor& other )
{
  this->sprite_ = other.sprite_;
  this->set_position ( other.position().x, other.position().y );
  this->offsets = other.offsets;
  this->set_state ( other.state() );
  this->sprite_sheet = other.sprite_sheet;
  this->set_frame ( other.frame() );
  this->scale_factor = other.scale_factor;

  this->max_frames = other.max_frames;
  this->current_frame = other.current_frame;
  this->frame_increment = other.frame_increment;
  this->animation_style = other.animation_style;
  this->animation_status = other.animation_status;
  this->fps = other.fps;

  return *this;
}

int32 Cursor::x ( void ) const
{
  return this->position().x;
}

int32 Cursor::y ( void ) const
{
  return this->position().y;
}

bool Cursor::locked ( void ) const
{
  return this->locked_;
}

void Cursor::toggle_lock ( bool toggle )
{
  this->locked_ = toggle;
}

int32 Cursor::move_up ( void )
{
  return -1; // Do nothing virtual implementation
}

int32 Cursor::move_down ( void )
{
  return -1; // Do nothing virtual implementation
}

int32 Cursor::move_left ( void )
{
  return -1; // Do nothing virtual implementation
}

int32 Cursor::move_right ( void )
{
  return -1; // Do nothing virtual implementation
}

/*
void Cursor::update ( void )
{
  SpriteBatch::update();
}
*/

} // namespace nom
