/******************************************************************************

    SDL-based Sprite Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "Sprite.hpp"

using namespace nom;

nom::Sprite::Sprite ( void )
{
NOMLIB_LOG_INFO;

  this->state = 0;

  this->sheet.sprite_width = 0;
  this->sheet.sprite_height = 0;
  this->sheet.width = 0;
  this->sheet.height = 0;
  this->sheet.id = -1;
  this->sheet.spacing = 0;
  this->sheet.padding = 0;
}

nom::Sprite::Sprite ( unsigned int width, unsigned int height )
{
NOMLIB_LOG_INFO;

  this->coords.setSize ( width, height );

  this->state = 0;

  this->sheet.id = -1;
  this->sheet.sprite_width = 0;
  this->sheet.sprite_height = 0;
  this->sheet.width = width = 0;
  this->sheet.height = height = 0;
  this->sheet.spacing = 0;
  this->sheet.padding = 0;
}

nom::Sprite::~Sprite ( void )
{
NOMLIB_LOG_INFO;
}

unsigned int nom::Sprite::getState ( void )
{
  return this->state;
}

void nom::Sprite::setState ( unsigned int state )
{
  this->state = state;
}

signed int nom::Sprite::getSheetID ( void )
{
  return this->sheet.id;
}

void nom::Sprite::setSheetID ( signed int id )
{
  this->sheet.id = id;
}

void nom::Sprite::setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding )
{
  this->sheet.sprite_width = this->coords.width;
  this->sheet.sprite_height = this->coords.height;
  this->sheet.width = sheet_width;
  this->sheet.height = sheet_height;
  this->sheet.spacing = spacing;
  this->sheet.padding = padding;
}

bool nom::Sprite::Load  ( std::string filename, nom::Color colorkey,
                          bool use_cache, unsigned int flags
                        )
{
  this->sprite_buffer.loadFromImage ( filename, colorkey, use_cache, flags );

  if ( ! this->sprite_buffer.valid() )
  {
NOMLIB_LOG_ERR ( "Could not load sprite image file: " + filename );
    return false;
  }

  return true;
}

void nom::Sprite::Update ( void )
{
  if ( this->sheet.id != -1 )
  {
    // FIXME: Presently, we assume every sprite on our sheet is on the same row
    this->offsets.setPosition ( this->sheet.id * this->sheet.sprite_width, 0 );
    this->offsets.setSize ( this->sheet.sprite_width, this->sheet.sprite_height );
    this->sprite_buffer.setOffsets ( this->offsets );
  }

  this->sprite_buffer.setPosition ( coords );
}

void nom::Sprite::Draw ( void* video_buffer ) const
{
NOMLIB_ASSERT ( this->sprite_buffer.valid() );

  this->sprite_buffer.Draw ( video_buffer );
}
