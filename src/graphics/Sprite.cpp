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
#include "nomlib/graphics/Sprite.hpp"

namespace nom {

Sprite::Sprite ( void ) : state ( 0 ), sheet_id ( 0 ), scale_factor ( 1 )
{
NOM_LOG_TRACE ( NOM );
}

Sprite::Sprite ( int32 width, int32 height )  :
  Transformable { Coords ( 0, 0, width, height ) }, state ( 0 ), sheet_id ( 0 ),
  scale_factor ( 1 )

{
NOM_LOG_TRACE ( NOM );
}

Sprite::Sprite ( const SpriteSheet* sheet )
{
NOM_LOG_TRACE ( NOM );

  this->sprite_sheet = SpriteSheet::SharedPtr ( sheet->clone() );

  Coords dims = sheet->dimensions(0);

  this->coords.setSize ( dims.width, dims.height );
  this->state = 0;
  this->sheet_id = 0;
  this->scale_factor = 1;
}

/* FIXME
Sprite::Sprite ( const Canvas& copy ) : sprite  ( copy ), state ( 0 ),
  offsets ( copy.getCanvasWidth(), copy.getCanvasHeight() ),
  sprite_sheet ( nullptr ), sheet_id ( 0 ), scale_factor ( 1 )
{
NOM_LOG_TRACE ( NOM );
}
*/

Sprite& Sprite::operator = ( const Sprite& other )
{
  this->sprite = other.sprite;
  this->coords = other.coords;
  this->offsets = other.offsets;
  this->state = other.state;
  this->sprite_sheet = other.sprite_sheet;
  this->scale_factor = other.scale_factor;

  return *this;
}

Sprite::~Sprite ( void )
{
NOM_LOG_TRACE ( NOM );
}

uint32 Sprite::getState ( void )
{
  return this->state;
}

void Sprite::setState ( uint32 state )
{
  this->state = state;
}

int32 Sprite::getSheetID ( void )
{
  return this->sheet_id;
}

void Sprite::setSheetID ( int32 id )
{
  this->sheet_id = id;
}

bool Sprite::load (
                    std::string filename, Color colorkey,
                    bool use_cache, uint32 flags
                  )
{
  this->sprite.load ( filename, colorkey, use_cache, flags );

  if ( ! this->sprite.valid() )
  {
NOM_LOG_ERR ( NOM, "Could not load sprite image file: " + filename );
    return false;
  }

  return true;
}

void Sprite::Update ( void )
{
  if ( sprite_sheet )
  {
    Coords dims = this->sprite_sheet->dimensions ( this->sheet_id );

    this->offsets.setPosition ( dims.x * scale_factor, dims.y * scale_factor );
    this->offsets.setSize ( dims.width * scale_factor, dims.height * scale_factor );
    this->sprite.setOffsets ( this->offsets );
  }

  this->sprite.setPosition ( this->coords );
}

void Sprite::Draw ( void* video_buffer ) const
{
NOM_ASSERT ( this->sprite.valid() );

  this->sprite.Draw ( video_buffer );
}

bool Sprite::resize ( enum ResizeAlgorithm scaling_algorithm )
{
  if ( this->sprite.valid() == false )
  {
NOM_LOG_ERR ( NOM, "Video surface is invalid." );
    return false;
  }

  if ( this->sprite.resize ( scaling_algorithm ) == false )
  {
NOM_LOG_ERR ( NOM, "Failed to resize the video surface." );
    return false;
  }

  this->scale_factor = this->sprite.getResizeScaleFactor ( scaling_algorithm );
  this->Update();

  return true;
}


} // namespace nom
