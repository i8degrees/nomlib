/******************************************************************************

    SDL-based Sprite Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "nomlib/gfx/Sprite.hpp"

namespace nom {

Sprite::Sprite ( void )
{
NOM_LOG_CLASSINFO;

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
NOM_LOG_CLASSINFO;

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
NOM_LOG_CLASSINFO;
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

void Sprite::setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding )
{
  this->sheet.sprite_width = this->coords.width;
  this->sheet.sprite_height = this->coords.height;
  this->sheet.width = sheet_width;
  this->sheet.height = sheet_height;
  this->sheet.spacing = spacing;
  this->sheet.padding = padding;
}

bool Sprite::Load  ( std::string filename, Color colorkey,
                          bool use_cache, unsigned int flags
                        )
{
  this->sprite_buffer.loadFromImage ( filename, colorkey, use_cache, flags );

  if ( ! this->sprite_buffer.valid() )
  {
NOM_LOG_ERR ( "Could not load sprite image file: " + filename );
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


} // namespace nom
