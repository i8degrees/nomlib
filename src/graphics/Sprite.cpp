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

Sprite::Sprite ( void )
{
NOM_LOG_TRACE ( NOM );

  this->state = 0;

  this->sheet.sprite_width = 0;
  this->sheet.sprite_height = 0;
  this->sheet.width = 0;
  this->sheet.height = 0;
  this->sheet.id = 1;
  this->sheet.spacing = 0;
  this->sheet.padding = 0;
}

Sprite::Sprite ( unsigned int width, unsigned int height )
{
NOM_LOG_TRACE ( NOM );

  this->coords.setSize ( width, height );

  this->state = 0;

  this->sheet.id = 1;
  this->sheet.sprite_width = 0;
  this->sheet.sprite_height = 0;
  this->sheet.width = width = 0;
  this->sheet.height = height = 0;
  this->sheet.spacing = 0;
  this->sheet.padding = 0;
}

Sprite::~Sprite ( void )
{
NOM_LOG_TRACE ( NOM );
}

unsigned int Sprite::getState ( void )
{
  return this->state;
}

void Sprite::setState ( unsigned int state )
{
  this->state = state;
}

signed int Sprite::getSheetID ( void )
{
  return this->sheet.id;
}

void Sprite::setSheetID ( signed int id )
{
  this->sheet.id = id;
}

void Sprite::setSheetDimensions ( int32 sheet_width, int32 sheet_height, int32 spacing, int32 padding )
{
  this->sheet.sprite_width = this->coords.width;
  this->sheet.sprite_height = this->coords.height;
  this->sheet.width = sheet_width;
  this->sheet.height = sheet_height;
  this->sheet.spacing = spacing;
  this->sheet.padding = padding;
}

bool Sprite::load ( std::string filename, Color colorkey,
                    bool use_cache, uint32 flags
                  )
{
  this->sprite_buffer.load ( filename, colorkey, use_cache, flags );

  if ( ! this->sprite_buffer.valid() )
  {
NOM_LOG_ERR ( NOM, "Could not load sprite image file: " + filename );
    return false;
  }

  return true;
}

void Sprite::Update ( void )
{
  // FIXME: Presently, we assume every sprite on our sheet is on the same row
  this->offsets.setPosition ( this->sheet.id * this->sheet.sprite_width, 0 );
  this->offsets.setSize ( this->sheet.sprite_width, this->sheet.sprite_height );
  this->sprite_buffer.setOffsets ( this->offsets );

  this->sprite_buffer.setPosition ( coords );
}

void Sprite::Draw ( void* video_buffer ) const
{
NOM_ASSERT ( this->sprite_buffer.valid() );

  this->sprite_buffer.Draw ( video_buffer );
}

bool Sprite::resize ( enum ResizeAlgorithm scaling_algorithm )
{
  if ( this->sprite_buffer.valid() == false )
  {
NOM_LOG_ERR ( NOM, "Video surface is invalid." );
    return false;
  }

  if ( this->sprite_buffer.resize ( scaling_algorithm ) == false )
  {
NOM_LOG_ERR ( NOM, "Failed to resize the video surface." );
    return false;
  }

  //this->Update();

  return true;
}


} // namespace nom
