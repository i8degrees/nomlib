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
#include "nomlib/graphics/sprite/SpriteBatch.hpp"

namespace nom {

SpriteBatch::SpriteBatch ( void ) : sheet_id_ ( 0 )
{
NOM_LOG_TRACE ( NOM );
}

SpriteBatch::SpriteBatch ( const SpriteSheet& sheet )
{
NOM_LOG_TRACE ( NOM );

  Coords dims;

  this->sprite_sheet = sheet;

  dims = this->sprite_sheet.dimensions(0);

  Sprite ( dims.w, dims.h );

  this->set_frame ( 0 );
}

SpriteBatch::SpriteBatch ( const std::string& filename )
{
NOM_LOG_TRACE ( NOM );

  Coords dims;

  this->sprite_sheet.load ( filename );

  dims = this->sprite_sheet.dimensions(0);

  Sprite ( dims.w, dims.h );

  this->set_frame ( 0 );
}

SpriteBatch& SpriteBatch::operator = ( const SpriteBatch& other )
{
  this->sprite_ = other.sprite_;
  this->set_position ( other.position().x, other.position().y );
  this->offsets = other.offsets;
  this->set_state ( other.state() );
  this->sprite_sheet = other.sprite_sheet;
  this->set_frame ( other.frame() );
  this->scale_factor = other.scale_factor;

  return *this;
}

SpriteBatch::~SpriteBatch ( void )
{
NOM_LOG_TRACE ( NOM );
}

int32 SpriteBatch::frame ( void ) const
{
//NOM_ASSERT ( this->sheet_id != ( this->frames() - 1 ) );
  return this->sheet_id_;
}

int32 SpriteBatch::frames ( void ) const
{
  return this->sprite_sheet.frames();
}

const std::string& SpriteBatch::sheet_filename ( void ) const
{
  return this->sprite_sheet.sheet_filename();
}

void SpriteBatch::set_frame ( int32 id )
{
//NOM_ASSERT ( id != ( this->frames() - 1 ) );
  this->sheet_id_ = id;

  this->update();
}

void SpriteBatch::update ( void )
{
  Sprite::update(); // Update rendering position

  Coords dims = this->sprite_sheet.dimensions ( this->frame() );

  this->offsets.x = dims.x * this->scale_factor;
  this->offsets.y = dims.y * this->scale_factor;
  this->offsets.w = dims.w * this->scale_factor;
  this->offsets.h = dims.h * this->scale_factor;

  this->sprite_.set_bounds( this->offsets );
}

} // namespace nom
